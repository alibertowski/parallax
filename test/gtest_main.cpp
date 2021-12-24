#include <gtest/gtest.h>

// Tests that the Foo::Bar() method does Abc.
TEST(FooTest, MethodBarDoesAbc) {
  EXPECT_EQ(1, 0);
}

// Tests that Foo does Xyz.
TEST(FooTest, DoesXyz) {
  EXPECT_EQ(1, 0);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
