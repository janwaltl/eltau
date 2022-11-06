/*******************************************************************************
 * @file element.cpp
 * @copyright Copyright 2022 Jan Waltl.
 * @license	This file is released under ElTau project's license, see LICENSE.
 ******************************************************************************/
#include <eltau/screen.hpp>

namespace eltau {
Screen::Screen(Vec2 size) : m_size{size}, m_buffer{m_size.m_col * m_size.m_row, Cell{}} {

    for (auto& c : m_buffer) {
        c.m_char[0] = '+';
        c.m_char[1] = 0;
    }
}

Vec2
Screen::size() const noexcept {
    return m_size;
}

Screen::Line
Screen::line(std::size_t idx) noexcept {
    auto range = std::as_const(*this).line(idx);

    return {
        const_cast<Cell*>(range.data()),
        const_cast<Cell*>(range.data() + range.size()),
    };
}

Screen::cLine
Screen::line(std::size_t idx) const noexcept {
    idx = idx * m_size.m_col;
    if (idx >= m_buffer.size())
        return {};
    const auto* begin = m_buffer.data() + idx;
    return {begin, begin + m_size.m_col};
}

const Cell*
Screen::operator[](Vec2 coords) const noexcept {
    if (!Window{{0, 0}, m_size}.is_inside(coords))
        return nullptr;

    auto idx = (coords.m_row + 1) * coords.m_col;
    return &m_buffer[idx];
}

Cell*
Screen::operator[](Vec2 coords) noexcept {
    return const_cast<Cell*>(std::as_const(*this)[coords]);
}

Vec2
operator+(const Vec2& l, const Vec2& r) {
    return {.m_row = l.m_row + r.m_row, .m_col = l.m_col + r.m_col};
}

Vec2
operator-(const Vec2& l, const Vec2& r) {
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

Window::Window(Vec2 begin, Vec2 size) noexcept : m_origin(begin), m_size(size) {}

bool
Window::is_inside(Vec2 pos) const noexcept {
    auto end = m_origin + m_size;
    return pos.m_row >= m_origin.m_row && pos.m_row < end.m_row && pos.m_col >= m_origin.m_col && pos.m_col < end.m_col;
}

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

Window
Window::sub_win(Vec2 offset, Vec2 size) {
    auto origin = m_origin + offset;
    auto max_size = m_size - offset;
    return {origin, min(max_size, size)};
}

DrawingWindow::DrawingWindow(const Window& win, Screen& screen) noexcept : Window{win}, m_screen{&screen} {}

DrawingWindow
DrawingWindow::sub_win(Vec2 offset, Vec2 size) {
    return DrawingWindow{Window::sub_win(offset, size), *m_screen};
}

Cell*
DrawingWindow::operator[](Vec2 coords) noexcept {
    if (this->is_inside(coords))
        return (*m_screen)[coords];
    return nullptr;
}

const Cell*
DrawingWindow::operator[](Vec2 coords) const noexcept {
    if (this->is_inside(coords))
        return (*m_screen)[coords];
    return nullptr;
}
} // namespace eltau
