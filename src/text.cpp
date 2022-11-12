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
Size2
calc_rows_cols(std::string_view str, std::size_t wrap_limit) {
    Size2::size_type max_cols = 0;
    Size2::size_type cur_cols = 0;
    Size2::size_type rows = str.empty() ? 0 : 1;
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
    return Size2{.m_row = rows, .m_col = max_cols};
}

} // namespace

Text::Text(std::string_view text, std::size_t wrap_limit) :
    m_text(escape_ascii(std::string(text))), m_wrap_limit(wrap_limit) {}

Size2
Text::do_calc_pref_size(Size2 max_size) {
    assert(max_size.m_row > 0 && max_size.m_col > 0);

    auto col_limit{std::min(static_cast<std::size_t>(max_size.m_col), m_wrap_limit)};

    return min(calc_rows_cols(m_text, col_limit), max_size);
}

void
Text::do_draw(Window& window) {
    // Precondition of do_draw.
    assert(window.cursor() == window.origin());

    auto col_limit{std::min(window.size().m_col, m_wrap_limit)};
    auto row_limit{window.size().m_row};

    // Window Cursor starts at 0,0.
    for (auto c : m_text) {
        auto cursor = window.cursor_rel();
        assert(cursor.m_row >= 0);
        assert(cursor.m_col >= 0);
        if (static_cast<Size2::size_type>(cursor.m_row) >= row_limit)
            // Run out of drawing space
            break;
        if (c == '\n' || static_cast<Size2::size_type>(cursor.m_col) >= col_limit)
            // \r\n
            window.move_nextline();
        else if (window.is_inside(window.cursor()))
            // Advances cursor
            window.write(c);
    }
}
} // namespace eltau::ascii
