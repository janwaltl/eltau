/*******************************************************************************
 * @file element.cpp
 * @copyright Copyright 2022 Jan Waltl.
 * @license	This file is released under ElTau project's license, see LICENSE.
 ******************************************************************************/
#include <cassert>

#include <eltau/screen.hpp>

namespace eltau {

Window::Window(Coords2 begin, Size2 size, Screen& screen) noexcept : m_origin(begin), m_size(size), m_screen{&screen} {}

Size2
Window::size() const noexcept {
    return m_size;
}

Coords2
Window::origin() const noexcept {
    return m_origin;
}

Coords2
Window::end() const noexcept {
    return m_origin + m_size;
}

Coords2
Window::cursor() const noexcept {
    return m_screen->cursor();
}

Offset2
Window::cursor_rel() const noexcept {
    return m_screen->cursor() - m_origin;
}

bool
Window::is_inside(Coords2 pos) const noexcept {
    auto end = this->end();
    return pos.m_row >= m_origin.m_row && pos.m_row < end.m_row && pos.m_col >= m_origin.m_col && pos.m_col < end.m_col;
}

Window
Window::sub_win(Size2 offset, Size2 size) const {
    assert(m_screen);
    auto origin = m_origin + offset;
    auto max_size = m_size - offset;
    return {origin, min(max_size, size), *m_screen};
}

void
Window::write(const TerminalCell& cell, Offset2 advance) {
    assert(m_screen);
    m_screen->write(cell, advance);
}

void
Window::move_nextline() noexcept {
    assert(m_screen);

    Coords2 dest{.m_row = m_screen->cursor().m_row + 1, .m_col = m_origin.m_col};
    dest = clamp(dest, m_origin, m_origin + m_size);

    m_screen->move_cursor(dest);
}

Screen::Screen(Size2 size) noexcept : m_size{size} {}

Coords2
Screen::cursor() const noexcept {
    return m_cursor;
}

Size2
Screen::size() const noexcept {
    return m_size;
}

void
Screen::write(const TerminalCell& cell, Offset2 advance) {
    // TODO (jw) Implement me.
    (void)cell;

    // Cursor is moved on itself by writing to the screen cell.
    if (advance != Offset2{.m_row = 0, .m_col = 1})
        advance_cursor(advance - Offset2{.m_row = 0, .m_col = 1});
}

void
Screen::move_cursor(const Coords2& dest) noexcept {
    m_cursor = min(dest, Coords2{0, 0} + m_size);
    // TODO (jw) Actually move cursor.
}

void
Screen::advance_cursor(const Offset2& offset) noexcept {
    move_cursor(m_cursor + offset);
}
} // namespace eltau
