#ifndef RLP_H
#define RLP_H

#include <exception>
#include <vector>

class BadFormat : public std::exception
{
public:
	virtual const char* what() const throw()
	{
		return "Bad RLP format";
	}
};

class RLP
{
	unsigned char* _data;
	size_t _dataLength;
	std::vector<RLP> _items;

public:

	RLP(unsigned char* data, size_t maxLength)
		: _data{data}, _dataLength{maxLength}
	{
		throw BadFormat();
	}

	size_t length() const { return 0; }

	const RLP & operator[](unsigned int index) const;

	unsigned int numItems() const { return 0; }

	const unsigned char* data() const { return _data; }
};

#endif
