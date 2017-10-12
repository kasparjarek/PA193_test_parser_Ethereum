#include "gtest/gtest.h"
#include "rlp.h"

#include <fstream>
#include <vector>
#include <stdexcept>

using namespace std;

class RLPTest : public ::testing::Test
{
protected:
	vector<uint8_t> _contents;
	size_t _maxLength;

	RLPTest()
	{
		ifstream file("../test/samples/1000000");
		file.seekg(0, ios_base::end);
		_maxLength = file.tellg();
		_contents.resize(_maxLength);
		file.seekg(0, ios_base::beg);
		file.read((char *) &(_contents[0]), _maxLength);
	}
};


TEST_F(RLPTest, BadBlockFile)
{
	vector<uint8_t> shortContents = _contents;
	shortContents.pop_back();
	try {
		RLP tmp(shortContents);
		FAIL() << "Expected BadRLPFormat exception";

	} catch (BadRLPFormat const &) {
		;
	} catch (...) {
		FAIL() << "Expected BadRLPFormat exception";
	}


	_contents[0x290] = 0x6f;
	try {
		RLP tmp(_contents);
		FAIL() << "Expected BadRLPFormat exception";

	} catch (BadRLPFormat const &) {
		;
	} catch (...) {
		FAIL() << "Expected BadRLPFormat exception";
	}
}

TEST_F(RLPTest, FirstLevelParseTest)
{
	RLP rlp{_contents};
	EXPECT_EQ(0xF9, _contents[rlp.dataOffset()]);
	EXPECT_EQ(0xF9, _contents[rlp.prefixOffset()]);
	EXPECT_EQ(768u, rlp.totalLength());
	EXPECT_EQ(765u, rlp.dataLength());
}

TEST_F(RLPTest, SecondLevelParseTest)
{
	RLP rlp{_contents};
	ASSERT_EQ(3u, rlp.numItems());
	EXPECT_EQ(0xF9, _contents[rlp[0].prefixOffset()]);
	EXPECT_EQ(0xA0, _contents[rlp[0].dataOffset()]);
	EXPECT_EQ(538u, rlp[0].totalLength());
	EXPECT_EQ(535u, rlp[0].dataLength());

	EXPECT_EQ(0xF8, _contents[rlp[1].prefixOffset()]);
	EXPECT_EQ(0xF8, _contents[rlp[1].dataOffset()]);
	EXPECT_EQ(226u, rlp[1].totalLength());
	EXPECT_EQ(224u, rlp[1].dataLength());

	EXPECT_EQ(0xc0, _contents[rlp[2].prefixOffset()]);

	EXPECT_EQ(1u, rlp[2].totalLength());
	EXPECT_EQ(0u, rlp[2].dataLength());
}

