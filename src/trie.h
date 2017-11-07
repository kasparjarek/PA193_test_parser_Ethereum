#ifndef TRIE_H
#define TRIE_H

#include <vector>
#include <map>
#include <memory>
#include <algorithm>
#include <iostream>
#include "keccak.h"
#include "rlp.h"


namespace trie {

void print_bytes(const std::vector<uint8_t> &bytes);

typedef uint8_t nibble; // Using just fisr 4 bits of byte
typedef uint8_t byte;
typedef uint32_t trie_key; // Trie key is 256 bits long.

/**
 * Convert array of bytes into array of nibble using big-endian.
 * Upper 4 bits of each byte are returned as even nibbles (starting with 0).
 * Lower 4 bits are returned as odd nibbles.
 * @param bytes
 * @return
 */
std::vector<nibble> bytes_to_nibbles(const std::vector<byte> &bytes);

/**
 * Hex-prefix encoding from yellow paper Appendix C. (page 17)
 * @param nibbles sequence of nibbles
 * @param t node type flag (Leaf node = True, Extension node = False)
 * @return
 */
std::vector<byte> hp(const std::vector<nibble> &nibbles, bool t);


/**
 * Return size of common prefix of both vectors
 * @tparam T arbitrary vector type
 * @param a
 * @param b
 * @return
 */
template<typename T>
size_t common_prefix_len(const std::vector<T> &a, const std::vector<T> &b) {
    size_t len = std::min(a.size(), b.size());
    auto its = std::mismatch(a.cbegin(), a.cbegin()+len, b.cbegin());
    return its.first - a.cbegin();
}

class Node {
public:
    /**
     * Method returns binary representation of node
     * as it should be stored in database.
     * @return
     */
    virtual std::vector<byte> structure_composition()= 0;

    /**
     * Method returns hash (key) of the node under which the node
     * should be stored in database.
     * @return
     */
    RLPField node_composition() {
        auto c = this->structure_composition();
        if (c.size() < 32) {
            return RLPField {c, true};
        }
        return RLPField {keccak(c), false};
    }

    /**
     * Add new value or update value in node or his subtree.
     * This probably cause creating new nodes and maybe replacing this node with different one,
     * so the method returns node, that should replace current one in the tree structure.
     * @param key_end
     * @param value
     */
    virtual Node* update(const std::vector<nibble> &key_end, const std::vector<byte> &value)= 0;

    virtual void print()=0;
};


class LeafNode: public Node {

    std::vector<nibble> key_end;
    std::vector<byte> value;

public:
    LeafNode(const std::vector<nibble> &key_end, const std::vector<byte> &value);
    std::vector<byte> structure_composition() override ;
    Node *update(const std::vector<nibble> &key_end, const std::vector<byte> &value) override;

    void print() override {
        printf("Leaf(");
        print_bytes(key_end);
        printf(", ");
        print_bytes(value);
        printf(")");
    }
};


class BranchNode: public Node {

    std::unique_ptr<Node> posterity[16]; // have to contain more than one entry
    std::vector<byte> value;

public:
    std::vector<byte> structure_composition() override;
    BranchNode * update(const std::vector<nibble> &key_end, const std::vector<byte> &value) override;

    void attach_node(const nibble &key, std::unique_ptr<Node> node) {
        if (posterity[key]) {
            throw std::runtime_error("BranchNode::attach_node : requested position (key value) to attach node is "
                                             "already filled.");
        }
        posterity[key] = std::move(node);
    }

    void print() override {
        printf("Branch(");
        for (const auto &child: posterity) {
            if (child) { child->print(); }
            printf(", ");
        }
        print_bytes(value);
        printf(")");
    }
};


class ExtensionNode: public Node {

    std::vector<nibble> shared_key; // have to be greater than one
    std::unique_ptr<BranchNode> next;

public:
    ExtensionNode(const std::vector<nibble> &shared_key, BranchNode *next);

    ExtensionNode(const std::vector<nibble> &shared_key, std::unique_ptr<BranchNode> next);

    std::vector<byte> structure_composition() override;

    Node *update(const std::vector<nibble> &key_end, const std::vector<byte> &value) override;

    void print() override {
        printf("Extension(");
        print_bytes(shared_key);
        printf(", ");
        next->print();
        printf(")");
    }
};


class Trie {

    std::unique_ptr<Node> root;

public:

    std::vector<byte> hash() {
        if (!root) {
            throw std::runtime_error("Empty trie: hash isn't defined.");
        }
        return keccak(root->structure_composition());
    }

    void update(const std::vector<byte> &key, const std::vector<byte> &value) {
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

    void print() {
        printf("TRIE: ");
        root->print();
        printf("\n");
    }

};

}  // namespace trie


#endif // TRIE_H
