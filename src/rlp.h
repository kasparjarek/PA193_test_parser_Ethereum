#ifndef RLP_H
#define RLP_H

#include <cstdint>
#include <exception>
#include <vector>


class RLP
{
	const std::vector<std::uint8_t> & _contents;
	std::size_t _prefixOff;
	std::size_t _dataOff;
	std::size_t _totalLen;
	std::size_t _dataLen;
	std::vector<RLP> _items;

public:
	RLP(const std::vector<std::uint8_t> & contents);

	RLP(const std::vector<std::uint8_t> & contents, std::size_t offset, 
	    std::size_t maxLength);

	std::size_t totalLength() const { return _totalLen; }

	std::size_t dataLength() const { return _dataLen; }

	const RLP & operator[](unsigned int index) const
	{ return _items[index]; }

	const RLP & at(unsigned int index) const { return _items.at(index); }

	unsigned int numItems() const { return _items.size(); }

	std::size_t dataOffset() const { return _dataOff; }

	std::size_t prefixOffset() const { return _prefixOff; }

private:
	void parseDataLength(std::size_t dataLengthSize);

	void parseItems();
};


class BadRLPFormat : public std::exception
{
public:
	virtual const char* what() const noexcept
	{
		return "Bad RLP format";
	}
};

#endif
