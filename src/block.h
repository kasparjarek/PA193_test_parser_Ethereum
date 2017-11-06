#ifndef BLOCK_H
#define BLOCK_H

#include "rlp.h"

#include <cstdint>
#include <exception>
#include <vector>


class EthereumParser;

class Header
{
	enum order {
		PARENT_HASH, OMMERS_HASH, BENEFICIARY, STATE_ROOT,
		TRANSACTIONS_ROOT, RECEIPTS_ROOT, LOGS_BLOOM, DIFFICULTY,
		NUMBER, GAS_LIMIT, GAS_USED, TIMESTAMP, EXTRA_DATA, MIX_HASH,
		NONCE
	};
	friend class EthereumParser;

	std::vector<std::uint8_t> _parentHash;
	std::vector<std::uint8_t> _ommersHash;
	std::vector<std::uint8_t> _beneficiary;
	std::vector<std::uint8_t> _stateRoot;
	std::vector<std::uint8_t> _transactionsRoot;
	std::vector<std::uint8_t> _receiptsRoot;
	std::vector<std::uint8_t> _logsBloom;
	std::size_t _difficulty;
	std::size_t _number;
	std::size_t _gasLimit;
	std::size_t _gasUsed;
	std::size_t _timestamp;
	std::vector<std::uint8_t> _extraData;
	std::vector<std::uint8_t> _mixHash;
	std::vector<std::uint8_t> _nonce;

public:
	Header() {}; 

	const std::vector<std::uint8_t> & parentHash() const
	{ return _parentHash; }
	const std::vector<std::uint8_t> & ommersHash() const
	{ return _ommersHash; }
	const std::vector<std::uint8_t> & beneficiary() const
	{ return _beneficiary; }
	const std::vector<std::uint8_t> & stateRoot() const
	{ return _stateRoot; }
	const std::vector<std::uint8_t> & transactionsRoot() const
	{ return _transactionsRoot; }
	const std::vector<std::uint8_t> & receiptsRoot() const
	{ return _receiptsRoot; }
	const std::vector<std::uint8_t> & logsBloom() const
	{ return _logsBloom; }
	std::size_t difficulty() const
	{ return _difficulty; }
	std::size_t number() const
	{ return _number; }
	std::size_t gasLimit() const
	{ return _gasLimit; }
	std::size_t gasUsed() const
	{ return _gasUsed; }
	std::size_t timestamp() const // maybe pass as a vector, max value is 2^256
	{ return _timestamp; }
	const std::vector<std::uint8_t> & extraData() const
	{ return _extraData; }
	const std::vector<std::uint8_t> & mixHash() const
	{ return _mixHash; }
	const std::vector<std::uint8_t> & nonce() const
	{ return _nonce; }
};


class Transaction
{
	enum order {
		NONCE, GAS_PRICE, GAS_LIMIT, TO, VALUE, V, R, S, INIT,
		DATA = INIT
	};
	friend class EthereumParser;

	std::size_t _nonce;
	std::size_t _gasPrice;
	std::size_t _gasLimit;
	std::vector<std::uint8_t> _to;
	std::size_t _value;
	std::size_t _v;
	std::size_t _r;
	std::size_t _s;
	std::vector<std::uint8_t> _init;
	std::vector<std::uint8_t> _data;

public:
	std::size_t nonce() const
	{ return _nonce; }
	std::size_t gasPrice() const
	{ return _gasPrice; }
	std::size_t gasLimit() const
	{ return _gasLimit; }
	const std::vector<std::uint8_t> & to() const
	{ return _to; }
	std::size_t value() const
	{ return _value; }
	std::size_t v() const
	{ return _v; }
	std::size_t r() const
	{ return _r; }
	std::size_t s() const
	{ return _s; }
	const std::vector<std::uint8_t> & init() const
	{ return _init; }
	const std::vector<std::uint8_t> & data() const
	{ return _data; }

};

class Block
{
	enum order { HEADER, TRANSACTIONS, OMMERS };
	friend class EthereumParser;

	Header _header;
	std::vector<Transaction> _transactions;
	std::vector<Header> _ommers;

	RLP _layout;

public:
	Block(Header header, std::vector<Transaction> transactions,
			std::vector<Header> ommers, RLP layout)
		: _header{header}, _transactions{transactions}, _ommers{ommers},
		_layout{layout}
	{}

	const Header & header() const
	{ return _header; }

	const std::vector<Transaction> & transactions() const
	{ return _transactions; }

	const std::vector<Header> & ommers() const
	{ return _ommers; }

	const RLP & layout() const
	{ return _layout; }
};


class BadBlockFormat : public std::exception
{
public:
	virtual const char* what() const noexcept
	{
		return "Bad Block format";
	}
};

#endif
