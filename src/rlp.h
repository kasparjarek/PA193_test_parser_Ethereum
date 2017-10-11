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
	std::vector<std::uint8_t> & _contents;
	std::size_t _prefixOff;
	std::size_t _dataOff;
	std::size_t _totalLen;
	std::size_t _dataLen;
	std::vector<RLP> _items;

public:

	RLP(std::vector<std::uint8_t> & contents, std::size_t maxLength);

	std::size_t totalLength() const { return _totalLen; }

	std::size_t dataLength() const { return _dataLen; }

	const RLP & operator[](unsigned int index) const { return _items[index]; }

	unsigned int numItems() const { return 0; }

	const std::uint8_t* data() const { return &(_contents[_dataOff]); }

	const std::uint8_t* prefix() const { return &(_contents[_prefixOff]); }
};

#endif
