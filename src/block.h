#ifndef BLOCK_H
#define BLOCK_H

#include "rlp.h"

#include <array>
#include <cstdint>
#include <vector>

class Header;
class Transaction;

class Block
{
	RLP _rlp;

public:
	Block(std::uint8_t* data);

	const Header & header() const;

	const vector<Transaction> & transactions() const;

	const vector<Header> & ommers() const;
};


class Header
{

public:
	const std::array<std::uint8_t, 32> & parentHash() const;
	const std::array<std::uint8_t, 32> & ommersHash() const;
	const std::array<std::uint8_t, 20> & beneficiary() const;
	const std::array<std::uint8_t, 32> & stateRoot() const;
	const std::array<std::uint8_t, 32> & transactionsRoot() const;
	const std::array<std::uint8_t, 32> & receiptsRoot() const;
	const std::array<std::uint8_t, 256> & logsBloom() const;
	std::size_t difficulty() const;
	std::size_t number() const;
	std::size_t gasLimit() const;
	std::size_t gasUsed() const;
	std::size_t timestamp() const; // maybe pass as a vector, max value is 2^256
	const std::vector<std::uint8_t> & extraData() const;
	const std::array<std::uint8_t, 32> & mixHash() const;
	const std::array<std::uint8_t, 8> & nonce() const;
};


class Transaction
{
public:
	std::size_t nonce() const;
	std::size_t gasPrice() const;
	std::size_t gasLimit() const;
	const std::vector<std::uint8_t, 20> & to() const;
	std::size_t value() const;
	unsigned int v() const;
	std::size_t r() const;
	std::size_t s() const;
	const std::vector<std::uint8_t> & init() const;
	const std::vector<std::uint8_t> & data() const;


};

#endif
