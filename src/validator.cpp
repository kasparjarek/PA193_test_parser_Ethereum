#include"block.h"
#include<tgmath.h>
#include"validator.h"
#include<iostream>
#include"keccak.h"
#include"rlp.h"
#include"trie.h"
#include<iterator>

using namespace std;


/** runs all validation checks, prints errors
 * @param Block  - parent block
 * @param Block - child block
 * @return void
 */
void validateAll(const Block& parent, const Block& child) {
    unsigned short int error_found = 0; //signals if some validation faile 0 means nothing failed;
    if (validateParentHash(parent, child) != 0) {
            error_found = 1;
            cout << "Parent hash is invalid." << endl;
    }
    if (validateBlockNumber(parent, child) != 0) {
            error_found = 1;
            cout << "Block number is invalid." << endl;
    }
    if (validateDifficulty(parent, child) != 0) {
            error_found = 1;
            cout << "Block difficulty is invalid." << endl;
    }
    if (validateGasLimit(parent, child) != 0) {
            error_found = 1;
            cout << "Gas limit is invalid." << endl;
    }
    if (validateTimeStamp(parent, child) != 0) {
            error_found = 1;
            cout << "Timestamp is invalid." << endl;
    }
    if (validateTransactionsRoot(child) != 0) {
            error_found = 1;
            cout << "Hash of root of transactions tree is invalid." << endl;
    }
    if (validateNonce(parent, child) != 0) {
            error_found = 1;
            cout << "Nonce is invalid." << endl;
    }
    if (validateMixHash(parent, child) != 0) {
            error_found = 1;
            cout << "Mixh hash is invalid." << endl;
    }
    if (validateReceipts(parent, child) != 0) {
            error_found = 1;
            cout << "Hash of root of transaction receipts tree is not valid." << endl;
    }
    if (validateLogsBloom(parent, child) != 0) {
            error_found = 1;
            cout << "Logs Bloom filters are invalid." << endl;
    }
    if (error_found == 0) {
            cout << "Block is valid." << endl;
    }
}

/* verifies if parent header hash of child block corresponds to hash of parent block header
 * * @param Block - parent block
 * @ param Block - child block
 * @return int - 0 if OK, else 1
*/
int validateParentHash(const Block& parent, const Block& child) {
    /*now getting parts of Block header, creating a vector of RLPfield
     * then serializing them through RLP.serialize and calculating their hash
    */
    Header header = parent.header();
    vector<uint8_t> headerbytes = header.toRLP();
    vector<uint8_t> parentheaderhash = keccak(headerbytes);
    vector<uint8_t> childhash = child.header().parentHash();
    if (parentheaderhash == childhash) {
            return 0;
    }
    else {
            return 1;
    }
}

/*verifies if child block has right block number according to parent block, see Yellow paper - Block header validity
 * @param Block - parent block
 * @param Block - child block
 * @return int - 0 if OK, else 1
 */
int validateBlockNumber(const Block& parent, const Block& child) {
    cout << "Parent number " << parent.header().number() << endl;
    if (parent.header().number() == (child.header().number() - 1)) {
            return 0;
    }
    else {
            return 1;
    }
}

/* Verifies if child block's difficulty is in correct range limited by parent block, see Yellow paper - Block header validity
 * @param Block - parent block
 * @param Block - child block
 * @return int - 0 if OK, else 1
 */
int validateDifficulty(const Block& parent, const Block& child) {
    //difficulty formula taken from https://ethereum.stackexchange.com/questions/1880/how-is-the-mining-difficulty-calculated-on-ethereum and from Yellow paper
    size_t block_diff = fmax(parent.header().difficulty() + parent.header().difficulty() / 2048 * fmax(1 - (child.header().timestamp() - parent.header().timestamp()) / 10, -99) + pow(2, ((child.header().number() / 100000) - 2)), 131072);
    if (block_diff == child.header().difficulty()) {
            return 0;
    }
    else {
            return 1;
    }
}

/* validates if gas limit of child is in accordance with gas limit of parent, see Yellow paper - block header validity
 * @param Block - parent
 * @param Block - child
 * @return int - 0 if OK, else 1
 */
int validateGasLimit(const Block& parent, const Block& child) {
    if ((child.header().gasLimit() < parent.header().gasLimit() + (parent.header().gasLimit() / 1024)) && (child.header().gasLimit() > parent.header().gasLimit() - (parent.header().gasLimit() / 1024))) {
            return 0;
    }
    else {
            return 1;
    }
}

/* verifies if child's timestamp is greater then parent's timestamp
 * @param Block - parent
 * @param Block - child
 * @return int - 0 if OK, else 1
 */
int validateTimeStamp(const Block& parent, const Block& child) {
    if (child.header().timestamp() > parent.header().timestamp()) {
            return 0;
    }
    else {
            return 1;
    }
}


/* verifies if transactions root hash stored in header corresponds to hash of root of transactions tree stored in block
 * @param Block - block to check
 * @return int - 0 if OK, else 1
 */
int validateTransactionsRoot(const Block& block) {
    trie::Trie trie;

    for (size_t i = 0; i < block.transactions().size(); ++i) {
        vector<uint8_t> index = numberToVector(i);
        index = RLP::serialize(vector<RLPField> {RLPField{index, false}});
        trie.update(index, block.transactions().at(i).toRLP());
    }

    if (trie.hash() == block.header().transactionsRoot()) {
        return 0;
    }
    else {
        return 1;
    }
}

int validateNonce(const Block& parent, const Block& child) {
    /*This method is not implemented because proper verification of nonce requires download whole state database. We think that downloading of whole state database and parsing it is a bit over the main goal of this project.*/
    Block tmp1 = parent;
    Block tmp2 = child;
    return 0;
}

int validateMixHash(const Block& parent, const Block& child) {
    /*This method is not implemented because proper verification of mixHash requires download whole state database. We think that downloading of whole state database and parsing it is a bit over the main goal of this project.*/
    Block tmp1 = parent;
    Block tmp2 = child;
    return 0;
}

int validateReceipts(const Block& parent, const Block& child) {
    /*This method is not implemented because proper verification of receipts trie requires download whole state database. We think that downloading of whole state database and parsing it is a bit over the main goal of this project.*/
    Block tmp1 = parent;
    Block tmp2 = child;
    return 0;
}

int validateLogsBloom(const Block& parent, const Block& child) {
    /*This method is not implemented because we would need to implement or use EVM (Ethereum virtual machine). We think that this is strongly out of scope of this project. */
    Block tmp1 = parent;
    Block tmp2 = child;
    return 0;
}

