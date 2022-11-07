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
    constexpr static et::Vec2 exp_size{.m_row = 3, .m_col = 4};

    class DummyElement : public et::Element {
    private:
        et::Vec2
        do_calc_pref_size(et::Vec2 max_size) override {
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

    SECTION("Initial cache is zero") { REQUIRE(e.get_last_pref_size() == et::Vec2{}); }
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
        et::Vec2
        do_calc_pref_size(et::Vec2 max_size) override {
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

    REQUIRE(e.calc_pref_size({0, 1}) == et::Vec2{0, 0});
    REQUIRE(e.calc_pref_size({1, 0}) == et::Vec2{0, 0});
    REQUIRE(e.calc_pref_size({0, 0}) == et::Vec2{0, 0});
}

TEST_CASE("Element draw is called correctly") {
    static et::Screen screen{{10, 11}};
    static et::DrawingWindow exp_window{et::Window{{0, 0}, {10, 11}}, screen};

    class DummyElement : public et::Element {
    private:
        et::Vec2
        do_calc_pref_size(et::Vec2 max_size) override {
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
