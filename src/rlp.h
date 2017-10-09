#ifndef RLP_H
#define RLP_H

#include <cstdint>
#include <exception>
#include <vector>

class BadRLPFormat : public std::exception
{
public:
	virtual const char* what() const noexcept
	{
		return "Bad RLP format";
	}
};

class RLP
{
	std::uint8_t* _prefix;
	std::uint8_t* _data;
	std::size_t _totalLength;
	std::size_t _dataLength;
	std::vector<RLP> _items;

public:

	RLP(std::uint8_t* byteArray, std::size_t maxLength);

	std::size_t totalLength() const { return _totalLength; }

	std::size_t dataLength() const { return _dataLength; }

	const RLP & operator[](unsigned int index) const { return _items[index]; }

	unsigned int numItems() const { return 0; }

	const std::uint8_t* data() const { return _data; }

	const std::uint8_t* prefix() const { return _prefix; }
};

#endif
