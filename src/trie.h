#ifndef TRIE_H
#define TRIE_H

#include <vector>
#include <map>
#include <memory>
#include <algorithm>
#include <iostream>
#include "keccak.h"
#include "rlp.h"

/**
 * Implementation of Modified Merkle Patricia Trie (yellowpaper Apendix D.)
 */
namespace trie {

    typedef uint8_t nibble; // Using just first 4 bits of byte
    typedef uint8_t byte;

    /**
     * Print byte array to std::cout (used just for debugging purpose)
     * @param bytes
     */
    void print_bytes(const std::vector<uint8_t> &bytes);

    /**
     * Convert array of bytes into array of nibble using big-endian.
     * Upper 4 bits of each byte are returned as even nibbles (starting with 0).
     * Lower 4 bits are returned as odd nibbles.
     * @param bytes
     * @return nibbles
     */
    std::vector<nibble> bytes_to_nibbles(const std::vector<byte> &bytes);

    /**
     * Hex-prefix encoding from yellow paper Appendix C. (page 17)
     * @param nibbles sequence of nibbles
     * @param t node type (Leaf node = True, Extension node = False)
     * @return bytes
     */
    std::vector<byte> hp(const std::vector<nibble> &nibbles, bool t);


    /**
     * Return size of common prefix of given vectors
     * @tparam T arbitrary vector type
     * @param a vector
     * @param b vector
     * @return number of same items from the beginning of vectors
     */
    template<typename T>
    size_t common_prefix_len(const std::vector<T> &a, const std::vector<T> &b);

    class Node {
    public:
        /**
         * Method returns binary representation of node as described
         * in yellowpaper Apendix D (structural composition function c).
         * @return
         */
        virtual std::vector<byte> structure_composition()= 0;

        /**
         * Method returns hash (key) of the node as described
         * in yellowpaper Apendix D (node composition function n).
         * @return Node composition in RLPField with added information, if the byte array is hash or
         * structure composition itself (if it is serialized or not).
         */
        RLPField node_composition() {
            auto c = this->structure_composition();
            return (c.size() < 32) ? RLPField {c, true} : RLPField {keccak(c), false};
        }

        /**
         * Add new entry or update value in node and his subtree.
         * This will in some cases create new node and replace this node with the new one,
         * so the method returns node pointer, that should replace current one in the tree structure.
         * Caller of this function is responsible for deleting the returned object.
         * Note that returned pointer can be pointer to 'this' node.
         * @param key_end
         * @param value
         */
        virtual Node* update(const std::vector<nibble> &key_end, const std::vector<byte> &value)= 0;

        /**
         * Useful for debugging. This function print to std::cout structure of this node and his subtree.
         */
        virtual void print()=0;

        virtual ~Node()= default;
    };


    class LeafNode: public Node {

        std::vector<nibble> key_end;
        std::vector<byte> value;

    public:
        LeafNode(const std::vector<nibble> &key_end, const std::vector<byte> &value);
        std::vector<byte> structure_composition() override;
        Node *update(const std::vector<nibble> &key_end, const std::vector<byte> &value) override;
        void print() override;
    };


    class BranchNode: public Node {

        std::unique_ptr<Node> posterity[16];
        std::vector<byte> value;

    public:
        std::vector<byte> structure_composition() override;
        BranchNode * update(const std::vector<nibble> &key_end, const std::vector<byte> &value) override;
        void print() override;

        /**
         * Attach given node pointer into given position in branch.
         * If the position is already used, std::runtime_error will be thrown.
         * @param key
         * @param node
         */
        void attach_node(const nibble &key, std::unique_ptr<Node> node);
    };


    class ExtensionNode: public Node {

        std::vector<nibble> shared_key;
        std::unique_ptr<BranchNode> next;

    public:
        ExtensionNode(const std::vector<nibble> &shared_key, BranchNode *next);
        ExtensionNode(const std::vector<nibble> &shared_key, std::unique_ptr<BranchNode> next);
        std::vector<byte> structure_composition() override;
        Node *update(const std::vector<nibble> &key_end, const std::vector<byte> &value) override;
        void print() override;
    };


    class Trie {

        std::unique_ptr<Node> root;

    public:

        /**
         * Calculate root trie hash
         * @return
         */
        std::vector<byte> hash();

        /**
         * Add new entry or update value in the Trie.
         * @param key
         * @param value
         */
        void update(const std::vector<byte> &key, const std::vector<byte> &value);

        /**
         * Useful for debugging. This function print to std::cout structure of this Trie.
         */
        void print();

    };

}  // namespace trie


#endif // TRIE_H
