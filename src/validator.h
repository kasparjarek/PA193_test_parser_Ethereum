#ifndef VALIDATOR_H
#define VALIDATOR_H

#include"block.h"

    void validateAll(const Block& parent, const Block& child);
    int validateParentHash(const Block& parent, const Block& child);
    int validateBlockNumber(const Block& parent, const Block& child);
    int validateDifficulty(const Block& parent, const Block& child);
    int validateGasLimit(const Block& parent, const Block& child);
    int validateTimeStamp(const Block& parent, const Block& child);
    int validateTransactionsRoot(const Block& block);
    int validateNonce(const Block& block); //not implemented
    int validateExtraData(const Block& block);
    int validateMixHash(const Block& block); //not implemented
    int validateReceipts(const Block& block); //not implemented
    int validateLogsBloom(const Block& block); //not implemented

#endif
