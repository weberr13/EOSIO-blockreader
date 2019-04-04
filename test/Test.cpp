#include "Test.hpp"
#include <string>
#include <vector>
#include <iostream>
#include <thread>
#include <atomic>
#include <unordered_map>

TEST_F(Test, Construct) {
}

TEST_F(Test, lessthan) {
    EXPECT_LT(1, 2);
}

TEST_F(Test, equalthing) {
    EXPECT_EQ(101, 101);
}

TEST_F(Test, truething) {
    EXPECT_TRUE(1 == 1);
    EXPECT_FALSE(1 == 2);
}

TEST_F(Test, assert) {
    assert(1 == 1);
}

