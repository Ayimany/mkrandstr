#include <algorithm>
#include <cctype>
#include <cstdint>
#include <iostream>
#include <ostream>
#include <random>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

enum class error_type
{
    NONE,
    CHAR_COUNT_UNPARSEABLE,
    CHAR_COUNT_OUT_OF_BOUNDS,
    CHAR_COUNT_NO_ELEMENT
};

std::uint32_t constexpr DEFAULT_STRING_LENGTH = 10;

std::uint8_t constexpr ASCII_VISIBLE_CODEPOINT_START = '!';
std::uint8_t constexpr ASCII_VISIBLE_CODEPOINT_END   = '~';

namespace
{
[[nodiscard]]
constexpr auto
obtain_char_count(std::vector<std::string> const &vec) noexcept
    -> std::pair<std::uint64_t, error_type>
{
    auto flag    = std::find(vec.begin(), vec.end(), "--count");
    auto element = flag + 1;

    if (flag == vec.end())
    {
        return std::make_pair(DEFAULT_STRING_LENGTH, error_type::NONE);
    }

    if (element == vec.end())
    {
        return std::make_pair(-1, error_type::CHAR_COUNT_NO_ELEMENT);
    }

    for (char character : *element)
    {
        if (std::isdigit(character) == 0)
        {
            return std::make_pair(-1, error_type::CHAR_COUNT_UNPARSEABLE);
        }
    }

    try
    {
        std::uint64_t count = std::stoul(*element);
        return std::make_pair(count, error_type::NONE);
    }
    catch (std::invalid_argument const &)
    {
        return std::make_pair(-1, error_type::CHAR_COUNT_UNPARSEABLE);
    }
    catch (std::out_of_range const &)
    {
        return std::make_pair(-1, error_type::CHAR_COUNT_OUT_OF_BOUNDS);
    }

    return std::make_pair(DEFAULT_STRING_LENGTH, error_type::NONE);
}

[[nodiscard]]
constexpr auto
report_possible_error(error_type error) noexcept -> std::uint8_t
{
    switch (error)
    {
    case error_type::NONE :
        {
        }
        return 0;
    case error_type::CHAR_COUNT_UNPARSEABLE :
        {
            std::println(
                std::cout,
                "[ERR] Could not parse desired character count"
            );
        }
        return 1;
    case error_type::CHAR_COUNT_OUT_OF_BOUNDS :
        {
            std::println(
                std::cout,
                "[ERR] Desired character count is out of bounds for an "
                "unsigned 64-bit integer"
            );
        }
        return 1;
    case error_type::CHAR_COUNT_NO_ELEMENT :
        {
            std::println(
                std::cout,
                "[ERR] Please provide a number after --count"
            );
        }
        return 1;
    }
}

[[nodiscard]]
auto
generate_random_string(std::uint64_t length) noexcept -> std::string
{
    std::random_device                          dev {};
    std::mt19937                                engine { dev() };
    std::uniform_int_distribution<std::uint8_t> distro {
        ASCII_VISIBLE_CODEPOINT_START,
        ASCII_VISIBLE_CODEPOINT_END
    };

    std::vector<unsigned char> str(length);

    std::generate(
        str.begin(),
        str.end(),
        [&distro, &engine]
        {
            return static_cast<unsigned char>(distro(engine));
        }
    );

    return std::string { str.begin(), str.end() };
}

}    // namespace

auto
main(int argc, char **argv) -> int
{
    std::uint8_t             error_count { 0 };
    std::vector<std::string> args { argv + 1, argv + argc };

    std::pair<std::uint64_t, error_type> char_count { obtain_char_count(args) };
    error_count += report_possible_error(char_count.second);

    if (error_count != 0)
    {
        std::println(std::cout, "[FATAL] Too many errors emmited. Aborting.");
        return 1;
    }

    std::string str = generate_random_string(char_count.first);

    std::println(std::cout, "{}", str);

    return 0;
}
