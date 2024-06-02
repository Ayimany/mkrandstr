#pragma once

#include <cstdint>
#include <string>
#include <utility>
#include <vector>

namespace mkrandstr
{
enum class error_type
{
    NONE,
    CHAR_COUNT_UNPARSEABLE,
    CHAR_COUNT_OUT_OF_BOUNDS,
    CHAR_COUNT_NO_ELEMENT,
    STRING_GEN_BAD_ALLOC
};

std::uint32_t constexpr DEFAULT_STRING_LENGTH = 10;

std::uint8_t constexpr ASCII_VISIBLE_CODEPOINT_START = '!';
std::uint8_t constexpr ASCII_VISIBLE_CODEPOINT_END   = '~';
[[nodiscard]]
constexpr auto
obtain_char_count(std::vector<std::string> const &vec) noexcept
    -> std::pair<std::uint64_t, error_type>;

constexpr auto
report_error_if_present(error_type error) noexcept -> void;

auto
generate_random_string(std::uint64_t length) noexcept
    -> std::pair<std::string, error_type>;
}    // namespace mkrandstr
