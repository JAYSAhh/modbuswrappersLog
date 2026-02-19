#include <gtest/gtest.h>
#include <iostream>

TEST(HelloTest2, BasicTest) {
    std::cout << "Hello World from tests_name_2 folder!" << std::endl;
    EXPECT_TRUE(true);
}