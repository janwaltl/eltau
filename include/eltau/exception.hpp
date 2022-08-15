/*******************************************************************************
 * @file exception.hpp
 * @copyright Copyright 2022 Jan Waltl.
 * @license	This file is released under ElTau project's license, see LICENSE.
 ******************************************************************************/
#pragma once

#include <stdexcept>

namespace eltau {
/*******************************************************************************
 * @brief Base exception class for ElTau project.
 *
 * Inherits logic error because it has std::string ctor.
 ******************************************************************************/
class EltauException : public std::logic_error {
public:
    using std::logic_error::logic_error;

    /*******************************************************************************
     * @brief Default copy ctor.
     ******************************************************************************/
    EltauException(const EltauException& other) = default;

    /*******************************************************************************
     * @brief Default move ctor.
     ******************************************************************************/
    EltauException(EltauException&& other) noexcept = default;

    /*******************************************************************************
     * @brief Default copy assignment.
     ******************************************************************************/
    EltauException&
    operator=(const EltauException& other) = default;

    /*******************************************************************************
     * @brief Default move assignment.
     ******************************************************************************/
    EltauException&
    operator=(EltauException&& other) noexcept = default;

    /*******************************************************************************
     * @brief Default destructor.
     ******************************************************************************/
    ~EltauException() override = default;

    /*******************************************************************************
     * @brief Construct an exception based on current errno value.
     *
     * @param msg is prepended to the `strerror` string.
     ******************************************************************************/
    static EltauException
    from_errno(std::string_view msg);
};

} // namespace eltau
