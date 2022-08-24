/*******************************************************************************
 * @file element.cpp
 * @copyright Copyright 2022 Jan Waltl.
 * @license	This file is released under ElTau project's license, see LICENSE.
 ******************************************************************************/
#include <cassert>

#include <eltau/element.hpp>

namespace eltau {

Vector
Element::calc_pref_size(Vector max_size) {
    // Only calc size for feasible bounds.
    if (max_size.m_col == 0 || max_size.m_row == 0)
        return m_last_pref_size = {0, 0};
    return m_last_pref_size = this->do_calc_pref_size(max_size);
}

void
Element::draw(DrawingWindow& window) {

    return this->do_draw(window);
}

Vector
Element::get_last_pref_size() const noexcept {
    return m_last_pref_size;
}

Text::Text(std::string_view text, std::size_t wrap_limit) : m_text(text), m_wrap_limit(wrap_limit) {}

Vector
Text::do_calc_pref_size(Vector max_size) {
    assert(max_size.m_row > 0 && max_size.m_col > 0);

    // TODO(jw) calculate unicode width properly, right now assume ASCII.
    const auto len = m_text.size();
    const auto usable_cols = m_wrap_limit == c_no_wrap ? max_size.m_col : std::min(max_size.m_col, m_wrap_limit);

    // Rounds rows up.
    const auto pref_rows{len / usable_cols + (len % usable_cols ? 1 : 0)};

    return Vector{.m_row = std::min(max_size.m_row, pref_rows), .m_col = std::min(len, usable_cols)};
}

void
Text::do_draw(DrawingWindow& window) {
    Vector cursor{0, 0};
    const Vector size = window.size();

    // TODO(jw) iterate over graphemes instead.
    for (auto c : m_text) {
        if (!window.is_inside(cursor))
            break;
        auto* cell = window[window.origin() + cursor];
        // TODO(jw) ignore non-printable ASCII characters.
        if (c == '\n') {
            ++cursor.m_row;
            cursor.m_col = 0;
        } else {
            cell->m_char[0] = c;
            cell->m_fg.m_value = 0; // Black
            cell->m_bg.m_value = 6; // Cyan
            cursor.m_col += 1;
            if (size.m_col <= cursor.m_col) {
                ++cursor.m_row;
                cursor.m_col = 0;
            }
        }
    }
}

} // namespace eltau
