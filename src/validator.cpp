#include"block.h"
#include<tgmath.h>
#include"validator.h"
#include<iostream>
#include"keccak.h"
#include"rlp.h"

using namespace std;

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
    if (validateTransactionsRoot(parent, child) != 0) {
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

/* validates The Keccak 256-bit hash of the parent block's header, in its entirety
 * * @param Block
 * @ param Block
 * @return int
*/
int validateParentHash(const Block& parent, const Block& child) {
    /*now getting parts of Block header, creating a vector of RLPfield
     * then serializing them through RLP.serialize and calculating their hash
    */
    Header header = parent.header();
    RLPField parenthash = {header.parentHash(), false};
    RLPField ommershash = {header.ommersHash(), false};
    RLPField beneficiary = {header.beneficiary(), false};
    RLPField stateroot = {header.stateRoot(), false};
    RLPField transactionsroot = {header.transactionsRoot(), false};
    RLPField receiptsroot = {header.receiptsRoot(), false};
    RLPField logsbloom = {header.logsBloom(), false};
    RLPField difficulty = {numberToVector(header.difficulty()), false};
    RLPField number = {numberToVector(header.number()), false};
    RLPField gaslimit = {numberToVector(header.gasLimit()), false};
    RLPField gasused = {numberToVector(header.gasUsed()), false};
    RLPField timestamp = {numberToVector(header.timestamp()), false};
    RLPField extradata = {header.extraData(), false};
    RLPField mixhash = {header.mixHash(), false};
    RLPField nonce = {header.nonce(), false};
    vector<RLPField> fields = {parenthash, ommershash, beneficiary, stateroot, transactionsroot, receiptsroot, logsbloom, difficulty, number, gaslimit, gasused, timestamp, extradata, mixhash, nonce};
    vector<uint8_t> headerbytes = RLP::serialize(fields);
    vector<uint8_t> parentheaderhash = keccak(headerbytes);
    vector<uint8_t> childhash = child.header().parentHash();
    if (parentheaderhash == childhash) {
            return 0;
    }
    else {
            return 1;
    }
}

int validateBlockNumber(const Block& parent, const Block& child) {
    if (parent.header().number() == (child.header().number() - 1)) {
            return 0;
    }
    else {
            return 1;
    }
}

//difficulty formula taken from https://ethereum.stackexchange.com/questions/1880/how-is-the-mining-difficulty-calculated-on-ethereum
//doplnit že se bere maximum ze vzorce a 131072
int validateDifficulty(const Block& parent, const Block& child) {
    size_t block_diff = fmax(parent.header().difficulty() + parent.header().difficulty() / 2048 * fmax(1 - (child.header().timestamp() - parent.header().timestamp()) / 10, -99) + pow(2, ((child.header().number() / 100000) - 2)), 131072);
    if (block_diff == child.header().difficulty()) {
            return 0;
    }
    else {
            return 1;
    }
}

int validateGasLimit(const Block& parent, const Block& child) {
    if ((child.header().gasLimit() < parent.header().gasLimit() + (parent.header().gasLimit() / 1024)) && (child.header().gasLimit() > parent.header().gasLimit() - (parent.header().gasLimit() / 1024))) {
            return 0;
    }
    else {
            return 1;
    }
}

int validateTimeStamp(const Block& parent, const Block& child) {
    if (child.header().timestamp() > parent.header().timestamp()) {
            return 0;
    }
    else {
            return 1;
    }
}

//nothing for now, waiting for merkle trees
int validateTransactionsRoot(const Block& parent, const Block& child) {
    Block tmp1 = parent;
    Block tmp2 = child;
    return 0;
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

vector<uint8_t> numberToVector(size_t input) {
    vector<uint8_t> result;
    vector<uint8_t> tmp; //will be filled with bytes and later reversed into result vector
    while (input > 0) {
            tmp.push_back(input % 256);
            input /= 256;
    }
    while (!tmp.empty()) {
            result.push_back(tmp.back());
            tmp.pop_back();
    }
    return result;
}
