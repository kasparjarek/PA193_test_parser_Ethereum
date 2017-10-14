#ifndef PARSER_H
#define PARSER_H

#include "block.h"
#include "rlp.h"

#include <string>
#include <vector>

class EthereumParser
{
	std::vector<std::uint8_t> _data;
	 
public:
	EthereumParser(const std::string & filename);

	std::vector<Block> parseFile() const;

private:

	Block parseBlock(std::size_t & offset) const;

	Header fillHeader(const RLP & rlp) const;

	void fill(std::vector<std::uint8_t> & dest, const RLP & rlp) const;

	void fill(std::size_t & dest, const RLP & rlp) const;

};

#endif
