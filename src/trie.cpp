#include <iomanip>
#include "trie.h"

namespace trie {

    void print_bytes(const std::vector<uint8_t> &bytes) {
        for (const auto &byte: bytes) {
            std::cout << std::setfill('0') << std::setw(2) << std::hex << (unsigned int) byte;
        }
    }

    std::vector<nibble> bytes_to_nibbles(const std::vector<byte> &bytes) {
        std::vector<nibble> nibbles;
        nibbles.reserve(bytes.size() * 2);

        for (auto byte : bytes) {
            nibbles.push_back(byte >> 4);  // upper 4 bits as even nibble
            nibbles.push_back( byte & (uint8_t) 0x0f);  // lower 4 bits as odd nibble
        }
        return nibbles;
    }

    std::vector<byte> hp(const std::vector<nibble> &nibbles, bool t) {
        uint8_t ft =  t ? (uint8_t) 2 : (uint8_t) 0;

        std::vector<byte> bytes;
        bytes.reserve((nibbles.size() / 2) + 1);

        size_t i = 0;
        if (nibbles.size() % 2 == 0) {
            bytes.push_back( (uint8_t) ft << 4);
        }
        else {
            bytes.push_back( (uint8_t) ( ((ft+1) << 4) + nibbles.at(0)) );
            ++i;
        }

        for (; i < nibbles.size(); i+=2) {
            bytes.push_back( (byte) (nibbles.at(i) << 4) + nibbles.at(i+1));
        }

        return bytes;
    }

    template<typename T>
    size_t common_prefix_len(const std::vector<T> &a, const std::vector<T> &b) {
        size_t len = std::min(a.size(), b.size());
        auto diff = std::mismatch(a.cbegin(), a.cbegin()+len, b.cbegin());
        return diff.first - a.cbegin();
    }

//    Leaf Node

    LeafNode::LeafNode(const std::vector<nibble> &key_end, const std::vector<byte> &value)
            : key_end(key_end), value(value) {}

    std::vector<byte> LeafNode::structure_composition() {
        auto hexprefix_key = hp(this->key_end, true);
        return RLP::serialize(std::vector< RLPField > {RLPField{hexprefix_key, false}, RLPField{this->value, false}});
    }

    Node *LeafNode::update(const std::vector<nibble> &key_end, const std::vector<byte> &value) {

        // Update just value
        if (this->key_end == key_end) {
            this->value = value;
            return this;
        }

        size_t prefix_len = common_prefix_len(key_end, this->key_end);

        // Replace with Branch Node
        if (prefix_len == 0) {
            auto *branch = new BranchNode;
            branch->update(this->key_end, this->value);
            branch->update(key_end, value);
            return branch;
        }

        // Create extension node and branch node
        std::vector<nibble> shared_key (key_end.cbegin(), key_end.cbegin() + prefix_len);
        auto *branch = new BranchNode;
        auto *ext_node = new ExtensionNode(shared_key, branch);

        branch->update(std::vector<nibble> (key_end.cbegin() + prefix_len, key_end.cend()), value);
        branch->update(std::vector<nibble> (this->key_end.cbegin() + prefix_len, this->key_end.cend()), this->value);

        return ext_node;
    }

    void LeafNode::print() {
        printf("Leaf(");
        print_bytes(key_end);
        printf(", ");
        print_bytes(value);
        printf(")");
    }

//    Branch Node

    std::vector<byte> BranchNode::structure_composition() {
        std::vector<RLPField> rlp_input;

        for (const auto &child: posterity) {
            if (child) {
                rlp_input.push_back(child->node_composition());
            } else {
                rlp_input.push_back(RLPField {std::vector<byte> {}, false});
            }
        }
        rlp_input.push_back(RLPField{value, false});
        return RLP::serialize(rlp_input);
    }

    BranchNode * BranchNode::update(const std::vector<nibble> &key_end, const std::vector<byte> &value) {
        if (key_end.empty()) {
            this->value = value;
            return this;
        }

        auto &child = posterity[key_end[0]];
        std::vector<nibble> new_key_end (key_end.cbegin()+1, key_end.cend());

        if (child) {
            Node *new_child = child->update(new_key_end ,value);
            if (new_child != child.get()) {
                child.reset(new_child);
            }
        } else {
            child.reset(new LeafNode(new_key_end, value));
        }

        return this;
    }

    void BranchNode::print() {
        printf("Branch(");
        for (const auto &child: posterity) {
            if (child) { child->print(); }
            printf(", ");
        }
        print_bytes(value);
        printf(")");
    }

    void BranchNode::attach_node(const nibble &key, std::unique_ptr<Node> node) {
        if (posterity[key]) {
            throw std::runtime_error("BranchNode::attach_node : "
                                     "requested position (key) to attach node is already filled.");
        }
        posterity[key] = std::move(node);
    }

//    Extension Node

    ExtensionNode::ExtensionNode(const std::vector<nibble> &shared_key, BranchNode *next)
            : shared_key(shared_key), next(next) {}

    ExtensionNode::ExtensionNode(const std::vector<nibble> &shared_key, std::unique_ptr<BranchNode> next)
            : shared_key(shared_key), next(std::move(next)) {}

    std::vector<byte> ExtensionNode::structure_composition() {
        auto hexprefix_key = hp(this->shared_key, false);
        return RLP::serialize(std::vector<RLPField> {RLPField{hexprefix_key, false}, next->node_composition()});
    }

    Node *ExtensionNode::update(const std::vector<nibble> &key_end, const std::vector<byte> &value) {
        if (shared_key.empty()) {
            throw std::runtime_error("ExtensionNode::update : invalid ExtensionNode, shared_key cannot be empty.");
        }

        size_t prefix_len = common_prefix_len(key_end, shared_key);

        // Just transfer update responsibility to the next Branch Node
        if (prefix_len == shared_key.size()) {
            if (!next) {
                throw std::runtime_error("ExtensionNode::update : key_end == shared_key, but next node is missing.");
            }
            // Update of branch node always returns this -> do not have to handle return pointer
            next->update(std::vector<nibble>(key_end.cbegin() + prefix_len, key_end.cend()), value);
            return this;
        }

        std::vector<nibble> prefix_key (shared_key.cbegin(), shared_key.cbegin() + prefix_len);
        size_t separation_index = prefix_len + 1;
        std::vector<nibble> suffix_key (shared_key.cbegin() + separation_index, shared_key.cend());

        // Create Extension Node from suffix?
        std::unique_ptr<Node> tail_node;
        if (!suffix_key.empty()) {
            tail_node.reset(new ExtensionNode(suffix_key, std::move(next)));
        } else {
            tail_node = std::move(next);
        }

        // Create new branch
        std::unique_ptr<BranchNode> new_branch (new BranchNode);
        new_branch->update(std::vector<nibble> (key_end.cbegin() + prefix_len, key_end.cend()), value); // always returns this
        new_branch->attach_node(shared_key[separation_index], std::move(tail_node));

        // Create Extension Node from shared prefix?
        if (!prefix_key.empty()) {
            this->shared_key = prefix_key;
            this->next = std::move(new_branch);
            return this;
        }

        return new_branch.release();
    }

    void ExtensionNode::print() {
        printf("Extension(");
        print_bytes(shared_key);
        printf(", ");
        next->print();
        printf(")");
    }

//    Trie

    std::vector<byte> Trie::hash() {
        if (!root) {
            throw std::runtime_error("Empty trie: hash isn't defined.");
        }
        return keccak(root->structure_composition());
    }

    void Trie::update(const std::vector<byte> &key, const std::vector<byte> &value) {
        auto key_nibble = bytes_to_nibbles(key);

        if (root) {
            Node *new_root = root->update(key_nibble, value);
            if (new_root != root.get()) {
                root.reset(new_root);
            }
        }
        else {
            root.reset(new LeafNode(key_nibble, value));
        }
    }

    void Trie::print() {
        printf("TRIE: ");
        root->print();
        printf("\n");
    }


}  // namespace trie