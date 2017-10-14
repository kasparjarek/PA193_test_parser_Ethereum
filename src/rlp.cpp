#include "rlp.h"

#include <cstdint>

using namespace std;


RLP::RLP(const vector<uint8_t> & contents)
	: RLP{contents, 0, contents.size()}
{}

RLP::RLP(const vector<uint8_t> & contents, size_t offset, size_t maxLength)
	: _contents{contents}, _prefixOff{offset}
{
	if (maxLength == 0)
		throw BadRLPFormat();

	uint8_t prefix = _contents[_prefixOff];
	if (prefix < 128) {
		_dataLen = 1;
		_totalLen = 1;
		_dataOff = _prefixOff;

	} else if (prefix <= 183) {
		_dataLen = prefix - 128;
		_totalLen = 1 + _dataLen;
		_dataOff = _prefixOff + 1;

	} else if (prefix < 192) {
		size_t dataLengthSize = prefix - 183;
		if (maxLength < 1 + dataLengthSize)
			throw BadRLPFormat();

		parseDataLength(dataLengthSize);
		_totalLen = 1 + dataLengthSize + _dataLen;
		_dataOff = _prefixOff + 1 + dataLengthSize;

	} else if (prefix <= 247) {
		_dataLen = prefix - 192;
		_totalLen = 1 + _dataLen;
		_dataOff = _prefixOff + 1;

	} else {
		size_t dataLengthSize = prefix - 247;
		if (maxLength < 1 + dataLengthSize)
			throw BadRLPFormat();

		parseDataLength(dataLengthSize);
		_totalLen = 1 + dataLengthSize + _dataLen;
		_dataOff = _prefixOff + 1 + dataLengthSize;
	}

	if (_totalLen > maxLength)
		throw BadRLPFormat();

	/* Add children */
	if (prefix >= 192)
		parseItems();
}


void RLP::parseDataLength(size_t dataLengthSize)
{
	_dataLen = 0;
	for (size_t i = 0; i < dataLengthSize; ++i) {
		_dataLen *= 256;
		_dataLen += _contents[_prefixOff + 1 + i];
	}
}


void RLP::parseItems()
{
	size_t remLen = _dataLen;
	size_t parseOffset = _dataOff;
	while (remLen > 0) {
		_items.push_back(RLP{_contents, parseOffset, remLen});
		remLen -= _items.back().totalLength();
		parseOffset += _items.back().totalLength();
	}
}

