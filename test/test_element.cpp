/*******************************************************************************
 * @file test_element.cpp
 * @copyright Copyright 2022 Jan Waltl.
 * @license	This file is released under ElTau project's license, see LICENSE.
 ******************************************************************************/

#include <catch2/catch_test_macros.hpp>
#include <eltau/element.hpp>

namespace et = eltau;

using namespace std::string_literals;

TEST_CASE("Element preferred size") {
    constexpr static et::Vector exp_size{.m_row = 3, .m_col = 4};

    class DummyElement : public et::Element {
    private:
        et::Vector
        do_calc_pref_size(et::Vector max_size) override {
            (void)max_size;
            return exp_size;
        }
        void
        do_draw(et::DrawingWindow& window) override {
            (void)window;
            FAIL_CHECK("do_draw must not be called");
        }
    };


    DummyElement e;

    SECTION("Initial cache is zero") { REQUIRE(e.get_last_pref_size() == et::Vector{}); }
    SECTION("Size is forwarded correctly") { REQUIRE(e.calc_pref_size({10, 10}) == exp_size); }
    SECTION("Size is cached correctly") {
        REQUIRE(e.get_last_pref_size() != exp_size);
        REQUIRE(e.calc_pref_size({10, 10}) == exp_size);
        REQUIRE(e.get_last_pref_size() == exp_size);
    }
}

TEST_CASE("Preferred size is not calculated for degenerate cases ") {
    class DummyElement : public et::Element {
    private:
        et::Vector
        do_calc_pref_size(et::Vector max_size) override {
            FAIL_CHECK("do_calc_pref_size must not be called.");
            return max_size;
        }
        void
        do_draw(et::DrawingWindow& window) override {
            (void)window;
            FAIL_CHECK("do_draw must not be called");
        }
    };

    DummyElement e;

    REQUIRE(e.calc_pref_size({0, 1}) == et::Vector{0, 0});
    REQUIRE(e.calc_pref_size({1, 0}) == et::Vector{0, 0});
    REQUIRE(e.calc_pref_size({0, 0}) == et::Vector{0, 0});
}

TEST_CASE("Element draw is called correctly") {
    static et::Screen screen{{10, 11}};
    static et::DrawingWindow exp_window{et::Window{{0, 0}, {10, 11}}, screen};

    class DummyElement : public et::Element {
    private:
        et::Vector
        do_calc_pref_size(et::Vector max_size) override {
            FAIL_CHECK("do_calc_pref_size must not be called.");
            return max_size;
        }
        void
        do_draw(et::DrawingWindow& window) override {
            REQUIRE(&window == &exp_window);
        }
    };


    DummyElement e;
    e.draw(exp_window);
}

SCENARIO("Text size calculation") {

    GIVEN("Empty text") {
        et::Text text{"", 0};
        WHEN("Unrestricted size is calculated") {
            auto size = text.calc_pref_size({10, 10});
            THEN("It is zero") { REQUIRE(size == et::Vector{0, 0}); }
        }
    }
    GIVEN("One character") {
        et::Text text{"a", 0};
        WHEN("Unrestricted size is calculated") {
            auto size = text.calc_pref_size({10, 10});
            THEN("It is one") { REQUIRE(size == et::Vector{1, 1}); }
        }
        WHEN("size is calculated with tight bounds") {
            auto size = text.calc_pref_size({1, 1});
            THEN("It is still correctly one") { REQUIRE(size == et::Vector{1, 1}); }
        }
    }
    GIVEN("Unwrapped sentence") {

        using namespace std::string_view_literals;
        constexpr auto sentence{"Hello"sv};

        et::Text text{sentence, 0};
        WHEN("Unrestricted size is calculated") {
            const auto size = text.calc_pref_size({10, 10});
            THEN("It is equal to sentence length") {
                REQUIRE(size == et::Vector{.m_row = 1, .m_col = sentence.length()});
            }
        }
        WHEN("size is calculated with tight bounds") {
            const auto size = text.calc_pref_size({.m_row = 1, .m_col = sentence.length()});
            THEN("It is still equal to sentence length") {
                REQUIRE(size == et::Vector{.m_row = 1, .m_col = sentence.length()});
            }
        }
        WHEN("size is stricter") {
            const auto size = text.calc_pref_size({.m_row = 10, .m_col = sentence.length() - 1});
            THEN("Last character is wrapped") {
                REQUIRE(size == et::Vector{.m_row = 2, .m_col = sentence.length() - 1});
            }
        }
        WHEN("size is most strict") {
            const auto size = text.calc_pref_size({.m_row = sentence.length(), .m_col = 1});
            THEN("Sentence is vertical") { REQUIRE(size == et::Vector{.m_row = sentence.length(), .m_col = 1}); }
        }
    }

    GIVEN("wrapped sentence") {

        using namespace std::string_view_literals;
        constexpr auto sentence{"Hello world"sv};
        constexpr auto wrap_limit = 4;

        et::Text text{sentence, wrap_limit};
        WHEN("Unrestricted size is calculated") {
            const auto size = text.calc_pref_size({100, 100});
            THEN("It is equal to sentence length") { REQUIRE(size == et::Vector{.m_row = 3, .m_col = 4}); }
        }
        WHEN("size is calculated with tight bounds") {
            const auto size = text.calc_pref_size({.m_row = 3, .m_col = wrap_limit});
            THEN("It is still equal to sentence length") {
                REQUIRE(size == et::Vector{.m_row = 3, .m_col = wrap_limit});
            }
        }
        WHEN("size is stricter") {
            const auto size = text.calc_pref_size({.m_row = 10, .m_col = wrap_limit - 1});
            THEN("Last character is wrapped") { REQUIRE(size == et::Vector{.m_row = 4, .m_col = wrap_limit - 1}); }
        }
        WHEN("size is most strict") {
            const auto size = text.calc_pref_size({.m_row = sentence.length(), .m_col = 1});
            THEN("Sentence is vertical") { REQUIRE(size == et::Vector{.m_row = sentence.length(), .m_col = 1}); }
        }
    }
}
