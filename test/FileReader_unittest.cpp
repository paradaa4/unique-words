#include "FileReader.h"
#include <gtest/gtest.h>

TEST(FileReader, open)
{
    FileReader fileReader{"file.txt"};
    ASSERT_TRUE(fileReader.open());
}

TEST(FileReader, nextChunk_big)
{
    FileReader fileReader{"file.txt"};
    ASSERT_TRUE(fileReader.open());
    ASSERT_EQ(fileReader.nextChunk(), "this is a test file");
}

TEST(FileReader, nextChunk_small)
{
    FileReader fileReader{"file.txt", 6};
    ASSERT_TRUE(fileReader.open());
    ASSERT_EQ(fileReader.nextChunk(), "this");
    ASSERT_EQ(fileReader.nextChunk(), "is a");
    ASSERT_EQ(fileReader.nextChunk(), "test");
    ASSERT_EQ(fileReader.nextChunk(), "file");
}

TEST(FileReader, hasNextChunk)
{
    FileReader fileReader{"file.txt"};
    ASSERT_TRUE(fileReader.open());
    ASSERT_TRUE(fileReader.hasNextChunk());
    const auto _ = fileReader.nextChunk();
    ASSERT_FALSE(fileReader.hasNextChunk());
}