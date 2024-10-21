#include "StringSplitter.h"
#include <gtest/gtest.h>

TEST(StringSplitter, empty)
{
    ASSERT_EQ(StringSplitter::split(""), (std::unordered_set<std::string>{}));
}

TEST(StringSplitter, singleWord)
{
    ASSERT_EQ(StringSplitter::split("single"), (std::unordered_set<std::string>{"single"}));
}

TEST(StringSplitter, sentence)
{
    ASSERT_EQ(
        StringSplitter::split("an example sentence"), (std::unordered_set<std::string>{"an", "example", "sentence"}));
}

TEST(StringSplitter, wrongDelimiter)
{
    ASSERT_EQ(StringSplitter::split("an example sentence", '-'), (std::unordered_set<std::string>{"an example sentence"}));
}
