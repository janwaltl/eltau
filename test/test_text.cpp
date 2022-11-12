/*******************************************************************************
 * @file test_text.cpp
 * @copyright Copyright 2022 Jan Waltl.
 * @license	This file is released under ElTau project's license, see LICENSE.
 ******************************************************************************/

#include <catch2/catch_test_macros.hpp>
#include <eltau/text.hpp>

namespace et = eltau;

using Text = et::ascii::Text;

using namespace std::string_literals;


SCENARIO("Text size calculation") {
    GIVEN("Empty text") {
        Text text{""};
        WHEN("Unrestricted size is calculated") {
            auto size = text.calc_pref_size({10, 10});
            THEN("It is zero") { REQUIRE(size == et::Size2{0, 0}); }
        }
    }
    GIVEN("One character") {
        Text text{"a"};
        WHEN("Unrestricted size is calculated") {
            auto size = text.calc_pref_size({10, 10});
            THEN("It is one") { REQUIRE(size == et::Size2{1, 1}); }
        }
        WHEN("size is calculated with tight bounds") {
            auto size = text.calc_pref_size({1, 1});
            THEN("It is still correctly one") { REQUIRE(size == et::Size2{1, 1}); }
        }
    }
    GIVEN("Non-printable ASCIIs") {
        WHEN("STX ETX") {
            Text text{"\x02\x03"};
            auto size = text.calc_pref_size({10, 10});
            THEN("Are counted") { REQUIRE(size == et::Size2{1, 2}); }
        }
    }
    GIVEN("Unwrapped sentence") {

        using namespace std::string_view_literals;
        constexpr auto sentence{"Hello"sv};

        Text text{sentence};
        WHEN("Unrestricted size is calculated") {
            const auto size = text.calc_pref_size({10, 10});
            THEN("It is equal to sentence length") {
                REQUIRE(size == et::Size2{.m_row = 1, .m_col = sentence.length()});
            }
        }
        WHEN("size is calculated with tight bounds") {
            const auto size = text.calc_pref_size({.m_row = 1, .m_col = sentence.length()});
            THEN("It is still equal to sentence length") {
                REQUIRE(size == et::Size2{.m_row = 1, .m_col = sentence.length()});
            }
        }
        WHEN("size is stricter") {
            const auto size = text.calc_pref_size({.m_row = 10, .m_col = sentence.length() - 1});
            THEN("Last character is wrapped") {
                REQUIRE(size == et::Size2{.m_row = 2, .m_col = sentence.length() - 1});
            }
        }
        WHEN("size is most strict") {
            const auto size = text.calc_pref_size({.m_row = sentence.length(), .m_col = 1});
            THEN("Sentence is vertical") { REQUIRE(size == et::Size2{.m_row = sentence.length(), .m_col = 1}); }
        }
    }

    GIVEN("Wrapped sentence") {

        using namespace std::string_view_literals;
        constexpr auto sentence{"Hello world"sv};
        constexpr auto wrap_limit = 4;

        Text text{sentence, wrap_limit};
        WHEN("Unrestricted size is calculated") {
            const auto size = text.calc_pref_size({100, 100});
            THEN("It is equal to sentence length") { REQUIRE(size == et::Size2{.m_row = 3, .m_col = 4}); }
        }
        WHEN("size is calculated with tight bounds") {
            const auto size = text.calc_pref_size({.m_row = 3, .m_col = wrap_limit});
            THEN("It is still equal to sentence length") {
                REQUIRE(size == et::Size2{.m_row = 3, .m_col = wrap_limit});
            }
        }
        WHEN("size is stricter") {
            const auto size = text.calc_pref_size({.m_row = 10, .m_col = wrap_limit - 1});
            THEN("Last character is wrapped") { REQUIRE(size == et::Size2{.m_row = 4, .m_col = wrap_limit - 1}); }
        }
        WHEN("size is most strict") {
            const auto size = text.calc_pref_size({.m_row = sentence.length(), .m_col = 1});
            THEN("Sentence is vertical") { REQUIRE(size == et::Size2{.m_row = sentence.length(), .m_col = 1}); }
        }
    }
    GIVEN("Multiline text") {
        using namespace std::string_view_literals;
        constexpr auto sentence{"Hello\nworld\nFoo Bar\nBaz"sv};

        Text text{sentence};
        WHEN("Unrestricted size is calculated") {
            const auto size = text.calc_pref_size({100, 100});
            THEN("Size corresponds to correctly formatted text") { REQUIRE(size == et::Size2{.m_row = 4, .m_col = 7}); }
        }
        WHEN("Restricted size is calculated") {
            const auto size = text.calc_pref_size({100, 5});
            // Foo Bar is partitioned -> extra line.
            THEN("Character wrapping works") { REQUIRE(size == et::Size2{.m_row = 5, .m_col = 5}); }
        }
    }
    SECTION("Wrap limits and newline") {
        using namespace std::string_view_literals;

        SECTION("Wrap at newline") {
            constexpr auto sentence{"Hello\n"sv};
            Text text{sentence, 5};
            const auto size = text.calc_pref_size({100, 100});
            // Newline does not count into "exceeding the wrap limit" -> still two lines.
            REQUIRE(size == et::Size2{.m_row = 2, .m_col = 5});
        }
        SECTION("Wrap before newline") {
            constexpr auto sentence{"Hello\n"sv};
            Text text{sentence, 4};
            const auto size = text.calc_pref_size({100, 100});
            // 'l' is on second line, newline makes a third line
            REQUIRE(size == et::Size2{.m_row = 3, .m_col = 4});
        }
    }
}
