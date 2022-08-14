/*******************************************************************************
 * @file test_screen.cpp
 * @copyright Copyright 2022 Jan Waltl.
 * @license	This file is released under ElTau project's license, see LICENSE.
 ******************************************************************************/

#include <limits>

#include <catch2/catch_test_macros.hpp>
#include <eltau/screen.hpp>

namespace et = eltau;
TEST_CASE("Vector min") {
    REQUIRE(min(et::Vector{0, 0}, et::Vector{1, 2}) == et::Vector{0, 0});
    REQUIRE(min(et::Vector{5, 1}, et::Vector{1, 5}) == et::Vector{1, 1});
    REQUIRE(min(et::Vector{5, 7}, et::Vector{7, 9}) == et::Vector{5, 7});
    REQUIRE(min(et::Vector{5, 7}, et::Vector{3, 1}) == et::Vector{3, 1});
}
TEST_CASE("Vector max") {
    REQUIRE(max(et::Vector{0, 0}, et::Vector{1, 2}) == et::Vector{1, 2});
    REQUIRE(max(et::Vector{5, 1}, et::Vector{1, 5}) == et::Vector{5, 5});
    REQUIRE(max(et::Vector{5, 7}, et::Vector{7, 9}) == et::Vector{7, 9});
    REQUIRE(max(et::Vector{5, 7}, et::Vector{3, 1}) == et::Vector{5, 7});
}

TEST_CASE("Vector subtraction") {
    constexpr auto m = std::numeric_limits<std::size_t>::max();
    REQUIRE(et::Vector{0, 0} - et::Vector{1, 2} == et::Vector{0, 0});
    REQUIRE(et::Vector{10, 5} - et::Vector{1, 7} == et::Vector{9, 0});
    REQUIRE(et::Vector{12, 18} - et::Vector{20, 18} == et::Vector{0, 0});
    REQUIRE(et::Vector{2, 13} - et::Vector{3, 11} == et::Vector{0, 2});
    REQUIRE(et::Vector{m, 0} - et::Vector{0, m} == et::Vector{m, 0});
    REQUIRE(et::Vector{0, m} - et::Vector{m, 0} == et::Vector{0, m});
}

TEST_CASE("Vector addition") {
    REQUIRE(et::Vector{0, 0} + et::Vector{1, 2} == et::Vector{1, 2});
    REQUIRE(et::Vector{10, 5} + et::Vector{1, 7} == et::Vector{11, 12});
    REQUIRE(et::Vector{12, 18} + et::Vector{20, 18} == et::Vector{32, 36});
    REQUIRE(et::Vector{2, 13} + et::Vector{3, 11} == et::Vector{5, 24});
}
