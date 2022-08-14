/*******************************************************************************
 * @file element.cpp
 * @copyright Copyright 2022 Jan Waltl.
 * @license	This file is released under ElPhi project's license, see LICENSE.
 ******************************************************************************/
#include <eltau/screen.hpp>

namespace eltau {
Screen::Screen(Vector dimensions) : m_dims{dimensions}, m_buffer{m_dims.m_col * m_dims.m_row, Cell{}} {

    for (auto& c : m_buffer) {
        c.m_char[0] = '+';
        c.m_char[1] = 0;
    }
}

Vector
Screen::dims() const noexcept {
    return m_dims;
}

Screen::Line
Screen::line(std::size_t idx) noexcept {
    idx = idx * m_dims.m_col;
    if (idx >= m_buffer.size())
        return {};
    auto* begin = m_buffer.data() + idx;
    return {begin, begin + m_dims.m_col};
}

Screen::cLine
Screen::line(std::size_t idx) const noexcept {
    idx = idx * m_dims.m_col;
    if (idx >= m_buffer.size())
        return {};
    const auto* begin = m_buffer.data() + idx;
    return {begin, begin + m_dims.m_col};
}

const Cell*
Screen::operator[](Vector coords) const noexcept {
    auto idx = coords.m_row * (coords.m_col + 1);

    return idx < m_buffer.size() ? &m_buffer[idx] : nullptr;
}

Cell*
Screen::operator[](Vector coords) noexcept {
    auto idx = coords.m_row * (coords.m_col + 1);

    return idx < m_buffer.size() ? &m_buffer[idx] : nullptr;
}

Vector
operator+(const Vector& l, const Vector& r) {
    return {.m_row = l.m_row + r.m_row, .m_col = l.m_col + r.m_col};
}

Vector
operator-(const Vector& l, const Vector& r) {
    Vector res{.m_row = l.m_row - r.m_row, .m_col = l.m_col - r.m_col};

    // Saturating
    return {.m_row = res.m_row > l.m_row ? 0 : res.m_row, .m_col = res.m_col > l.m_col ? 0 : res.m_col};
}

Vector
min(const Vector& l, const Vector& r) noexcept {
    return {std::min(l.m_row, r.m_row), std::min(l.m_col, r.m_col)};
}

Vector
max(const Vector& l, const Vector& r) noexcept {
    return {std::max(l.m_row, r.m_row), std::max(l.m_col, r.m_col)};
}

Window::Window(Vector begin, Vector size) noexcept : m_begin(begin), m_size(size) {}

bool
Window::is_inside(Vector pos) const noexcept {
    auto end = m_begin + m_size;
    return pos.m_row >= m_begin.m_row && pos.m_row < end.m_row && pos.m_col >= m_begin.m_col && pos.m_col < end.m_col;
}

Vector
Window::size() const noexcept {
    return m_size;
}

Vector
Window::origin() const noexcept {
    return m_begin;
}

Vector
Window::end() const noexcept {
    return m_begin + m_size;
}

Window
Window::sub_win(Vector offset, Vector size) {
    auto origin = m_begin + offset;
    auto max_size = m_size - offset;
    return {origin, min(max_size, size)};
}

DrawingWindow::DrawingWindow(const Window& win, Screen& screen) noexcept : Window{win}, m_screen{&screen} {}

DrawingWindow
DrawingWindow::sub_win(Vector offset, Vector size) {
    return DrawingWindow{Window::sub_win(offset, size), *m_screen};
}

Cell*
DrawingWindow::operator[](Vector coords) noexcept {
    if (this->is_inside(coords))
        return (*m_screen)[coords + this->origin()];
    return nullptr;
}

const Cell*
DrawingWindow::operator[](Vector coords) const noexcept {
    if (this->is_inside(coords))
        return (*m_screen)[coords + this->origin()];
    return nullptr;
}
} // namespace eltau
