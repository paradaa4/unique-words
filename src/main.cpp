#include "FileReader.h"
#include "StringSplitter.h"
#include "ThreadPool.h"
#include <format>
#include <iostream>
#include <ranges>

constexpr size_t MAX_CHUNK_SIZE = 1024 * 1024 * 128;

int main(int argc, char *argv[])
{
    const auto args = std::vector<std::string_view>(argv + 1, argv + argc);
    if (args.size() != 1) {
        std::cerr << std::format("Filename is missing\n");
        return EXIT_FAILURE;
    }
    const auto fileName = args.at(0);
    if (!std::filesystem::exists(fileName)) {
        std::cerr << std::format("File does not exist\n");
        return EXIT_FAILURE;
    }

    ThreadPool pool;
    const auto threads = pool.size();
    const auto maxQueueSize = int(threads * 1.5);
    const auto chunkSize = std::min(std::filesystem::file_size(fileName) / maxQueueSize, MAX_CHUNK_SIZE);

    FileReader fileReader{fileName, chunkSize};
    if (!fileReader.open()) {
        std::cerr << std::format("Cannot open file\n");
        return EXIT_FAILURE;
    }

    std::vector<std::future<std::unordered_set<std::string>>> futures;
    futures.reserve(threads);
    while (fileReader.hasNextChunk()) {
        while (pool.unassignedTasks().load(std::memory_order_acquire) == maxQueueSize) {
        }
        futures.push_back(pool.enqueue(
            [](auto &&text) { return StringSplitter::split(std::string_view{*text}); },
            std::make_shared<std::string>(std::move(fileReader.nextChunk()))));
    }

    std::unordered_set<std::string> uniqueWords;
    for (auto data : futures | std::views::transform([](auto &f) { return f.get(); })) {
        uniqueWords.insert(data.begin(), data.end());
    }
    std::cout << std::format("{}\n", uniqueWords.size());

    return EXIT_SUCCESS;
}
