#include "mkrandstr/mkrandstr.hh"

#include <algorithm>
#include <cctype>
#include <iostream>
#include <new>
#include <ostream>
#include <random>
#include <stdexcept>
#include <utility>

namespace mkrandstr
{
constexpr auto
obtain_char_count(std::vector<std::string> const &vec) noexcept
    -> std::pair<std::uint64_t, error_type>
{
    auto flag    = std::find(vec.begin(), vec.end(), "--length");
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
}

constexpr auto
report_error_if_present(error_type error) noexcept -> void
{
    switch (error)
    {
    case error_type::NONE :
        {
        }
        break;
    case error_type::CHAR_COUNT_UNPARSEABLE :
        {
            std::println(
                std::cout,
                "[ERR] Could not parse desired character count"
            );
        }
        break;
    case error_type::CHAR_COUNT_OUT_OF_BOUNDS :
        {
            std::println(
                std::cout,
                "[ERR] Desired character count is out of bounds for an "
                "unsigned 64-bit integer"
            );
        }
        break;
    case error_type::CHAR_COUNT_NO_ELEMENT :
        {
            std::println(
                std::cout,
                "[ERR] Please provide a number after --count"
            );
        }
        break;
    }
}

auto
generate_random_string(std::uint64_t length) noexcept
    -> std::pair<std::string, error_type>
{
    std::random_device                          dev {};
    std::mt19937                                engine { dev() };
    std::uniform_int_distribution<std::uint8_t> distro {
        ASCII_VISIBLE_CODEPOINT_START,
        ASCII_VISIBLE_CODEPOINT_END
    };

    std::vector<unsigned char> str;
    try
    {
        str.resize(length);
    }
    catch (std::bad_alloc const &)
    {
        return std::make_pair("", error_type::STRING_GEN_BAD_ALLOC);
    }

    std::generate(
        str.begin(),
        str.end(),
        [&distro, &engine]
        {
            return static_cast<unsigned char>(distro(engine));
        }
    );

    return std::make_pair(
        std::string { str.begin(), str.end() },
        error_type::NONE
    );
}

}    // namespace mkrandstr

auto
main(int argc, char **argv) -> int
{
    std::vector<std::string>           args { argv + 1, argv + argc };
    std::vector<mkrandstr::error_type> errors;
    bool                               has_errors = false;

    std::pair<std::uint64_t, mkrandstr::error_type> char_count {
        mkrandstr::obtain_char_count(args)
    };
    errors.emplace_back(char_count.second);

    for (mkrandstr::error_type error : errors)
    {
        mkrandstr::report_error_if_present(error);
        if (error != mkrandstr::error_type::NONE)
        {
            has_errors = true;
        }
    }

    if (has_errors)
    {
        std::println(std::cout, "[FATAL] Too many errors emmited. Aborting.");
        return 1;
    }

    std::pair<std::string, mkrandstr::error_type> str
        = mkrandstr::generate_random_string(char_count.first);

    if (str.second == mkrandstr::error_type::STRING_GEN_BAD_ALLOC)
    {
        std::println(
            std::cout,
            "[FATAL] Cannot allocate memory to generate a string of such size."
        );
        return 1;
    }

    std::println(std::cout, "{}", str.first);

    return 0;
}

