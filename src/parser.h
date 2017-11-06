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
	EthereumParser(const std::string & filename);

	void parseFile() const;

	std::vector<std::uint8_t> & rawData() const { return _data; }

	std::vector<Block> & blocks() const { return _blocks; }

	std::vector<RLP> & layout() const { return _layout; }

private:

	Block parseBlock(std::size_t & offset) const;

	Header fillHeader(const RLP & rlp) const;

	Transaction fillTransaction(const RLP & rlp) const;

	void fill(std::vector<std::uint8_t> & dest, const RLP & rlp) const;

	void fill(std::size_t & dest, const RLP & rlp) const;

};

#endif
