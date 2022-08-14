/*******************************************************************************
 * @file test_screen.cpp
 * @copyright Copyright 2022 Jan Waltl.
 * @license	This file is released under ElTau project's license, see LICENSE.
 ******************************************************************************/

#include <limits>

#include <catch2/catch_test_macros.hpp>
#include <eltau/screen.hpp>

namespace et = eltau;
TEST_CASE("Vector min") {
    REQUIRE(min(et::Vector{0, 0}, et::Vector{1, 2}) == et::Vector{0, 0});
    REQUIRE(min(et::Vector{5, 1}, et::Vector{1, 5}) == et::Vector{1, 1});
    REQUIRE(min(et::Vector{5, 7}, et::Vector{7, 9}) == et::Vector{5, 7});
    REQUIRE(min(et::Vector{5, 7}, et::Vector{3, 1}) == et::Vector{3, 1});
}

TEST_CASE("Vector max") {
    REQUIRE(max(et::Vector{0, 0}, et::Vector{1, 2}) == et::Vector{1, 2});
    REQUIRE(max(et::Vector{5, 1}, et::Vector{1, 5}) == et::Vector{5, 5});
    REQUIRE(max(et::Vector{5, 7}, et::Vector{7, 9}) == et::Vector{7, 9});
    REQUIRE(max(et::Vector{5, 7}, et::Vector{3, 1}) == et::Vector{5, 7});
}

TEST_CASE("Vector subtraction") {
    constexpr auto m = std::numeric_limits<std::size_t>::max();
    REQUIRE(et::Vector{0, 0} - et::Vector{1, 2} == et::Vector{0, 0});
    REQUIRE(et::Vector{10, 5} - et::Vector{1, 7} == et::Vector{9, 0});
    REQUIRE(et::Vector{12, 18} - et::Vector{20, 18} == et::Vector{0, 0});
    REQUIRE(et::Vector{2, 13} - et::Vector{3, 11} == et::Vector{0, 2});
    REQUIRE(et::Vector{m, 0} - et::Vector{0, m} == et::Vector{m, 0});
    REQUIRE(et::Vector{0, m} - et::Vector{m, 0} == et::Vector{0, m});
}

TEST_CASE("Vector addition") {
    REQUIRE(et::Vector{0, 0} + et::Vector{1, 2} == et::Vector{1, 2});
    REQUIRE(et::Vector{10, 5} + et::Vector{1, 7} == et::Vector{11, 12});
    REQUIRE(et::Vector{12, 18} + et::Vector{20, 18} == et::Vector{32, 36});
    REQUIRE(et::Vector{2, 13} + et::Vector{3, 11} == et::Vector{5, 24});
}

TEST_CASE("Window getters") {

    constexpr et::Vector origin{10, 1};
    constexpr et::Vector size{3, 4};

    et::Window win{origin, size};
    REQUIRE(win.size() == size);
    REQUIRE(win.origin() == origin);
    REQUIRE(win.end() == (origin + size));
}

TEST_CASE("Window inside test") {

    constexpr et::Vector origin{10, 16};

    SECTION("Empty window") {
        constexpr et::Vector size{0, 0};
        et::Window win{origin, size};

        REQUIRE(!win.is_inside(origin));
    }
    SECTION("Zero-extent row window") {
        constexpr et::Vector size{0, 13};
        et::Window win{origin, size};

        REQUIRE(!win.is_inside(origin));
        REQUIRE(!win.is_inside(origin + et::Vector{0, 1}));
        REQUIRE(!win.is_inside(origin + et::Vector{1, 0}));
        REQUIRE(!win.is_inside(origin + et::Vector{0, 3}));
        REQUIRE(!win.is_inside(origin + et::Vector{0, size.m_col - 1}));
        REQUIRE(!win.is_inside(origin + et::Vector{0, size.m_col}));
    }
    SECTION("Zero-extent col window") {
        constexpr et::Vector size{11, 0};
        et::Window win{origin, size};

        REQUIRE(!win.is_inside(origin));
        REQUIRE(!win.is_inside(origin + et::Vector{1, 0}));
        REQUIRE(!win.is_inside(origin + et::Vector{0, 1}));
        REQUIRE(!win.is_inside(origin + et::Vector{3, 0}));
        REQUIRE(!win.is_inside(origin + et::Vector{size.m_row - 1, 0}));
        REQUIRE(!win.is_inside(origin + et::Vector{size.m_row, 0}));
    }
    SECTION("Ordinary window") {
        constexpr et::Vector size{3, 5};
        et::Window win{origin, size};

        for (std::size_t r = 0; r < size.m_row; ++r)
            for (std::size_t c = 0; c < size.m_col; ++c) {

                REQUIRE(win.is_inside(origin + et::Vector{r, c}));
            }

        REQUIRE(win.is_inside(origin));
        REQUIRE(!win.is_inside(origin + et::Vector{0, size.m_col}));
        REQUIRE(!win.is_inside(origin + et::Vector{size.m_row, 0}));
        REQUIRE(!win.is_inside(origin + size));
    }
}
TEST_CASE("Subwindow construction") {
    constexpr et::Vector origin{10, 16};
    constexpr et::Vector size{5, 3};

    et::Window win{origin, size};

    SECTION("Full window") {
        auto sub = win.sub_win({0, 0}, size);
        REQUIRE(sub == win);
    }
    SECTION("Larger subwindow is clamped") {
        auto sub = win.sub_win({0, 0}, size + et::Vector{11, 12});
        // Still the same window
        REQUIRE(sub == win);
    }
    SECTION("Larger subwidnow is clamped (with offset)") {
        const et::Vector sub_origin{1, 1};
        const et::Vector exp_sub_size{size - sub_origin};

        auto sub = win.sub_win(sub_origin, size); // Larger size
        REQUIRE(sub.size() == exp_sub_size);
        REQUIRE(sub.origin() == (sub_origin + origin));
    }
    SECTION("Partial window") {
        const et::Vector sub_size{size - et::Vector{2, 1}};

        auto sub = win.sub_win({0, 0}, sub_size);
        REQUIRE(sub.size() == sub_size);
    }
    SECTION("Offset origin max subwindow") {
        const et::Vector sub_origin{1, 1};
        const et::Vector sub_size{size - sub_origin};

        auto sub = win.sub_win(sub_origin, sub_size);
        REQUIRE(sub.size() == sub_size);
        REQUIRE(sub.origin() == (sub_origin + origin));
    }
}
