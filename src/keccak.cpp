#include "keccak.h"
#include <cryptopp/keccak.h>

static CryptoPP::Keccak_256 _keccak;
std::vector<uint8_t > keccak(const std::vector<uint8_t > &input) {
    std::vector<uint8_t> digest(_keccak.DigestSize());
    _keccak.CalculateDigest(digest.data(), input.data(), input.size());
    return digest;
}