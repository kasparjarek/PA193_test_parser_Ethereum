#ifndef BLOCK_H
#define BLOCK_H

#include "rlp.h"

#include <cstdint>
#include <exception>
#include <vector>


class EthereumParser;

/**
 * Class representing the header of an Ethereum block
 */
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
	/** parentHash getter */
	const std::vector<std::uint8_t> & parentHash() const
	{ return _parentHash; }
	/** ommersHash getter */
	const std::vector<std::uint8_t> & ommersHash() const
	{ return _ommersHash; }
	/** beneficiary getter */
	const std::vector<std::uint8_t> & beneficiary() const
	{ return _beneficiary; }
	/** stateRoot getter */
	const std::vector<std::uint8_t> & stateRoot() const
	{ return _stateRoot; }
	/** transactionsRoot getter */
	const std::vector<std::uint8_t> & transactionsRoot() const
	{ return _transactionsRoot; }
	/** receiptsRoot getter */
	const std::vector<std::uint8_t> & receiptsRoot() const
	{ return _receiptsRoot; }
	/** logsBloom getter */
	const std::vector<std::uint8_t> & logsBloom() const
	{ return _logsBloom; }
	/** difficulty getter */
	std::size_t difficulty() const
	{ return _difficulty; }
	/** number getter */
	std::size_t number() const
	{ return _number; }
	/** gasLimit getter */
	std::size_t gasLimit() const
	{ return _gasLimit; }
	/** gasUsed getter */
	std::size_t gasUsed() const
	{ return _gasUsed; }
	/** timestamp getter */
	std::size_t timestamp() const // maybe pass as a vector, max value is 2^256
	{ return _timestamp; }
	/** extraData getter */
	const std::vector<std::uint8_t> & extraData() const
	{ return _extraData; }
	/** mixHash getter */
	const std::vector<std::uint8_t> & mixHash() const
	{ return _mixHash; }
	/** nonce getter */
	const std::vector<std::uint8_t> & nonce() const
	{ return _nonce; }

	/**
	 * Serializes the header using the RLP algorithm
	 * @return vector of bytes of serialized header
	 */
	std::vector<std::uint8_t> toRLP() const;
};


/**
 * Class representing an Ethereum transaction
 */
class Transaction
{
	enum order {
		NONCE, GAS_PRICE, GAS_LIMIT, TO, VALUE, INIT, V, R, S,
		DATA = INIT
	};
	friend class EthereumParser;

	std::size_t _nonce;
	std::size_t _gasPrice;
	std::size_t _gasLimit;
	std::vector<std::uint8_t> _to;
	std::vector<std::uint8_t> _value;
	std::vector<std::uint8_t> _v;
	std::vector<std::uint8_t> _r;
	std::vector<std::uint8_t> _s;
	std::vector<std::uint8_t> _init;
	std::vector<std::uint8_t> _data;

public:
	/** nonce getter */
	std::size_t nonce() const
	{ return _nonce; }
	/** gasPrice getter */
	std::size_t gasPrice() const
	{ return _gasPrice; }
	/** gasLimit getter */
	std::size_t gasLimit() const
	{ return _gasLimit; }
	/** to getter */
	const std::vector<std::uint8_t> & to() const
	{ return _to; }
	/** value getter */
	const std::vector<std::uint8_t> & value() const
	{ return _value; }
	/** v getter */
	const std::vector<std::uint8_t> & v() const
	{ return _v; }
	/** r getter */
	const std::vector<std::uint8_t> & r() const
	{ return _r; }
	/** s getter */
	const std::vector<std::uint8_t> & s() const
	{ return _s; }
	/** init getter */
	const std::vector<std::uint8_t> & init() const
	{ return _init; }
	/** data getter */
	const std::vector<std::uint8_t> & data() const
	{ return _data; }

	/**
	 * Serializes the transaction using the RLP algorithm
	 * @return vector of bytes of serialized transaction
	 */
	std::vector<std::uint8_t> toRLP() const;
};


/**
 * Class representing an Ethereum block
 */
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

	/** header getter */
	const Header & header() const
	{ return _header; }

	/** transaction list getter */
	const std::vector<Transaction> & transactions() const
	{ return _transactions; }

	/** ommers list getter */
	const std::vector<Header> & ommers() const
	{ return _ommers; }

	/** RLP layout getter */
	const RLP & layout() const
	{ return _layout; }

	/**
	 * Serializes the block using the RLP algorithm
	 * @return vector of bytes of serialized block
	 */
	std::vector<std::uint8_t> toRLP() const;
};


/**
 * Indicates an error in block format
 */
class BadBlockFormat : public std::exception
{
public:
	virtual const char* what() const noexcept
	{
		return "Bad Block format";
	}
};

#endif // BLOCK_H
