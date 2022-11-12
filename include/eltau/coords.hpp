/*******************************************************************************
 * @file coords.hpp
 * @copyright Copyright 2022 Jan Waltl.
 * @license	This file is released under ElTau project's license, see LICENSE.
 ******************************************************************************/
#pragma once

#include <cstdint>
#include <ostream>

namespace eltau {

struct Offset2;
struct Size2;

/*******************************************************************************
 * @brief Coordinates in the screen.
 *
 * 0,0 is top-left corner.
 *
 * All operations are saturating w.r.t. 0 - e.g. `4 - 10 = 0`.
 * Overflow is asserted as it signals bugs in the code for our use case.
 ******************************************************************************/
struct Coords2 {
    using coords_type = std::uint32_t;

    /*! Row in the screen, upmost is 0. */
    coords_type m_row = 0;
    /*! Column in the screen, leftmost is 0. */
    coords_type m_col = 0;

    /*******************************************************************************
     * @brief Element-wise comparison.
     ******************************************************************************/
    bool
    operator==(const Coords2&) const noexcept = default;

    /*******************************************************************************
     * @brief Element-wise saturating addition.
     ******************************************************************************/
    Coords2&
    operator+=(const Offset2& other) noexcept;

    /*******************************************************************************
     * @brief Element-wise addition.
     ******************************************************************************/
    Coords2&
    operator+=(const Size2& other) noexcept;

    /*******************************************************************************
     * @brief Element-wise saturating subtraction.
     ******************************************************************************/
    Coords2&
    operator-=(const Offset2& other) noexcept;

    /*******************************************************************************
     * @brief Treat coordinates as positive offset.
     ******************************************************************************/
    explicit operator Offset2() const noexcept;

    /*******************************************************************************
     * @brief Treat coordinates as positive size.
     ******************************************************************************/
    explicit operator Size2() const noexcept;
};

/*******************************************************************************
 * @brief Signed offset in the screen.
 *
 * To work with relative coordinates.
 ******************************************************************************/
struct Offset2 {
    using offset_type = std::int32_t;
    /*! Row offset, positive in right direction. */
    offset_type m_row = 0;
    /*! Column offset, positive in bottom direction. */
    offset_type m_col = 0;

    /*******************************************************************************
     * @brief Element-wise comparison.
     ******************************************************************************/
    bool
    operator==(const Offset2&) const noexcept = default;

    /*******************************************************************************
     * @brief Element-wise addition.
     ******************************************************************************/
    Offset2&
    operator+=(const Offset2& other) noexcept;

    /*******************************************************************************
     * @brief Element-wise subtraction.
     ******************************************************************************/
    Offset2&
    operator-=(const Offset2& other) noexcept;
};

/*******************************************************************************
 * @brief Positive offset, analogous to std::size_t for 1D.
 ******************************************************************************/
struct Size2 {
    using size_type = std::size_t;

    size_type m_row = 0;
    size_type m_col = 0;

    /*******************************************************************************
     * @brief Element-wise comparison.
     ******************************************************************************/
    bool
    operator==(const Size2&) const noexcept = default;

    /*******************************************************************************
     * @brief Element-wise addition.
     ******************************************************************************/
    Size2&
    operator+=(const Size2& other) noexcept;

    /*******************************************************************************
     * @brief Treat size as positive offset.
     ******************************************************************************/
    explicit operator Offset2() const noexcept;
};


/*******************************************************************************
 * @brief Element-wise minimum.
 ******************************************************************************/
Coords2
min(const Coords2& l, const Coords2& r) noexcept;

/*******************************************************************************
 * @brief Element-wise minimum.
 ******************************************************************************/
Offset2
min(const Offset2& l, const Offset2& r) noexcept;

/*******************************************************************************
 * @brief Element-wise minimum.
 ******************************************************************************/
Size2
min(const Size2& l, const Size2& r) noexcept;

/*******************************************************************************
 * @brief Element-wise maximum.
 ******************************************************************************/
Coords2
max(const Coords2& l, const Coords2& r) noexcept;

/*******************************************************************************
 * @brief Element-wise maximum.
 ******************************************************************************/
Offset2
max(const Offset2& l, const Offset2& r) noexcept;

/*******************************************************************************
 * @brief Element-wise maximum.
 ******************************************************************************/
Size2
max(const Size2& l, const Size2& r) noexcept;

/*******************************************************************************
 * @brief Element-wise clamp.
 *
 * Assumes low<=high.
 *
 * @param vec Vector to clamp into [low,high].
 * @param low Low point.
 * @param high High point.
 ******************************************************************************/
Coords2
clamp(const Coords2& vec, const Coords2& low, const Coords2& high) noexcept;

/*******************************************************************************
 * @brief Element-wise clamp.
 *
 * Assumes low<=high.
 *
 * @param vec Vector to clamp into [low,high].
 * @param low Low point.
 * @param high High point.
 ******************************************************************************/
Offset2
clamp(const Offset2& vec, const Offset2& low, const Offset2& high) noexcept;

/*******************************************************************************
 * @brief Element-wise clamp.
 *
 * Assumes low<=high.
 *
 * @param vec Vector to clamp into [low,high].
 * @param low Low point.
 * @param high High point.
 ******************************************************************************/
Size2
clamp(const Size2& vec, const Size2& low, const Size2& high) noexcept;

/*******************************************************************************
 * @brief Element-wise saturating addition.
 ******************************************************************************/
Coords2
operator+(const Coords2& l, const Offset2& r) noexcept;

/*******************************************************************************
 * @brief Element-wise addition.
 ******************************************************************************/
Coords2
operator+(const Coords2& l, const Size2& r) noexcept;

/*******************************************************************************
 * @brief Element-wise addition.
 ******************************************************************************/
Offset2
operator+(const Offset2& l, const Offset2& r) noexcept;

/*******************************************************************************
 * @brief Element-wise addition.
 ******************************************************************************/
Size2
operator+(const Size2& l, const Size2& r) noexcept;

/*******************************************************************************
 * @brief Saturating element-wise subtraction.
 ******************************************************************************/
Coords2
operator-(const Coords2& l, const Offset2& r) noexcept;

/*******************************************************************************
 * @brief Saturating element-wise subtraction.
 ******************************************************************************/
Offset2
operator-(const Coords2& l, const Coords2& r) noexcept;

/*******************************************************************************
 * @brief Saturating element-wise subtraction.
 ******************************************************************************/
Size2
operator-(const Size2& l, const Size2& r) noexcept;

/*******************************************************************************
 * @brief Element-wise subtraction.
 ******************************************************************************/
Offset2
operator-(const Offset2& l, const Offset2& r) noexcept;

/*******************************************************************************
 * @brief Unary minus.
 ******************************************************************************/
Offset2
operator-(const Offset2& offset) noexcept;

/*******************************************************************************
 * @brief Pretty-print.
 *
 * Mainly for testing.
 ******************************************************************************/
std::ostream&
operator<<(std::ostream& os, const Coords2& value);

/*******************************************************************************
 * @brief Pretty-print.
 *
 * Mainly for testing.
 ******************************************************************************/
std::ostream&
operator<<(std::ostream& os, const Size2& value);

/*******************************************************************************
 * @brief Pretty-print.
 *
 * Mainly for testing.
 ******************************************************************************/
std::ostream&
operator<<(std::ostream& os, const Offset2& value);

} // namespace eltau
