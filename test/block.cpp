#include "gtest/gtest.h"
#include "block.h"

using namespace std;

class BlockTest : public ::testing::Test
{
protected:
	Block _block;

	BlockTest() : _block{Header(), vector<Transaction>(), vector<Header>()} {}
};

TEST_F(BlockTest, HeaderFields)
{

}
