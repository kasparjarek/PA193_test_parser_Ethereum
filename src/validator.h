#ifndef VALIDATOR_H
#define VALIDATOR_H

#include"block.h"

    int validateAll(const Block& parent, const Block& child);
    int validateParentHash(const Block& parent, const Block& child);
    int validateBlockNumber(const Block& parent, const Block& child);
    int validateDifficulty(const Block& parent, const Block& child);
    int validateGaslimit(const Block& parent, const Block& child);
    int validateTimestamp(const Block& parent, const Block& child);
    int validateTransactionsRoot(const Block& parent, const Block& child);
    int validateNonce(const Block& parent, const Block& child); //not implemented
    int validateMixHash(const Block& parent, const Block& child); //not implemented
    int validateReceipts(Const Block b1, const Block b2); //not implemented
    int validateLogsBloom(const Block& parent, const Block& child); //not implemented

