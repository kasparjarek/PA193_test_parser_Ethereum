#include "gtest/gtest.h"
#include "rlp.h"

TEST(FooTest, ConstructorTest)
{
	RLP rlp(4);
	EXPECT_EQ(4, rlp.foo());
}

TEST(FooTest, ConstructorFailTest)
{
	RLP rlp(4);
	EXPECT_EQ(5, rlp.foo());
}

int main(int argc, char* argv[])
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
