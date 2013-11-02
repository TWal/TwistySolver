#include <gtest/gtest.h>
#include "Utils.h"

TEST(Utils, Factorial) {
    EXPECT_EQ(Utils::fac(0), 1);
    EXPECT_EQ(Utils::fac(5), 120);
    EXPECT_EQ(Utils::fac(8), 40320);
    EXPECT_EQ(Utils::fac(12), 479001600);
}

TEST(Utils, Pow) {
    EXPECT_EQ(Utils::pow(10, 0), 1);
    EXPECT_EQ(Utils::pow(0, 10), 0);
    EXPECT_EQ(Utils::pow(10, 2), 100);
    EXPECT_EQ(Utils::pow(2, 10), 1024);
    EXPECT_EQ(Utils::pow(3, 7), 2187);
}

TEST(Utils, Cnk) {
    EXPECT_EQ(Utils::cnk(0, 0), 1);
    EXPECT_EQ(Utils::cnk(10, 0), 1);
    EXPECT_EQ(Utils::cnk(10, 10), 1);
    EXPECT_EQ(Utils::cnk(10, 5), 252);
    EXPECT_EQ(Utils::cnk(42, 10), 1471442973);
}

