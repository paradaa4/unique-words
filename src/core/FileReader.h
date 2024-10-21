#pragma once

#include <fstream>
#include <string>

class FileReader
{
public:
    explicit FileReader(const std::string_view &fileName, size_t chunkSize = 1024);
    ~FileReader();

    [[nodiscard]] bool open();
    [[nodiscard]] std::string nextChunk();
    [[nodiscard]] bool hasNextChunk() const;

private:
    const std::string_view mFileName;
    const size_t mChunkSize;
    std::ifstream mInputFile;
};