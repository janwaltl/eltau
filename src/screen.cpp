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
Window::write(const TerminalCell& cell) {
    assert(m_screen);
    m_screen->write(cell);
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

Coords2
Screen::write(const TerminalCell& cell) {
    return do_write(cell);
}

Coords2
Screen::move_cursor(const Coords2& dest) noexcept {
    // m_cursor set by the call.
    return m_cursor = do_move_cursor(dest);
}

Coords2
Screen::advance_cursor(const Offset2& offset) noexcept {
    return m_cursor = do_advance_cursor(offset);
}

Coords2
Screen::do_move_cursor(const Coords2& dest) noexcept {
    return do_advance_cursor(dest - cursor());
}
} // namespace eltau
