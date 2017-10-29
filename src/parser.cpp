#include "parser.h"
#include "block.h"
#include "rlp.h"

#include <cstring>
#include <fstream>
#include <vector>

using namespace std;

EthereumParser::EthereumParser(const string & filename)
{
	ifstream file(filename);
	file.seekg(0, ios_base::end);
	size_t fileSize = file.tellg();

	_data.resize(fileSize);
	file.seekg(0, ios_base::beg);
	file.read((char *) &(_data[0]), fileSize);
}

 
vector<Block> EthereumParser::parseFile() const
{
	vector<Block> out;
	size_t offset = 0;
	while (offset < _data.size())
		out.push_back(parseBlock(offset));

	return out;
}


Block EthereumParser::parseBlock(size_t & offset) const
{
	RLP rlp{_data, offset, _data.size() - offset};
	offset += rlp.totalLength();

	Header header = fillHeader(rlp[Block::HEADER]);

	vector<Transaction> transactions;
	for (unsigned i = 0; i < rlp[Block::TRANSACTIONS].numItems(); ++i)
		transactions.push_back(
			fillTransaction(rlp[Block::TRANSACTIONS][i]));

	vector<Header> ommers;
	for (unsigned i = 0; i < rlp[Block::OMMERS].numItems(); ++i)
		ommers.push_back(fillHeader(rlp[Block::OMMERS][i]));

	return Block{header, transactions, ommers};
}


Header EthereumParser::fillHeader(const RLP & rlp) const
{
	Header header;

	fill(header._parentHash, rlp[Header::PARENT_HASH]);
	fill(header._ommersHash, rlp[Header::OMMERS_HASH]);
	fill(header._beneficiary, rlp[Header::BENEFICIARY]);
	fill(header._stateRoot, rlp[Header::STATE_ROOT]);
	fill(header._transactionsRoot, rlp[Header::TRANSACTIONS_ROOT]);
	fill(header._receiptsRoot, rlp[Header::RECEIPTS_ROOT]);
	fill(header._logsBloom, rlp[Header::LOGS_BLOOM]);
	fill(header._difficulty, rlp[Header::DIFFICULTY]);
	fill(header._number, rlp[Header::NUMBER]);
	fill(header._gasLimit, rlp[Header::GAS_LIMIT]);
	fill(header._gasUsed, rlp[Header::GAS_USED]);
	fill(header._timestamp, rlp[Header::TIMESTAMP]);
	fill(header._extraData, rlp[Header::EXTRA_DATA]);
	fill(header._mixHash, rlp[Header::MIX_HASH]);
	fill(header._nonce, rlp[Header::NONCE]);

	return header;
}


Transaction EthereumParser::fillTransaction(const RLP & rlp) const
{
	Transaction transaction;

	fill(transaction._nonce, rlp[Transaction::NONCE]);
	fill(transaction._gasPrice, rlp[Transaction::GAS_PRICE]);
	fill(transaction._gasLimit, rlp[Transaction::GAS_LIMIT]);
	fill(transaction._to, rlp[Transaction::TO]);
	fill(transaction._value, rlp[Transaction::VALUE]);
	fill(transaction._v, rlp[Transaction::V]);
	fill(transaction._r, rlp[Transaction::R]);
	fill(transaction._s, rlp[Transaction::S]);
	if (transaction.to().size() == 0)
		fill(transaction._init, rlp[Transaction::INIT]);
	else
		fill(transaction._data, rlp[Transaction::INIT]);

	return transaction;
}


void EthereumParser::fill(vector<uint8_t> & dest, const RLP & r) const 
{
	dest.resize(r.dataLength());
	memcpy(dest.data(), &this->_data[r.dataOffset()], dest.size());
}

 
void EthereumParser::fill(size_t & dest, const RLP & r) const
{
	dest = 0;
	for (unsigned i = 0; i < r.dataLength(); ++i) {
		dest *= 256;
		dest += this->_data[r.dataOffset() + i];
	}
}

