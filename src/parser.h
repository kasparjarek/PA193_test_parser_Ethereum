#ifndef PARSER_H
#define PARSER_H

#include "block.h"
#include "rlp.h"

#include <string>
#include <vector>

class EthereumParser
{
	std::vector<std::uint8_t> _data;
	std::vector<Block> _blocks;
	std::vector<RLP> _layout;
	 
public:

	void parseFile(const std::string & filename);

	const std::vector<std::uint8_t> & rawData() const { return _data; }

	const std::vector<Block> & blocks() const { return _blocks; }

	const std::vector<RLP> & layout() const { return _layout; }

private:

	void parseBlock(std::size_t & offset);

	Header fillHeader(const RLP & rlp) const;

	Transaction fillTransaction(const RLP & rlp) const;

	void fill(std::vector<std::uint8_t> & dest, const RLP & rlp) const;

	void fill(std::size_t & dest, const RLP & rlp) const;

};

#endif
