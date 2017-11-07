#include "block.h"
#include "rlp.h"

#include <vector>

using namespace std;


vector<uint8_t> Header::toRLP() const
{
	vector<RLPField> fields {
		{ _parentHash, false },
		{ _ommersHash, false },
		{ _beneficiary, false },
		{ _stateRoot, false },
		{ _transactionsRoot, false },
		{ _receiptsRoot, false },
		{ _logsBloom, false },
		{ numberToVector(_difficulty), false },
		{ numberToVector(_number), false },
		{ numberToVector(_gasLimit), false },
		{ numberToVector(_gasUsed), false },
		{ numberToVector(_timestamp), false },
		{ _extraData, false },
		{ _mixHash, false },
		{ _nonce, false }
	};

	return RLP::serialize(fields);
}


vector<uint8_t> Transaction::toRLP() const
{
	vector<RLPField> fields {
		{ numberToVector(_nonce), false },
		{ numberToVector(_gasPrice), false },
		{ numberToVector(_gasLimit), false },
		{ _to, false },
		{ _value, false },
		{ _to.empty() ? _init : _data, false },
		{ _v, false },
		{ _r, false },
		{ _s, false }
	};

	return RLP::serialize(fields);
}


vector<uint8_t> Block::toRLP() const
{
	vector<RLPField> RLPTransactions;
	for (auto t : _transactions)
		RLPTransactions.push_back({t.toRLP(), true});

	vector<RLPField> RLPOmmers;
	for (auto o : _ommers)
		RLPOmmers.push_back({o.toRLP(), true});

	vector<RLPField> fields {
		{ _header.toRLP(), true },
		{ RLP::serialize(RLPTransactions), true },
		{ RLP::serialize(RLPOmmers), true },
	};

	return RLP::serialize(fields);
}
