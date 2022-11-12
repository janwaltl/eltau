/*******************************************************************************
 * @file test_screen.cpp
 * @copyright Copyright 2022 Jan Waltl.
 * @license	This file is released under ElTau project's license, see LICENSE.
 ******************************************************************************/

#include <limits>
#include <sstream>
#include <unordered_set>

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <eltau/screen.hpp>

namespace et = eltau;
TEST_CASE("Window getters") {

    constexpr et::Coords2 origin{10, 1};
    constexpr et::Size2 size{3, 4};

    et::Screen screen{size};

    et::Window win{origin, size, screen};
    REQUIRE(win.size() == size);
    REQUIRE(win.origin() == origin);
    REQUIRE(win.end() == (origin + size));
}

TEST_CASE("Window inside test") {

    constexpr et::Coords2 origin{10, 16};
    et::Screen screen{{100, 100}};

    SECTION("Empty window") {
        constexpr et::Size2 size{0, 0};
        et::Window win{origin, size, screen};

        REQUIRE(!win.is_inside(origin));
    }
    SECTION("Zero-extent row window") {
        constexpr et::Size2 size{0, 13};
        et::Window win{origin, size, screen};

        REQUIRE(!win.is_inside(origin));
        REQUIRE(!win.is_inside(origin + et::Size2{0, 1}));
        REQUIRE(!win.is_inside(origin + et::Size2{1, 0}));
        REQUIRE(!win.is_inside(origin + et::Size2{0, 3}));
        REQUIRE(!win.is_inside(origin + et::Size2{0, size.m_col - 1}));
        REQUIRE(!win.is_inside(origin + et::Size2{0, size.m_col}));
    }
    SECTION("Zero-extent col window") {
        constexpr et::Size2 size{11, 0};
        et::Window win{origin, size, screen};

        REQUIRE(!win.is_inside(origin));
        REQUIRE(!win.is_inside(origin + et::Size2{1, 0}));
        REQUIRE(!win.is_inside(origin + et::Size2{0, 1}));
        REQUIRE(!win.is_inside(origin + et::Size2{3, 0}));
        REQUIRE(!win.is_inside(origin + et::Size2{size.m_row - 1, 0}));
        REQUIRE(!win.is_inside(origin + et::Size2{size.m_row, 0}));
    }
    SECTION("Ordinary window") {
        constexpr et::Size2 size{3, 5};
        et::Window win{origin, size, screen};

        for (std::size_t r = 0; r < size.m_row; ++r)
            for (std::size_t c = 0; c < size.m_col; ++c) {

                REQUIRE(win.is_inside(origin + et::Size2{r, c}));
            }

        REQUIRE(win.is_inside(origin));
        REQUIRE(!win.is_inside(origin + et::Size2{0, size.m_col}));
        REQUIRE(!win.is_inside(origin + et::Size2{size.m_row, 0}));
        REQUIRE(!win.is_inside(origin + size));
        REQUIRE(!win.is_inside(origin - et::Offset2{1, 0}));
        REQUIRE(!win.is_inside(origin - et::Offset2{0, 1}));
    }
}
TEST_CASE("Subwindow construction") {
    constexpr et::Coords2 origin{10, 16};
    constexpr et::Size2 size{5, 3};
    et::Screen screen{size};

    et::Window win{origin, size, screen};

    SECTION("Full window") {
        auto sub = win.sub_win({0, 0}, size);
        REQUIRE(sub == win);
    }
    SECTION("Larger subwindow is clamped") {
        auto sub = win.sub_win({0, 0}, et::Size2{11, 12} + size);
        // Still the same window
        REQUIRE(sub == win);
    }
    SECTION("Larger subwidnow is clamped (with offset)") {
        const et::Size2 sub_origin{1, 1};
        const et::Size2 exp_sub_size{size - sub_origin};

        auto sub = win.sub_win(sub_origin, size); // Larger size
        REQUIRE(sub.size() == exp_sub_size);
        REQUIRE(sub.origin() == (origin + sub_origin));
    }
    SECTION("Partial window") {
        const et::Size2 sub_size{size - et::Size2{2, 1}};

        auto sub = win.sub_win({0, 0}, sub_size);
        REQUIRE(sub.size() == sub_size);
    }
    SECTION("Offset origin max subwindow") {
        const et::Size2 sub_origin{1, 1};
        const et::Size2 sub_size{size - sub_origin};

        auto sub = win.sub_win(sub_origin, sub_size);
        REQUIRE(sub.size() == sub_size);
        REQUIRE(sub.origin() == (origin + sub_origin));
    }
}
TEST_CASE("Screen size") {
    SECTION("Empty screen") {
        et::Screen s{et::Size2{0, 0}};

        REQUIRE(s.size() == et::Size2{0, 0});
    }

    SECTION("Sizes") {
        auto size{GENERATE(et::Size2{0, 0}, et::Size2{1, 2}, et::Size2{53, 1}, et::Size2{2, 0}, et::Size2{0, 3})};
        et::Screen s{size};

        REQUIRE(s.size() == size);
    }
}
