#include <gtest/gtest.h>
#include <iostream>

TEST(HelloTest, BasicTest) {
    std::cout << "Hello World from 1 test file!" << std::endl;
    EXPECT_TRUE(true);
}