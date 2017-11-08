#include"block.h"
#include"validator.h"
#include"keccak.h"
#include"rlp.h"
#include"trie.h"

#include<iostream>
#include<cmath>
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
    if (validateNonce(child) != 0) {
            error_found = 1;
            cout << "Nonce is invalid." << endl;
    }
    if (validateExtraData(child) != 0) {
	    error_found = 1;
	    cout << "Extra data field is too large." << endl;
    }
    if (validateMixHash(child) != 0) {
            error_found = 1;
            cout << "Mixh hash is invalid." << endl;
    }
    if (validateOmmersRoot(child) != 0) {
            error_found = 1;
            cout << "The ommers hash of block is invalid." << endl;
    }
    if (validateTransactionsGas(child) != 0) {
            error_found = 1;
            cout << "Amount of gas used in transactions is not equal to amount of gas in block header." << endl;
    }
    if (validateReceipts(child) != 0) {
            error_found = 1;
            cout << "Hash of root of transaction receipts tree is not valid." << endl;
    }
    if (validateLogsBloom(child) != 0) {
            error_found = 1;
            cout << "Logs Bloom filters are invalid." << endl;
    }
    if (error_found == 0) {
            cout << "Block is valid." << endl;
    }
}

/* verifies if parent header hash of child block corresponds to hash of parent block header
 * see Yellow paper 4.3.4 Block Header Validity
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

/*verifies if child block has right block number according to parent block
 * see Yellow paper 4.3.4 Block Header Validity
 * @param Block - parent block
 * @param Block - child block
 * @return int - 0 if OK, else 1
 */
int validateBlockNumber(const Block& parent, const Block& child) {
    if (parent.header().number() == (child.header().number() - 1)) {
            return 0;
    }
    else {
            return 1;
    }
}

/* Verifies if child block's difficulty is in correct range limited by parent block
 * see Yellow paper 4.3.4 Block Header Validity
 * @param Block - parent block
 * @param Block - child block
 * @return int - 0 if OK, else 1
 */
int validateDifficulty(const Block& parent, const Block& child) {
    //difficulty formula taken from https://ethereum.stackexchange.com/questions/1880/how-is-the-mining-difficulty-calculated-on-ethereum and from Yellow paper
    size_t block_diff = fmax(parent.header().difficulty() + floor(parent.header().difficulty() / 2048) * fmax(1 - floor((child.header().timestamp() - parent.header().timestamp()) / 10), -99) + floor(pow(2, floor(((child.header().number() / 100000)) - 2))), 131072);
    if (block_diff == child.header().difficulty()) {
            return 0;
    }
    else {
            return 1;
    }
}

/* validates if gas limit of child is in accordance with gas limit of parent
 * see Yellow paper 4.3.4 Block Header Validity
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
 * see Yellow paper 4.3.4 Block Header Validity
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

/* Verifies the nonce of the block header
 * see Yellow paper 4.3.4 Block Header Validity and 11.4 State & Nonce Validation
 */
int validateNonce(const Block& block) {
    /*This method is not implemented because proper verification of nonce requires download whole state database. We think that downloading of whole state database and parsing it is a bit over the main goal of this project.*/
    /*
     * (n <= (^256 / child.header()difficulty()) && m = child.header().mixHash()
     * where (n, m) = proof_of_work(child.header_without_n(), child.header(), d)
     * where d = the current DAG - large data set needed to compute the mix-hash
     * and proof_of_work see 11.5 - Ethash algorithm - needs seed computed for
     * each block by scanning through the block headers up until that point, to
     * generate a large dataset
     */
    Block b = block;
    return 0;
}

/* Verifies the length of extraData field in the block header
 * @param child - block to verify
 * @return int - 0 if OK, else 1
 */
int validateExtraData(const Block& block) {
    
    return block.header().extraData().size() > 32;
}

/* Verifies the mixHash in the block header
 * see Yellow paper 4.3.4 Block Header Validity and 11.4 State & Nonce Validation
 * @param block - block to verify
 * @return int - 0 if OK, else 1
 */
int validateMixHash(const Block& block) {
    /*This method is not implemented because proper verification of mixHash requires download whole state database. We think that downloading of whole state database and parsing it is a bit over the main goal of this project.*/
    /* The mixHash is outputted by the proof_of_work function, which takes as a
     * parameter a large dataset generated using the whole state database, see 
     * validateNonce()
     */
    Block b = block;
    return 0;
}

/* Verifies the stateRoot hash in the block header
 * see Yellow paper 4.3.2 Holistic Validity
 * @param block - block to verify
 * @return int - 0 if OK, else 1
 */
int validateStateRoot(const Block& block) {
    /* Not implemented because the whole state database is needed, as well as
     * means of applying transactions to it - Ethereum Virtual machine
     */
    Block b = block;
    return 0;
}

/* Verifies ommersHash in the block header
 * see Yellow paper 4.3.2 Holistic Validity
 * @param block - block to verify
 * @return int - 0 if OK, else 1
 */
int validateOmmersRoot(const Block& block) {
    vector<RLPField> rlpommers;
    for (unsigned int i = 0; i < block.ommers().size(); ++i) {
            rlpommers.push_back({block.ommers()[i].toRLP(), true});
    }
    vector<uint8_t> serializedommers = RLP::serialize(rlpommers);
    vector<uint8_t> computedhash = keccak(serializedommers);
    if (computedhash == block.header().ommersHash()) {
            return 0;
    }
    else {
            return 1;
    }
}

/* verifies if sum of gas used in transaction is equal to number of gas used in header
 * @param Block - input block
 * @return - 0 if OK, else 1
 */
int validateTransactionsGas(const Block &block) {
    size_t computedgas = 0;
    for (unsigned int i = 0; i < block.transactions().size(); ++i) {
            computedgas += block.transactions()[i].gasPrice();
            cout << block.transactions()[i].gasPrice() << endl;
    }
    cout << computedgas << endl;
    cout << block.header().gasUsed() << endl;
    if (computedgas == block.header().gasUsed()) {
            return 0;
    }
    else {
            return 1;
    }
}

/* Verifies receiptRoot in the block header
 * see Yellow paper 4.3.2 Holistic Validity
 * @param block - block to verify
 * @returm int - 0 if OK, else 1
 */
int validateReceipts(const Block& block) {
    /*This method is not implemented because proper verification of receipts trie requires download whole state database. We think that downloading of whole state database and parsing it is a bit over the main goal of this project.*/
    /* Needs to create receipts for the transactions, which needs executing the
     * transaction code on Ethereum Virtual Machine as well as having the state
     * tree from before the execution
     */
    Block b = block;
    return 0;
}

/* Verifies logsBloom in the block header
 * see Yellow paper 4.3.2 Holistic Validity
 * @param block - block to verify
 * @return int - 0 if OK, else 1
 */
int validateLogsBloom(const Block& block) {
    /*This method is not implemented because we would need to implement or use EVM (Ethereum virtual machine). We think that this is strongly out of scope of this project. */
    /* Needs transaction receipts for which the Ethereum Virtual Machine is needed
     */
    Block b = block;
    return 0;
}

