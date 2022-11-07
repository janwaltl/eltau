/*******************************************************************************
 * @file screen.hpp
 * @copyright Copyright 2022 Jan Waltl.
 * @license	This file is released under ElTau project's license, see LICENSE.
 ******************************************************************************/
#pragma once

#include <array>
#include <cstdint>
#include <ostream>
#include <span>
#include <vector>

namespace eltau {

/*! UTF-8 code point. */
using cpoint = char;

/*! UTF-8 uses maximum 4 code points per code unit. */
constexpr std::size_t c_utf8_cpoints = 4;

/*******************************************************************************
 * @brief Style of one Cell
 ******************************************************************************/
enum Style : std::uint8_t {
    Bold = 1 << 0,
    Dim = 1 << 1,
    Italic = 1 << 2,
    Underline = 1 << 3,
    Blink = 1 << 4,
    Inverse = 1 << 5,
    Strike = 1 << 6,
};

/*******************************************************************************
 * @brief 256 Terminal colors
 ******************************************************************************/
struct Color256 {
    std::uint8_t m_value = 0;
};

/*******************************************************************************
 * @brief Terminal cell
 *
 * Basic unit of terminal output - has unique row, column.
 ******************************************************************************/
struct Cell {
    /*! We support up to 6 code units per grapheme/terminal cell/glyph
     *  Enough for normal languages in NFC format, no weird emojis though. */
    constexpr static std::size_t c_max_cunits = 3;

    Style m_style;
    /*! Foreground color. */
    Color256 m_fg;
    /*! Background color. */
    Color256 m_bg;

    /*! One printable UTF-8 character, null-terminated(hence +1). */
    std::array<cpoint, c_utf8_cpoints * c_max_cunits + 1> m_char;
};

// Expected sizes I am aiming for, change with caution.
static_assert(sizeof(Cell) == 16, "Keep nice - 8, 16, or 32 in the future.");

/*******************************************************************************
 * @brief Coordinates in the screen.
 *
 * 0,0 is top-left corner.
 ******************************************************************************/
struct Vec2 {
    std::size_t m_row = 0;
    std::size_t m_col = 0;

    bool
    operator==(const Vec2&) const noexcept = default;
};

/*******************************************************************************
 * @brief Element-wise minimum.
 ******************************************************************************/
Vec2
min(const Vec2& l, const Vec2& r) noexcept;

/*******************************************************************************
 * @brief Element-wise maximum.
 ******************************************************************************/
Vec2
max(const Vec2& l, const Vec2& r) noexcept;

/*******************************************************************************
 * @brief Element-wise sum.
 ******************************************************************************/
Vec2
operator+(const Vec2& l, const Vec2& r);

/*******************************************************************************
 * @brief Saturating element-wise subtraction.
 ******************************************************************************/
Vec2
operator-(const Vec2& l, const Vec2& r);

/*******************************************************************************
 * @brief Pretty-print Vec2.
 *
 * Mainly for testing.
 ******************************************************************************/
std::ostream&
operator<<(std::ostream& os, const Vec2& value);

/*******************************************************************************
 * @brief Rectangular area on the screen.
 ******************************************************************************/
class Window {
public:
    /*******************************************************************************
     * @brief New window.
     ******************************************************************************/
    Window(Vec2 begin, Vec2 size) noexcept;

    /*******************************************************************************
     * @brief Default copy ctor.
     ******************************************************************************/
    Window(const Window& other) = default;

    /*******************************************************************************
     * @brief Default move ctor.
     ******************************************************************************/
    Window(Window&& other) noexcept = default;

    /*******************************************************************************
     * @brief Default copy assignment.
     ******************************************************************************/
    Window&
    operator=(const Window& other) = default;

    /*******************************************************************************
     * @brief Default move assignment.
     ******************************************************************************/
    Window&
    operator=(Window&& other) noexcept = default;

    /*******************************************************************************
     * @brief Virtual dtor to support inheritance.
     ******************************************************************************/
    virtual ~Window() noexcept = default;

    /*******************************************************************************
     * @brief Dimensions of the window.
     ******************************************************************************/
    Vec2
    size() const noexcept;

    /*******************************************************************************
     * @brief Top-left corner coords.
     ******************************************************************************/
    Vec2
    origin() const noexcept;

    /*******************************************************************************
     * @brief Bottom-right corner coords.
     ******************************************************************************/
    Vec2
    end() const noexcept;

    /*******************************************************************************
     * @brief Test whether @p pos is inside the window or not.
     ******************************************************************************/
    bool
    is_inside(Vec2 pos) const noexcept;

    /*******************************************************************************
     * @brief Return a sub-window.
     *
     * @param offset Top-left corner of the new window, relative to begin().
     * @param size Size of the new window, clamped appropriately if it is too large.
     * @return New sub-window.
     ******************************************************************************/
    Window
    sub_win(Vec2 offset, Vec2 size);

    bool
    operator==(const Window& other) const noexcept = default;

private:
    /*! Top-left corner. */
    Vec2 m_origin;
    /*! Size of the window. */
    Vec2 m_size;
};

/*******************************************************************************
 * @brief Abstract terminal screen
 ******************************************************************************/
class Screen {
public:
    /*! Non-owning view of a line of cells. */
    using Line = std::span<Cell>;
    /*! Non-owning read-only view of a line of cells. */
    using cLine = std::span<const Cell>;

    /*******************************************************************************
     * @brief Construct a new screen with the given dimensions.
     *
     * @param size Size of the screen.
     ******************************************************************************/
    explicit Screen(Vec2 size);

    /*******************************************************************************
     * @brief Return screen dimensions passed in the ctor.
     ******************************************************************************/
    Vec2
    size() const noexcept;

    /*******************************************************************************
     * @brief Line-based access to the screen.
     *
     * @param idx Line index, valid range given by dims().
     * @return Empty line if @p idx is out of range.
     ******************************************************************************/
    Line
    line(std::size_t idx) noexcept;

    /*******************************************************************************
     * @brief See non-const version.
     ******************************************************************************/
    cLine
    line(std::size_t idx) const noexcept;

    /*******************************************************************************
     * @brief Cell-based access to the screen.
     *
     * @param coords Cell to return
     * @retval nullptr If the @p coords are not within the screen.
     ******************************************************************************/
    Cell*
    operator[](Vec2 coords) noexcept;

    /*******************************************************************************
     * @brief See non-const version.
     ******************************************************************************/
    const Cell*
    operator[](Vec2 coords) const noexcept;

private:
    Vec2 m_size;
    /*! Row-major storage. */
    std::vector<Cell> m_buffer;
};

/*******************************************************************************
 * @brief Drawable area in a screen.
 ******************************************************************************/
class DrawingWindow : public Window {
public:
    /*******************************************************************************
     * @brief Create a new window.
     *
     * @param win Window to draw on, copied.
     * @param screen Screen to use, reference is captured.
     ******************************************************************************/
    DrawingWindow(const Window& win, Screen& screen) noexcept;

    /*******************************************************************************
     * @brief Same as Window::sub_win .
     ******************************************************************************/
    DrawingWindow
    sub_win(Vec2 offset, Vec2 size);

    /*******************************************************************************
     * @brief Cell-based access to the window.
     *
     * @param coords Cell to return, valid values in range [origin, origin+size).
     * @retval nullptr If the @p coords are not within the window.
     ******************************************************************************/
    Cell*
    operator[](Vec2 coords) noexcept;

    /*******************************************************************************
     * @brief See non-const version.
     ******************************************************************************/
    const Cell*
    operator[](Vec2 coords) const noexcept;

private:
    /*! Used screen, valid unless moved-from. */
    Screen* m_screen;
};


} // namespace eltau
