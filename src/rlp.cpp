#include "rlp.h"

#include <cstdint>

using namespace std;

RLP::RLP(uint8_t* byteArray, size_t maxLength)
	: _prefix{byteArray}, _totalLength{maxLength}
{
	throw BadRLPFormat();
}
