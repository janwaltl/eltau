/*******************************************************************************
 * @file element.cpp
 * @copyright Copyright 2022 Jan Waltl.
 * @license	This file is released under ElTau project's license, see LICENSE.
 ******************************************************************************/
#include <cassert>

#include <eltau/element.hpp>

namespace eltau {

Vec2
Element::calc_pref_size(Vec2 max_size) {
    // Only calc size for feasible bounds.
    if (max_size.m_col == 0 || max_size.m_row == 0)
        return m_last_pref_size = {0, 0};
    return m_last_pref_size = this->do_calc_pref_size(max_size);
}

void
Element::draw(DrawingWindow& window) {

    return this->do_draw(window);
}

Vec2
Element::get_last_pref_size() const noexcept {
    return m_last_pref_size;
}
} // namespace eltau
