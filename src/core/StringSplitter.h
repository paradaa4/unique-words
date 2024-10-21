#pragma once

#include <string>
#include <unordered_set>

namespace StringSplitter {
[[nodiscard]] std::unordered_set<std::string> split(const std::string_view &text, char delimiter = ' ');
} // namespace StringSplitter