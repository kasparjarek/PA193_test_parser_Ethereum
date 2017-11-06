#ifndef VALIDATOR_H
#define VALIDATOR_H

#include"block.h"

    void validateAll(const Block& parent, const Block& child);
    int validateParentHash(const Block& parent, const Block& child);
    int validateBlockNumber(const Block& parent, const Block& child);
    int validateDifficulty(const Block& parent, const Block& child);
    int validateGasLimit(const Block& parent, const Block& child);
    int validateTimeStamp(const Block& parent, const Block& child);
    int validateTransactionsRoot(const Block& parent, const Block& child);
    int validateNonce(const Block& parent, const Block& child); //not implemented
    int validateMixHash(const Block& parent, const Block& child); //not implemented
    int validateReceipts(const Block& parent, const Block& child); //not implemented
    int validateLogsBloom(const Block& parent, const Block& child); //not implemented
    std::vector<std::uint8_t> numberToVector(size_t input);

#endif
