#ifndef RLP_H
#define RLP_H

#include <exception>
#include <vector>

class BadFormat : public std::exception
{
public:
	virtual const char* what() const
	{
		return "Bad RLP format";
	}
};

class RLP
{
	unsigned char* _prefix;
	unsigned char* _data;
	size_t _totalLength;
	size_t _dataLength;
	std::vector<RLP> _items;

public:

	RLP(unsigned char* byteArray, size_t maxLength)
		: _prefix{byteArray}, _totalLength{maxLength}
	{
		throw BadFormat();
	}

	size_t totalLength() const { return _totalLength; }

	size_t dataLength() const { return _dataLength; }

	const RLP & operator[](unsigned int index) const;

	unsigned int numItems() const { return 0; }

	const unsigned char* data() const { return _data; }
};

#endif
