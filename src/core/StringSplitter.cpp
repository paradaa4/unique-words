#include "StringSplitter.h"

std::unordered_set<std::string> StringSplitter::split(const std::string_view &text, char delimiter)
{
    auto start = text.begin();
    auto next = std::find(start, text.end(), delimiter);
    std::unordered_set<std::string> uniqueWords;
    while (next != text.end()) {
        uniqueWords.insert(std::string{start, next});
        start = next + 1;
        next = std::find(start, text.end(), delimiter);
    }
    if (start != next) {
        uniqueWords.insert(std::string{start, next});
    }
    return uniqueWords;
}