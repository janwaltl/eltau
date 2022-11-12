/*******************************************************************************
 * @file coords.cpp
 * @copyright Copyright 2022 Jan Waltl.
 * @license	This file is released under ElTau project's license, see LICENSE.
 ******************************************************************************/
#include <cassert>
#include <limits>

#include <eltau/coords.hpp>

namespace eltau {

namespace {

/*******************************************************************************
 * @brief Saturating addition `x+y`.
 *
 * Clamps underflow to 0, asserts overflow since that cannot be handle.
 *
 * @param x Unsigned quantity.
 * @param y Signed quantity to add.
 * @return `x+y`.
 ******************************************************************************/
Coords2::coords_type
sat_add(Coords2::coords_type x, Offset2::offset_type y) {
    // Protect against overflow.
    assert(y < 0 || x < std::numeric_limits<decltype(x)>::max() - y);

    Coords2::coords_type res{x + y};

    return y < 0 && res > x ? 0 : res;
}
} // namespace

Coords2&
Coords2::operator+=(const Offset2& other) noexcept {
    // Protect against overflow.
    assert(other.m_row < 0 || this->m_row < std::numeric_limits<decltype(this->m_row)>::max() - other.m_row);
    assert(other.m_col < 0 || this->m_col < std::numeric_limits<decltype(this->m_col)>::max() - other.m_col);

    this->m_row = sat_add(this->m_row, other.m_row);
    this->m_col = sat_add(this->m_col, other.m_col);
    return *this;
}

Coords2&
Coords2::operator+=(const Size2& other) noexcept {
    this->m_row += other.m_row;
    this->m_col += other.m_col;
    return *this;
}

Coords2&
Coords2::operator-=(const Offset2& other) noexcept {
    return *this += -other;
}

Coords2::operator Offset2() const noexcept {
    // Protect against overflow.
    assert(m_row <= std::numeric_limits<Offset2::offset_type>::max());
    assert(m_col <= std::numeric_limits<Offset2::offset_type>::max());

    return {.m_row = static_cast<Offset2::offset_type>(this->m_row),
            .m_col = static_cast<Offset2::offset_type>(this->m_col)};
}

Coords2::operator Size2() const noexcept {
    // Protect against overflow.
    static_assert(sizeof(Coords2::coords_type) <= 2 * sizeof(Size2::size_type));

    return {.m_row = static_cast<Size2::size_type>(this->m_row), .m_col = static_cast<Size2::size_type>(this->m_col)};
}
Offset2&
Offset2::operator+=(const Offset2& other) noexcept {
    // Protect against overflow.
    assert(other.m_row < 0 || this->m_row < std::numeric_limits<decltype(this->m_row)>::max() - other.m_row);
    assert(other.m_row > 0 || this->m_row > std::numeric_limits<decltype(this->m_row)>::min() - other.m_row);
    assert(other.m_col < 0 || this->m_col < std::numeric_limits<decltype(this->m_col)>::max() - other.m_col);
    assert(other.m_col > 0 || this->m_col > std::numeric_limits<decltype(this->m_col)>::min() - other.m_col);

    this->m_row += other.m_row;
    this->m_col += other.m_col;
    return *this;
}

Offset2&
Offset2::operator-=(const Offset2& other) noexcept {
    return *this += -other;
}

Size2&
Size2::operator+=(const Size2& other) noexcept {
    // Protect against overflow.
    assert(this->m_row < std::numeric_limits<decltype(this->m_row)>::max() - other.m_row);
    assert(this->m_col < std::numeric_limits<decltype(this->m_col)>::max() - other.m_col);

    this->m_row += other.m_row;
    this->m_col += other.m_col;
    return *this;
}

Size2::operator Offset2() const noexcept {
    // Protect against overflow.
    assert(m_row <= std::numeric_limits<Offset2::offset_type>::max());
    assert(m_col <= std::numeric_limits<Offset2::offset_type>::max());

    return {.m_row = static_cast<Offset2::offset_type>(this->m_row),
            .m_col = static_cast<Offset2::offset_type>(this->m_col)};
}

Coords2
min(const Coords2& l, const Coords2& r) noexcept {
    return {std::min(l.m_row, r.m_row), std::min(l.m_col, r.m_col)};
}

Offset2
min(const Offset2& l, const Offset2& r) noexcept {
    return {std::min(l.m_row, r.m_row), std::min(l.m_col, r.m_col)};
}

Size2
min(const Size2& l, const Size2& r) noexcept {
    return {std::min(l.m_row, r.m_row), std::min(l.m_col, r.m_col)};
}

Coords2
max(const Coords2& l, const Coords2& r) noexcept {
    return {std::max(l.m_row, r.m_row), std::max(l.m_col, r.m_col)};
}

Offset2
max(const Offset2& l, const Offset2& r) noexcept {
    return {std::max(l.m_row, r.m_row), std::max(l.m_col, r.m_col)};
}

Size2
max(const Size2& l, const Size2& r) noexcept {
    return {std::max(l.m_row, r.m_row), std::max(l.m_col, r.m_col)};
}

Coords2
clamp(const Coords2& vec, const Coords2& low, const Coords2& high) noexcept {
    assert(low.m_row <= high.m_row && low.m_col <= high.m_col);
    return min(high, max(low, vec));
}

Offset2
clamp(const Offset2& vec, const Offset2& low, const Offset2& high) noexcept {
    assert(low.m_row <= high.m_row && low.m_col <= high.m_col);
    return min(high, max(low, vec));
}

Size2
clamp(const Size2& vec, const Size2& low, const Size2& high) noexcept {
    assert(low.m_row <= high.m_row && low.m_col <= high.m_col);
    return min(high, max(low, vec));
}

Coords2
operator+(const Coords2& l, const Offset2& r) noexcept {
    auto res{l};
    return res += r;
}

Offset2
operator+(const Offset2& l, const Offset2& r) noexcept {
    auto res{l};
    return res += r;
}

Coords2
operator-(const Coords2& l, const Offset2& r) noexcept {
    auto res{l};
    return res -= r;
}

Coords2
operator+(const Coords2& l, const Size2& r) noexcept {
    auto res{l};
    return res += r;
}

Size2
operator+(const Size2& l, const Size2& r) noexcept {
    auto res{l};
    return res += r;
}

Offset2
operator-(const Coords2& l, const Coords2& r) noexcept {
    // TODO (jw) Improve range where the difference is representable
    // but individual coords as offset are not.
    return static_cast<Offset2>(l) - static_cast<Offset2>(r);
}

Size2
operator-(const Size2& l, const Size2& r) noexcept {
    Size2 res{.m_row = l.m_row - r.m_row, .m_col = l.m_col - r.m_col};
    res = {.m_row = res.m_row > l.m_row ? 0 : res.m_row, .m_col = res.m_col > l.m_col ? 0 : res.m_col};

    return res;
}

Offset2
operator-(const Offset2& l, const Offset2& r) noexcept {
    auto res{l};
    return res -= r;
}

Offset2
operator-(const Offset2& offset) noexcept {
    return {.m_row = -offset.m_row, .m_col = -offset.m_col};
}

std::ostream&
operator<<(std::ostream& os, const Coords2& value) {
    os << "{r=" << value.m_row << ", c=" << value.m_col << "}";
    return os;
}

std::ostream&
operator<<(std::ostream& os, const Size2& value) {
    os << std::showpos << "{r=" << value.m_row << ", c=" << value.m_col << "}";
    return os;
}

std::ostream&
operator<<(std::ostream& os, const Offset2& value) {
    os << std::showpos << "{r=" << value.m_row << ", c=" << value.m_col << "}";
    return os;
}

} // namespace eltau
