#include"block.h"
#include<tgmath.h>

using namespace std;



/* validates The Keccak 256-bit hash of the parent block's header, in its entirety
 * * @param Block
 * @ param Block
 * @return int
*/
int validateParentHash(const Block b1, const Block b2) {
    vector<uint8_t> hash1; // place holder, will be replaced by hash of root node of previous node
    vector<uint8_t> hash2 = b2.header().parentHash();
    if (hash1 == hash2) {
            return 0;
    }
    else {
            return 1;
    }
}

int validateBlockNumber(const Block b1, const Block b2) {
    if (b1.header().number() == b2.header().number()) {
            return 0;
    }
    else {
            return 1;
    }
}

//difficulty formula taken from https://ethereum.stackexchange.com/questions/1880/how-is-the-mining-difficulty-calculated-on-ethereum
int validateDifficulty(const Block b1, const Block b2) {
    size_t block_diff = b1.header().difficulty() + b1.header().difficulty() / 2048 * fmax(1 - (b2.header().timestamp() - b1.header().timestamp()) / 10, -99) + pow(2, ((b2.header().number() / 100000) - 2));
    if (block_diff == b2.header().difficulty()) {
            return 0;
    }
    else {
            return 1;
    }
}

int validateGaslimit(const Block b1, const Block b2) {
    return 0;
}

int validateTimestamp(const Block b1, const Block b2) {
    if (b2.header().timestamp() > b1.header().timestamp()) {
            return 0;
    }
    else {
            return 1;
    }
}

int validateNonce(const Block b1, const Block b2) {
    /*This method is not implemented because proper verification of nonce requires download whole state database. We think that downloading of whole state database and parsing it is a bit over the main goal of this project.*/
    return 1;
}

int validateMixHash(const Block b1, const Block b2); {
    /*This method is not implemented because proper verification of mixHash requires download whole state database. We think that downloading of whole state database and parsing it is a bit over the main goal of this project.*/
    return 1;
}

int validateReceipts(Const Block b1, const Block b2) {
    /*This method is not implemented because proper verification of receipts trie requires download whole state database. We think that downloading of whole state database and parsing it is a bit over the main goal of this project.*/
    return 1;
}

int validateLogsBloom(const Block b1, const Block b2) {
    /*This method is not implemented because we would need to implement or use EVM (Ethereum virtual machine). We think that this is strongly out of scope of this project. */
    return 1;
}
