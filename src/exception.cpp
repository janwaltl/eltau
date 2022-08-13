/*******************************************************************************
 * @file exception.cpp
 * @copyright Copyright 2022 Jan Waltl.
 * @license	This file is released under ElPhi project's license, see LICENSE.
 ******************************************************************************/
#include <cerrno>
#include <cstring>

#include <fmt/format.h>

#include <eltau/exception.hpp>


namespace eltau {
EltauException
EltauException::from_errno(std::string_view msg) {
    auto e = errno;
    std::string buffer;
    buffer.resize(256);
    char* ret = strerror_r(e, buffer.data(), buffer.size());
    (void)ret;
    return EltauException{fmt::format("C Error: {}, reason: {}", msg, ret)};
}
} // namespace eltau
