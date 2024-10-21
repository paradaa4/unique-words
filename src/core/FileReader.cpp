#include "FileReader.h"

FileReader::FileReader(const std::string_view &fileName, size_t chunkSize)
    : mFileName{fileName}
    , mChunkSize{chunkSize}
{}

FileReader::~FileReader()
{
    if (mInputFile.is_open()) {
        mInputFile.close();
    }
}

bool FileReader::open()
{
    mInputFile.open(mFileName);
    return mInputFile.good();
}

std::string FileReader::nextChunk()
{
    std::string buffer;
    buffer.resize(mChunkSize);
    mInputFile.read(buffer.data(), mChunkSize);
    const auto bytesRead = mInputFile.gcount();
    if (!bytesRead) {
        return {};
    }
    if (bytesRead != mChunkSize) {
        buffer.resize(bytesRead);
    } else if (const auto lastSpace = buffer.find_last_of(' '); lastSpace != std::string::npos) {
        const auto toSeek = mChunkSize - lastSpace;
        mInputFile.seekg(-toSeek + 1, std::ios_base::cur);
        buffer.resize(lastSpace);
    }

    return buffer;
}

bool FileReader::hasNextChunk() const
{
    return !mInputFile.eof();
}
