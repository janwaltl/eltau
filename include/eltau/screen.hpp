/*******************************************************************************
 * @file screen.hpp
 * @copyright Copyright 2022 Jan Waltl.
 * @license	This file is released under ElTau project's license, see LICENSE.
 ******************************************************************************/
#pragma once

#include <array>
#include <cstdint>

#include <eltau/coords.hpp>

namespace eltau {

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
 * @brief Support ASCII-only right now.
 ******************************************************************************/
using TerminalCell = unsigned char;


class Screen;

/*******************************************************************************
 * @brief Rectangular area on the screen.
 ******************************************************************************/
class Window {
public:
    /*******************************************************************************
     * @brief New window.
     * @param begin Top-left corner of this window in terminal coordinates.
     * @param size Size for the window.
     * @param screen Screen to associate with this window, reference captured
     * and must outlive this object.
     ******************************************************************************/
    Window(Coords2 begin, Size2 size, Screen& screen) noexcept;

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
    Size2
    size() const noexcept;

    /*******************************************************************************
     * @brief Top-left corner coords, relative to the terminal.
     ******************************************************************************/
    Coords2
    origin() const noexcept;

    /*******************************************************************************
     * @brief Bottom-right corner coords, relative to the terminal.
     ******************************************************************************/
    Coords2
    end() const noexcept;

    /*******************************************************************************
     * @brief Position of cursor, absolute screen coordinates.
     ******************************************************************************/
    Coords2
    cursor() const noexcept;
    /*******************************************************************************
     * @brief Position of the cursor, relative to this window.
     ******************************************************************************/
    Offset2
    cursor_rel() const noexcept;

    /*******************************************************************************
     * @brief Test whether @p pos is inside the window or not.
     ******************************************************************************/
    bool
    is_inside(Coords2 pos) const noexcept;

    /*******************************************************************************
     * @brief Return a sub-window.
     *
     * @param offset Top-left corner of the new window, relative to this window.
     * @param size Size of the new window, clamped appropriately if it is too large.
     * @return New sub-window using the same terminal.
     ******************************************************************************/
    Window
    sub_win(Size2 offset, Size2 size) const;

    /*******************************************************************************
     * @brief If two windows handle the same screen area.
     ******************************************************************************/
    bool
    operator==(const Window& other) const noexcept = default;

    /*******************************************************************************
     * @brief Write to the screen at the current cursor position.
     *
     * @param cell to write.
     * @param advance How to advance the screen cursor.
     ******************************************************************************/
    void
    write(const TerminalCell& cell, Offset2 advance = {.m_row = 0, .m_col = +1});

    /*******************************************************************************
     * @brief Move cursor to the beginning of the next line in this window.
     ******************************************************************************/
    void
    move_nextline() noexcept;

private:
    /*! Top-left corner. */
    Coords2 m_origin{0, 0};
    /*! Size of the window. */
    Size2 m_size{0, 0};
    /*! Screen attached to this window. Held as ptr to keep moveability. */
    Screen* m_screen = nullptr;
};


class Screen {
public:
    explicit Screen(Size2 size) noexcept;
    /*******************************************************************************
     * @brief Default copy ctor.
     ******************************************************************************/
    Screen(const Screen& other) = default;

    /*******************************************************************************
     * @brief Default move ctor.
     ******************************************************************************/
    Screen(Screen&& other) noexcept = default;

    /*******************************************************************************
     * @brief Default copy assignment.
     ******************************************************************************/
    Screen&
    operator=(const Screen& other) = default;

    /*******************************************************************************
     * @brief Default move assignment.
     ******************************************************************************/
    Screen&
    operator=(Screen&& other) noexcept = default;

    /*******************************************************************************
     * @brief Virtual dtor.
     ******************************************************************************/
    virtual ~Screen() noexcept = default;

    /*******************************************************************************
     * @brief Position of the cursor.
     ******************************************************************************/
    Coords2
    cursor() const noexcept;

    /*******************************************************************************
     * @brief Dimensions of the screen.
     ******************************************************************************/
    Size2
    size() const noexcept;

    /*******************************************************************************
     * @brief Write to the screen at the current cursor position.
     *
     * cursor() after the call must be equal to cursor() before the call + @p advance.
     *
     * @param cell to write.
     * @param advance How to advance the screen cursor.
     ******************************************************************************/
    void
    write(const TerminalCell& cell, Offset2 advance = {.m_row = 0, .m_col = +1});

    /*******************************************************************************
     * @brief Move cursor to @p dest position.
     *
     * @param dest Absolute coordinates to move the cursor to.
     ******************************************************************************/
    void
    move_cursor(const Coords2& dest) noexcept;

    /*******************************************************************************
     * @brief Move cursor by the given @p offset .
     *
     * @param offset Amount to advance the cursor by.
     ******************************************************************************/
    void
    advance_cursor(const Offset2& offset) noexcept;

private:
    /*! Cached position of the cursor. */
    Coords2 m_cursor{0, 0};
    /*! Size of the screen. */
    Size2 m_size{0, 0};
};

} // namespace eltau
