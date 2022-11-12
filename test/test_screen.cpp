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
TEST_CASE("Vector min") {
    REQUIRE(min(et::Vec2{0, 0}, et::Vec2{1, 2}) == et::Vec2{0, 0});
    REQUIRE(min(et::Vec2{5, 1}, et::Vec2{1, 5}) == et::Vec2{1, 1});
    REQUIRE(min(et::Vec2{5, 7}, et::Vec2{7, 9}) == et::Vec2{5, 7});
    REQUIRE(min(et::Vec2{5, 7}, et::Vec2{3, 1}) == et::Vec2{3, 1});
}

TEST_CASE("Vector max") {
    REQUIRE(max(et::Vec2{0, 0}, et::Vec2{1, 2}) == et::Vec2{1, 2});
    REQUIRE(max(et::Vec2{5, 1}, et::Vec2{1, 5}) == et::Vec2{5, 5});
    REQUIRE(max(et::Vec2{5, 7}, et::Vec2{7, 9}) == et::Vec2{7, 9});
    REQUIRE(max(et::Vec2{5, 7}, et::Vec2{3, 1}) == et::Vec2{5, 7});
}

TEST_CASE("Vector clamp") {
    REQUIRE(clamp(et::Vec2{5, 4}, et::Vec2{0, 0}, et::Vec2{6, 6}) == et::Vec2{5, 4});
    REQUIRE(clamp(et::Vec2{5, 4}, et::Vec2{6, 0}, et::Vec2{8, 3}) == et::Vec2{6, 3});
    REQUIRE(clamp(et::Vec2{5, 4}, et::Vec2{0, 5}, et::Vec2{2, 8}) == et::Vec2{2, 5});
}

TEST_CASE("Vector subtraction") {
    constexpr auto m = std::numeric_limits<std::size_t>::max();
    REQUIRE(et::Vec2{0, 0} - et::Vec2{1, 2} == et::Vec2{0, 0});
    REQUIRE(et::Vec2{10, 5} - et::Vec2{1, 7} == et::Vec2{9, 0});
    REQUIRE(et::Vec2{12, 18} - et::Vec2{20, 18} == et::Vec2{0, 0});
    REQUIRE(et::Vec2{2, 13} - et::Vec2{3, 11} == et::Vec2{0, 2});
    REQUIRE(et::Vec2{m, 0} - et::Vec2{0, m} == et::Vec2{m, 0});
    REQUIRE(et::Vec2{0, m} - et::Vec2{m, 0} == et::Vec2{0, m});
}

TEST_CASE("Vector member subtraction") {
    constexpr auto m = std::numeric_limits<std::size_t>::max();
    REQUIRE((et::Vec2{0, 0} -= et::Vec2{1, 2}) == et::Vec2{0, 0});
    REQUIRE((et::Vec2{10, 5} -= et::Vec2{1, 7}) == et::Vec2{9, 0});
    REQUIRE((et::Vec2{12, 18} -= et::Vec2{20, 18}) == et::Vec2{0, 0});
    REQUIRE((et::Vec2{2, 13} -= et::Vec2{3, 11}) == et::Vec2{0, 2});
    REQUIRE((et::Vec2{m, 0} -= et::Vec2{0, m}) == et::Vec2{m, 0});
    REQUIRE((et::Vec2{0, m} -= et::Vec2{m, 0}) == et::Vec2{0, m});
}

TEST_CASE("Vector addition") {
    REQUIRE(et::Vec2{0, 0} + et::Vec2{1, 2} == et::Vec2{1, 2});
    REQUIRE(et::Vec2{10, 5} + et::Vec2{1, 7} == et::Vec2{11, 12});
    REQUIRE(et::Vec2{12, 18} + et::Vec2{20, 18} == et::Vec2{32, 36});
    REQUIRE(et::Vec2{2, 13} + et::Vec2{3, 11} == et::Vec2{5, 24});
}

TEST_CASE("Vector member addition") {
    REQUIRE((et::Vec2{0, 0} += et::Vec2{1, 2}) == et::Vec2{1, 2});
    REQUIRE((et::Vec2{10, 5} += et::Vec2{1, 7}) == et::Vec2{11, 12});
    REQUIRE((et::Vec2{12, 18} += et::Vec2{20, 18}) == et::Vec2{32, 36});
    REQUIRE((et::Vec2{2, 13} += et::Vec2{3, 11}) == et::Vec2{5, 24});
}

TEST_CASE("Vector pretty-print") {
    std::stringstream s;
    s << et::Vec2{15, 9};
    REQUIRE(s.str() == "{r=15, c=9}");
}


TEST_CASE("Window getters") {

    constexpr et::Vec2 origin{10, 1};
    constexpr et::Vec2 size{3, 4};

    et::Screen screen{size};

    et::Window win{origin, size, screen};
    REQUIRE(win.size() == size);
    REQUIRE(win.origin() == origin);
    REQUIRE(win.end() == (origin + size));
}

TEST_CASE("Window inside test") {

    constexpr et::Vec2 origin{10, 16};
    et::Screen screen{{100, 100}};

    SECTION("Empty window") {
        constexpr et::Vec2 size{0, 0};
        et::Window win{origin, size, screen};

        REQUIRE(!win.is_inside(origin));
    }
    SECTION("Zero-extent row window") {
        constexpr et::Vec2 size{0, 13};
        et::Window win{origin, size, screen};

        REQUIRE(!win.is_inside(origin));
        REQUIRE(!win.is_inside(origin + et::Vec2{0, 1}));
        REQUIRE(!win.is_inside(origin + et::Vec2{1, 0}));
        REQUIRE(!win.is_inside(origin + et::Vec2{0, 3}));
        REQUIRE(!win.is_inside(origin + et::Vec2{0, size.m_col - 1}));
        REQUIRE(!win.is_inside(origin + et::Vec2{0, size.m_col}));
    }
    SECTION("Zero-extent col window") {
        constexpr et::Vec2 size{11, 0};
        et::Window win{origin, size, screen};

        REQUIRE(!win.is_inside(origin));
        REQUIRE(!win.is_inside(origin + et::Vec2{1, 0}));
        REQUIRE(!win.is_inside(origin + et::Vec2{0, 1}));
        REQUIRE(!win.is_inside(origin + et::Vec2{3, 0}));
        REQUIRE(!win.is_inside(origin + et::Vec2{size.m_row - 1, 0}));
        REQUIRE(!win.is_inside(origin + et::Vec2{size.m_row, 0}));
    }
    SECTION("Ordinary window") {
        constexpr et::Vec2 size{3, 5};
        et::Window win{origin, size, screen};

        for (std::size_t r = 0; r < size.m_row; ++r)
            for (std::size_t c = 0; c < size.m_col; ++c) {

                REQUIRE(win.is_inside(origin + et::Vec2{r, c}));
            }

        REQUIRE(win.is_inside(origin));
        REQUIRE(!win.is_inside(origin + et::Vec2{0, size.m_col}));
        REQUIRE(!win.is_inside(origin + et::Vec2{size.m_row, 0}));
        REQUIRE(!win.is_inside(origin + size));
    }
}
TEST_CASE("Subwindow construction") {
    constexpr et::Vec2 origin{10, 16};
    constexpr et::Vec2 size{5, 3};
    et::Screen screen{size};

    et::Window win{origin, size, screen};

    SECTION("Full window") {
        auto sub = win.sub_win({0, 0}, size);
        REQUIRE(sub == win);
    }
    SECTION("Larger subwindow is clamped") {
        auto sub = win.sub_win({0, 0}, size + et::Vec2{11, 12});
        // Still the same window
        REQUIRE(sub == win);
    }
    SECTION("Larger subwidnow is clamped (with offset)") {
        const et::Vec2 sub_origin{1, 1};
        const et::Vec2 exp_sub_size{size - sub_origin};

        auto sub = win.sub_win(sub_origin, size); // Larger size
        REQUIRE(sub.size() == exp_sub_size);
        REQUIRE(sub.origin() == (sub_origin + origin));
    }
    SECTION("Partial window") {
        const et::Vec2 sub_size{size - et::Vec2{2, 1}};

        auto sub = win.sub_win({0, 0}, sub_size);
        REQUIRE(sub.size() == sub_size);
    }
    SECTION("Offset origin max subwindow") {
        const et::Vec2 sub_origin{1, 1};
        const et::Vec2 sub_size{size - sub_origin};

        auto sub = win.sub_win(sub_origin, sub_size);
        REQUIRE(sub.size() == sub_size);
        REQUIRE(sub.origin() == (sub_origin + origin));
    }
}
TEST_CASE("Screen size") {
    SECTION("Empty screen") {
        et::Screen s{et::Vec2{0, 0}};

        REQUIRE(s.size() == et::Vec2{0, 0});
    }

    SECTION("Sizes") {
        auto size{GENERATE(et::Vec2{0, 0}, et::Vec2{1, 2}, et::Vec2{53, 1}, et::Vec2{2, 0}, et::Vec2{0, 3})};
        et::Screen s{size};

        REQUIRE(s.size() == size);
    }
}
