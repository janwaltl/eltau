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
namespace {
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
} // namespace
TEST_CASE("Window getters") {

    constexpr et::Coords2 origin{10, 1};
    constexpr et::Size2 size{30, 40};

    DummyScreen screen{size};

    et::Window win{origin, size, screen};
    REQUIRE(win.size() == size);
    REQUIRE(win.origin() == origin);
    REQUIRE(win.end() == (origin + size));
}

TEST_CASE("Window cursor") {
    constexpr et::Coords2 origin{10, 1};
    constexpr et::Size2 size{30, 40};

    DummyScreen screen{size};
    et::Window win{origin, {5, 6}, screen};

    auto cursor_pos{
        GENERATE(et::Coords2{0, 0}, et::Coords2{1, 2}, et::Coords2{20, 1}, et::Coords2{20, 0}, et::Coords2{11, 20})};
    SECTION("Getter returns true cursor position.") {
        screen.move_cursor(cursor_pos);
        REQUIRE(win.cursor() == cursor_pos);
    }
    SECTION("Getter returns true relative cursor position.") {
        screen.move_cursor(cursor_pos);
        REQUIRE((win.origin() + win.cursor_rel()) == cursor_pos);
    }
}

TEST_CASE("Window inside test") {

    constexpr et::Coords2 origin{10, 16};
    DummyScreen screen{{100, 100}};

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
    DummyScreen screen{size};

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
        DummyScreen s{{0, 0}};

        REQUIRE(s.size() == et::Size2{0, 0});
    }

    SECTION("Sizes") {
        auto size{GENERATE(et::Size2{0, 0}, et::Size2{1, 2}, et::Size2{53, 1}, et::Size2{2, 0}, et::Size2{0, 3})};
        DummyScreen s{size};

        REQUIRE(s.size() == size);
    }
}

TEST_CASE("Cursor starts at {0,0}") {
    auto size{GENERATE(et::Size2{0, 0}, et::Size2{1, 2}, et::Size2{53, 1}, et::Size2{2, 0}, et::Size2{0, 3})};
    DummyScreen s{size};

    REQUIRE(s.cursor() == et::Coords2{0, 0});
}

TEST_CASE("Cursor movement getter") {
    et::Size2 size{100, 100};
    DummyScreen s{size};
    auto dest{GENERATE(et::Coords2{0, 0}, et::Coords2{1, 2}, et::Coords2{53, 1}, et::Coords2{2, 0}, et::Coords2{0, 3})};

    s.move_cursor(dest);
    REQUIRE(s.cursor() == dest);
}

TEST_CASE("Cursor advance getter") {
    et::Size2 size{100, 100};
    DummyScreen s{size};

    et::Offset2 adv{};
    et::Coords2 exp_pos{};

    REQUIRE(s.cursor() == et::Coords2{0, 0});
    adv = {5, 0};
    s.advance_cursor(adv);
    exp_pos += adv;
    REQUIRE(s.cursor() == exp_pos);

    adv = {-2, 0};
    s.advance_cursor(adv);
    exp_pos += adv;
    REQUIRE(s.cursor() == exp_pos);

    adv = {0, 5};
    s.advance_cursor(adv);
    exp_pos += adv;
    REQUIRE(s.cursor() == exp_pos);

    adv = {0, -4};
    s.advance_cursor(adv);
    exp_pos += adv;
    REQUIRE(s.cursor() == exp_pos);
}
