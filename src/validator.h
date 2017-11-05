#ifndef VALIDATOR_H
#define VALIDATOR_H

#include"block.h"

    int validateAll(const Block b1, const Block b2);
    int validateParentHash(const Block b1, const Block b2);
    int validateBlockNumber(const Block b1, const Block b2);
    int validateDifficulty(const Block b1, const Block b2);
    int validateGaslimit(const Block b1, const Block b2);
    int validateTimestamp(const Block b1, const Block b2);
    int validateTransactionsRoot(const Block b1, const Block b2);
    int validateNonce(const Block b1, const Block b2); //not implemented
    int validateMixHash(const Block b1, const Block b2); //not implemented
    int validateReceipts(Const Block b1, const Block b2); //not implemented
    int validateLogsBloom(const Block b1, const Block b2); //not implemented

