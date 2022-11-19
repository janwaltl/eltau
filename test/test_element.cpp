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
    constexpr static et::Size2 exp_size{.m_row = 3, .m_col = 4};

    class DummyElement : public et::Element {
    private:
        et::Size2
        do_calc_pref_size(et::Size2 max_size) override {
            (void)max_size;
            return exp_size;
        }
        void
        do_draw(et::Window& window) override {
            (void)window;
            FAIL_CHECK("do_draw must not be called");
        }
    };


    DummyElement e;

    SECTION("Initial cache is zero") { REQUIRE(e.get_last_pref_size() == et::Size2{}); }
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
        et::Size2
        do_calc_pref_size(et::Size2 max_size) override {
            FAIL_CHECK("do_calc_pref_size must not be called.");
            return max_size;
        }
        void
        do_draw(et::Window& window) override {
            (void)window;
            FAIL_CHECK("do_draw must not be called");
        }
    };

    DummyElement e;

    REQUIRE(e.calc_pref_size({0, 1}) == et::Size2{0, 0});
    REQUIRE(e.calc_pref_size({1, 0}) == et::Size2{0, 0});
    REQUIRE(e.calc_pref_size({0, 0}) == et::Size2{0, 0});
}

TEST_CASE("Element draw is called correctly") {
    class DummyScreen : public et::Screen {
    public:
        using et::Screen::Screen;

    private:
        et::Coords2
        do_write(const et::TerminalCell& cell) noexcept override {
            (void)cell;
            return cursor() + et::Offset2{0, 1};
        }
        et::Coords2
        do_advance_cursor(const et::Offset2& offset) noexcept override {
            return cursor() + offset;
        }
    };
    static DummyScreen screen{{100, 100}};
    static et::Window exp_window{{0, 0}, {10, 11}, screen};

    class DummyElement : public et::Element {
    private:
        et::Size2
        do_calc_pref_size(et::Size2 max_size) override {
            FAIL_CHECK("do_calc_pref_size must not be called.");
            return max_size;
        }
        void
        do_draw(et::Window& window) override {
            REQUIRE(&window == &exp_window);
        }
    };


    DummyElement e;
    e.draw(exp_window);
}
