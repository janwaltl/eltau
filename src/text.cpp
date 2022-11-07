/*******************************************************************************
 * @file text.cpp
 * @copyright Copyright 2022 Jan Waltl.
 * @license	This file is released under ElTau project's license, see LICENSE.
 ******************************************************************************/

#include <algorithm>
#include <cassert>

#include <eltau/text.hpp>

namespace eltau::ascii {
namespace {
/*! Escape non-printable ASCII with a space. */
constexpr char c_escape_char = ' ';

/*******************************************************************************
 * @brief Replace non-printable ASCII characters, leaves newlines intact.
 *
 * @param str String to replace, ASCII-only.
 * @return Escaped copy of a string.
 ******************************************************************************/
std::string
escape_ascii(std::string str) noexcept {
    std::string res{std::move(str)};

    std::replace_if(
        res.begin(), res.end(), [](char c) { return c < 32 && c != '\n'; }, c_escape_char);

    return res;
}

/*******************************************************************************
 * @brief Replace non-printable ASCII characters, leaves newlines intact.
 *
 * @param str String to replace, ASCII-only.
 * @param wrap_limit Limit line length.
 * @return Escaped copy of a string.
 ******************************************************************************/
Vec2
calc_rows_cols(std::string_view str, std::size_t wrap_limit) {
    std::size_t max_cols = 0;
    std::size_t cur_cols = 0;
    std::size_t rows = str.empty() ? 0 : 1;
    for (auto c : str) {
        // Also "consumes" newline as wrap_limit if both conds are true.
        if (c == '\n' || cur_cols >= wrap_limit) {
            ++rows;
            max_cols = std::max(std::exchange(cur_cols, 0), max_cols);
        }
        if (c != '\n') // Place the character
            ++cur_cols;
    }
    max_cols = std::max(cur_cols, max_cols);
    return Vec2{.m_row = rows, .m_col = max_cols};
}

} // namespace

Text::Text(std::string_view text, std::size_t wrap_limit) :
    m_text(escape_ascii(std::string(text))), m_wrap_limit(wrap_limit) {}

Vec2
Text::do_calc_pref_size(Vec2 max_size) {
    assert(max_size.m_row > 0 && max_size.m_col > 0);

    auto limit{std::min(max_size.m_col, m_wrap_limit)};

    return min(calc_rows_cols(m_text, limit), max_size);
}

void
Text::do_draw(DrawingWindow& window) {
    (void)window;
    // TODO (jw) Implement me after terminal rework.
}
} // namespace eltau::ascii
