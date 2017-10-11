#include "rlp.h"

#include <cstdint>

using namespace std;

RLP::RLP(vector<uint8_t> & contents, size_t maxLength)
	: _contents{contents}
{
	throw BadRLPFormat();
}
