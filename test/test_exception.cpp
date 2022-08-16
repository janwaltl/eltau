/*******************************************************************************
 * @file test_exception.cpp
 * @copyright Copyright 2022 Jan Waltl.
 * @license	This file is released under ElTau project's license, see LICENSE.
 ******************************************************************************/

#include <cerrno>
#include <limits>
#include <string>
#include <unordered_set>

#include <fmt/core.h>

#include <catch2/catch_test_macros.hpp>
#include <eltau/exception.hpp>

namespace et = eltau;

using namespace std::string_literals;

TEST_CASE("Exception string is captured") {
    const auto err_msg{"Hello world"s};

    et::EltauException e{err_msg};

    REQUIRE(e.what() == err_msg);
}

TEST_CASE("Exception errno - no error") {

    SECTION("Empty prefix") {
        errno = 0;
        auto e{et::EltauException::from_errno("")};

        REQUIRE(e.what() == "C Error: , reason: Success"s);
    }
    SECTION("Custom msg") {
        errno = 0;
        const auto err_msg{"Hello"s};
        auto e{et::EltauException::from_errno(err_msg)};

        REQUIRE(e.what() == fmt::format("C Error: {}, reason: Success", err_msg));
    }
}

TEST_CASE("Exception errno - eagain") {

    SECTION("Empty prefix") {
        errno = EPERM;
        auto e{et::EltauException::from_errno("")};

        REQUIRE(e.what() == "C Error: , reason: Operation not permitted"s);
    }
    SECTION("Custom msg") {
        errno = EPERM;
        const auto err_msg{"Hello"s};
        auto e{et::EltauException::from_errno(err_msg)};

        REQUIRE(e.what() == fmt::format("C Error: {}, reason: Operation not permitted", err_msg));
    }
}
