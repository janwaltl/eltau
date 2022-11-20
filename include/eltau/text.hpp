/*******************************************************************************
 * @file text.hpp
 * @copyright Copyright 2022 Jan Waltl.
 * @license	This file is released under ElTau project's license, see LICENSE.
 ******************************************************************************/
#pragma once

#include <cstdint>
#include <limits>
#include <string>

#include <eltau/element.hpp>
#include <eltau/screen.hpp>

namespace eltau::ascii {
/*******************************************************************************
 * @brief Basic styled ASCII text with optional wrapping.
 ******************************************************************************/
class Text : public Element {
public:
    /*! Constant denoting unwrapped text. */
    inline constexpr static std::size_t c_no_wrap = std::numeric_limits<std::size_t>::max();

    /*******************************************************************************
     * @brief New ASCII Text element.
     *
     * All non-printable ASCII characters are escaped apart from newline.
     * Which is still respected and correctly wraps the text.
     *
     * @param text String to show, ASCII-only.
     * @param wrap_limit Optional character-based wrapping. c_no_wrap is no wrapping.
     ******************************************************************************/
    explicit Text(std::string_view text, std::size_t wrap_limit = c_no_wrap);

private:
    /*******************************************************************************
     * @brief Return the space needed to render text.
     *
     * By default, the text is rendered in one-line unless wrapped or if contains
     * an explicit newline.
     *
     * Wrapping is either based on `wrap_limit` or the size limits.
     *
     * @param max_size limits on the size.
     * @return Calculated necessary size.
     ******************************************************************************/
    Size2
    do_calc_pref_size(Size2 max_size) override;

    /*******************************************************************************
     * @brief Draw text to the assigned window.
     *
     * Wraps the text if necessary, cuts off the rest that cannot fit if there are
     * not enough lines.
     ******************************************************************************/
    void
    do_draw(Window& window) override;

    /*! Text to render. */
    std::string m_text;
    /*! Hard wrap-limit on the text. */
    std::size_t m_wrap_limit;
};

} // namespace eltau::ascii
