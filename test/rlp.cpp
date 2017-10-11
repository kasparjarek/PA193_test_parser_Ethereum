#include "gtest/gtest.h"
#include "rlp.h"

#include <fstream>
#include <vector>

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
	try {
		RLP(_contents, _maxLength - 1);
		FAIL() << "Expected BadRLPFormat exception";

	} catch (BadRLPFormat const & err) {
		EXPECT_STREQ(err.what(), "Bad RLP format");

	} catch (...) {
		FAIL() << "Expected BadRLPFormat exception";
	}


	/*try {
		RLP();
		FAIL() << "Expected BadRLPFormat exception";

	} catch (BadRLPFormat const & err) {
		EXPECT_STREQ(err.what(), "Bad RLP format");

	} catch (...) {
		FAIL() << "Expected BadRLPFormat exception";
	}*/
}

TEST_F(RLPTest, FirstLevelParseTest)
{
	RLP rlp{_contents, _maxLength};
	EXPECT_EQ(0xF9, rlp.data()[0]);
	EXPECT_EQ(0xF9, rlp.prefix()[0]);
	EXPECT_EQ(768u, rlp.totalLength());
	EXPECT_EQ(765u, rlp.dataLength());
}

TEST_F(RLPTest, SecondLevelParseTest)
{
	RLP rlp{_contents, _maxLength};
	ASSERT_EQ(3u, rlp.numItems());
	// TODO: change indices to enum (ETH_HEADER, ETH_TXS, etc.)
	EXPECT_EQ(0xF9, rlp[0].prefix()[0]);
	EXPECT_EQ(0xA0, rlp[0].data()[0]);
	EXPECT_EQ(538u, rlp[0].totalLength());
	EXPECT_EQ(535u, rlp[0].dataLength());

	EXPECT_EQ(0xF8, rlp[1].prefix()[0]);
	EXPECT_EQ(0xF8, rlp[1].data()[0]);
	EXPECT_EQ(226u, rlp[1].totalLength());
	EXPECT_EQ(224u, rlp[1].dataLength());

	EXPECT_EQ(0xc0, rlp[2].prefix()[0]);
	//EXPECT_EQ(0xA0, rlp[0].data()[0]);
	EXPECT_EQ(1u, rlp[2].totalLength());
	EXPECT_EQ(0u, rlp[2].dataLength());
}

