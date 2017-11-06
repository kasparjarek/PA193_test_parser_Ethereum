#include"block.h"
#include<tgmath.h>
//#include<gcrypt.h>

using namespace std;



/* validates The Keccak 256-bit hash of the parent block's header, in its entirety
 * * @param Block
 * @ param Block
 * @return int
*/
int validateParentHash(const Block& parent, const Block& child) {
    vector<uint8_t> hash1; // place holder, will be replaced by hash of root node of previous node
    vector<uint8_t> hash2 = child.header().parentHash();
    if (hash1 == hash2) {
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

int validateGaslimit(const Block& parent, const Block& child) {
    if ((child.header().gasLimit() < parent.header().gasLimit() + (parent.header().gasLimit() / 1024)) && (child.header().gasLimit() > parent.header().gasLimit() - (parent.header().gasLimit() / 1024))) {
            return 0;
    }
    else {
            return 1;
    }
}

int validateTimestamp(const Block& parent, const Block& child) {
    if (child.header().timestamp() > parent.header().timestamp()) {
            return 0;
    }
    else {
            return 1;
    }
}

int validateNonce(const Block& parent, const Block& child) {
    /*This method is not implemented because proper verification of nonce requires download whole state database. We think that downloading of whole state database and parsing it is a bit over the main goal of this project.*/
    return 0;
}

int validateMixHash(const Block& parent, const Block& child); {
    /*This method is not implemented because proper verification of mixHash requires download whole state database. We think that downloading of whole state database and parsing it is a bit over the main goal of this project.*/
    return 0;
}

int validateReceipts(Const Block parent, const Block child) {
    /*This method is not implemented because proper verification of receipts trie requires download whole state database. We think that downloading of whole state database and parsing it is a bit over the main goal of this project.*/
    return 0;
}

int validateLogsBloom(const Block& parent, const Block& child) {
    /*This method is not implemented because we would need to implement or use EVM (Ethereum virtual machine). We think that this is strongly out of scope of this project. */
    return 0;
}
