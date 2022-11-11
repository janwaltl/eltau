/*******************************************************************************
 * @file element.cpp
 * @copyright Copyright 2022 Jan Waltl.
 * @license	This file is released under ElTau project's license, see LICENSE.
 ******************************************************************************/
#include <cassert>

#include <eltau/screen.hpp>

namespace eltau {
Vec2&
Vec2::operator+=(const Vec2& other) noexcept {
    *this = *this + other;
    return *this;
}

Vec2&
Vec2::operator-=(const Vec2& other) noexcept {
    *this = *this - other;
    return *this;
}

Vec2
operator+(const Vec2& l, const Vec2& r) noexcept {
    return {.m_row = l.m_row + r.m_row, .m_col = l.m_col + r.m_col};
}

Vec2
operator-(const Vec2& l, const Vec2& r) noexcept {
    Vec2 res{.m_row = l.m_row - r.m_row, .m_col = l.m_col - r.m_col};

    // Saturating
    return {.m_row = res.m_row > l.m_row ? 0 : res.m_row, .m_col = res.m_col > l.m_col ? 0 : res.m_col};
}

Vec2
min(const Vec2& l, const Vec2& r) noexcept {
    return {std::min(l.m_row, r.m_row), std::min(l.m_col, r.m_col)};
}

Vec2
max(const Vec2& l, const Vec2& r) noexcept {
    return {std::max(l.m_row, r.m_row), std::max(l.m_col, r.m_col)};
}

Vec2
clamp(const Vec2& vec, const Vec2& low, const Vec2& high) noexcept {
    assert(low.m_row <= high.m_row && low.m_col <= high.m_col);
    return min(high, max(low, vec));
}

std::ostream&
operator<<(std::ostream& os, const Vec2& value) {
    os << "{r=" << value.m_row << ", c=" << value.m_col << "}";
    return os;
}


Window::Window(Vec2 begin, Vec2 size, Screen& screen) noexcept : m_origin(begin), m_size(size), m_screen{&screen} {}

Vec2
Window::size() const noexcept {
    return m_size;
}

Vec2
Window::origin() const noexcept {
    return m_origin;
}

Vec2
Window::end() const noexcept {
    return m_origin + m_size;
}

Vec2
Window::cursor() const noexcept {
    return m_screen->cursor() - m_origin;
}

bool
Window::is_inside(Vec2 pos) const noexcept {
    auto end = m_origin + m_size;
    return pos.m_row >= m_origin.m_row && pos.m_row < end.m_row && pos.m_col >= m_origin.m_col && pos.m_col < end.m_col;
}

Window
Window::sub_win(Vec2 offset, Vec2 size) const {
    auto origin = m_origin + offset;
    auto max_size = m_size - offset;
    return {origin, min(max_size, size), *m_screen};
}

void
Window::write(const TerminalCell& cell, Vec2 advance) {
    assert(m_screen);
    m_screen->write(cell, advance);
}

void
Window::move_nextline() noexcept {
    assert(m_screen);

    Vec2 dest{.m_row = m_screen->cursor().m_row + 1, .m_col = m_origin.m_col};
    dest = clamp(dest, m_origin, m_origin + m_size);

    m_screen->move_cursor(dest);
}

Screen::Screen(Vec2 size) noexcept : m_size{size} {}

Vec2
Screen::cursor() const noexcept {
    return m_cursor;
}

Vec2
Screen::size() const noexcept {
    return m_size;
}

void
Screen::write(const TerminalCell& cell, Vec2 advance) {
    // TODO (jw) Implement me.
    (void)cell;

    // Cursor is moved on itself by writing to the screen cell.
    if (advance != Vec2{.m_row = 0, .m_col = 1})
        advance_cursor(advance - Vec2{.m_row = 0, .m_col = 1});
}

void
Screen::move_cursor(const Vec2& dest) noexcept {
    m_cursor = min(dest, m_size);
    // TODO (jw) Actually move cursor.
}

void
Screen::advance_cursor(const Vec2& offset) noexcept {
    auto dest = m_cursor + offset;
    move_cursor(dest);
}
} // namespace eltau
