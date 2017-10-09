#include "gtest/gtest.h"
#include "rlp.h"

#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/mapped_region.hpp>

using namespace std;
using namespace boost::interprocess;

class RLPTest : public ::testing::Test
{
protected:
	RLPTest()
	{
		const char* filename = "../test/samples/1000000";
		file_mapping m_file(filename, read_only);
		mapped_region f(m_file,read_only);

		data_ = (unsigned char *) f.get_address();
		maxLength_ = f.get_size();
	}
	
	unsigned char* data_;
	size_t maxLength_;
};


TEST_F(RLPTest, ConstructorNotEnoughData)
{
	try {
		RLP(data_, maxLength_ - 1);
		FAIL() << "Expected BadFormat exception";

	} catch(BadFormat const & err) {
		EXPECT_STREQ(err.what(), "Bad RLP format");

	} catch(...) {
		FAIL() << "Expected BadFormat exception";
	}
}

TEST_F(RLPTest, FirstLevelParseTest)
{
	RLP block(data_, maxLength_);
	EXPECT_EQ(0xF9, block.data()[0]);
	EXPECT_EQ(0xF9, block.prefix()[0]);
	EXPECT_EQ(768, block.totalLength());
	EXPECT_EQ(765, block.dataLength());
}

TEST_F(RLPTest, SecondLevelParseTest)
{
	RLP block(data_, maxLength_);
	ASSERT_EQ(3, block.numItems());
	// TODO: change indices to enum (ETH_HEADER, ETH_TXS, etc.)
	EXPECT_EQ(0xF9, block[0].prefix()[0]);
	EXPECT_EQ(0xA0, block[0].data()[0]);
	EXPECT_EQ(538, block[0].totalLength());
	EXPECT_EQ(535, block[0].dataLength());

	EXPECT_EQ(0xF8, block[1].prefix()[0]);
	EXPECT_EQ(0xF8, block[1].data()[0]);
	EXPECT_EQ(226, block[0].totalLength());
	EXPECT_EQ(224, block[0].dataLength());

	EXPECT_EQ(0xc0, block[2].prefix()[0]);
	//EXPECT_EQ(0xA0, block[0].data()[0]);
	EXPECT_EQ(1, block[2].totalLength());
	EXPECT_EQ(0, block[2].dataLength());
}


int main(int argc, char* argv[])
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
