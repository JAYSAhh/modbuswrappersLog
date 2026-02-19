#include <gtest/gtest.h>
#include <iostream>

TEST(HelloTest1, BasicTest) {
    std::cout << "Hello World from tests_name_1 folder!" << std::endl;
    EXPECT_TRUE(true);
}