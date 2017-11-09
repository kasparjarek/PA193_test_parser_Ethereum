#ifndef PARSER_H
#define PARSER_H

#include "block.h"
#include "rlp.h"

#include <string>
#include <vector>

/**
 * EthereumParser class
 * provides parsing of ethereum blocks from raw files
 */
class EthereumParser
{
	std::vector<std::uint8_t> _data;
	std::vector<Block> _blocks;
	 
public:

	/**
	 * Parse raw byte file containing ethereum blocks and store those blocks internally
	 * @param filename path to the file
	 */
	void parseFile(const std::string & filename);

	/**
	 * Access the raw bytes of the latest parsed file
	 * @return vector of bytes of the parsed file
	 */
	const std::vector<std::uint8_t> & rawData() const { return _data; }

	/**
	 * Return parsed blocks
	 * @return vector of parsed blocks
	 */
	const std::vector<Block> & blocks() const { return _blocks; }

private:

	void parseBlock(std::size_t & offset);

	Header fillHeader(const RLP & rlp) const;

	Transaction fillTransaction(const RLP & rlp) const;

	void fill(std::vector<std::uint8_t> & dest, const RLP & rlp) const;

	void fill(std::size_t & dest, const RLP & rlp) const;

};

#endif // PARSER_H
