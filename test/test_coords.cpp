/*******************************************************************************
 * @file test_coords.cpp
 * @copyright Copyright 2022 Jan Waltl.
 * @license	This file is released under ElTau project's license, see LICENSE.
 ******************************************************************************/

#include <limits>
#include <sstream>
#include <unordered_set>

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <eltau/coords.hpp>

namespace et = eltau;
TEST_CASE("Coords min") {
    REQUIRE(min(et::Coords2{0, 0}, et::Coords2{1, 2}) == et::Coords2{0, 0});
    REQUIRE(min(et::Coords2{5, 1}, et::Coords2{1, 5}) == et::Coords2{1, 1});
    REQUIRE(min(et::Coords2{5, 7}, et::Coords2{7, 9}) == et::Coords2{5, 7});
    REQUIRE(min(et::Coords2{5, 7}, et::Coords2{3, 1}) == et::Coords2{3, 1});
}

TEST_CASE("Offset min") {
    REQUIRE(min(et::Offset2{0, 0}, et::Offset2{1, 2}) == et::Offset2{0, 0});
    REQUIRE(min(et::Offset2{5, 1}, et::Offset2{1, 5}) == et::Offset2{1, 1});
    REQUIRE(min(et::Offset2{5, 7}, et::Offset2{7, 9}) == et::Offset2{5, 7});
    REQUIRE(min(et::Offset2{5, 7}, et::Offset2{3, 1}) == et::Offset2{3, 1});
}

TEST_CASE("Size min") {
    REQUIRE(min(et::Size2{0, 0}, et::Size2{1, 2}) == et::Size2{0, 0});
    REQUIRE(min(et::Size2{5, 1}, et::Size2{1, 5}) == et::Size2{1, 1});
    REQUIRE(min(et::Size2{5, 7}, et::Size2{7, 9}) == et::Size2{5, 7});
    REQUIRE(min(et::Size2{5, 7}, et::Size2{3, 1}) == et::Size2{3, 1});
}

TEST_CASE("Coords max") {
    REQUIRE(max(et::Coords2{0, 0}, et::Coords2{1, 2}) == et::Coords2{1, 2});
    REQUIRE(max(et::Coords2{5, 1}, et::Coords2{1, 5}) == et::Coords2{5, 5});
    REQUIRE(max(et::Coords2{5, 7}, et::Coords2{7, 9}) == et::Coords2{7, 9});
    REQUIRE(max(et::Coords2{5, 7}, et::Coords2{3, 1}) == et::Coords2{5, 7});
}

TEST_CASE("Offset max") {
    REQUIRE(max(et::Offset2{0, 0}, et::Offset2{1, 2}) == et::Offset2{1, 2});
    REQUIRE(max(et::Offset2{5, 1}, et::Offset2{1, 5}) == et::Offset2{5, 5});
    REQUIRE(max(et::Offset2{5, 7}, et::Offset2{7, 9}) == et::Offset2{7, 9});
    REQUIRE(max(et::Offset2{5, 7}, et::Offset2{3, 1}) == et::Offset2{5, 7});
}

TEST_CASE("Size2 max") {
    REQUIRE(max(et::Size2{0, 0}, et::Size2{1, 2}) == et::Size2{1, 2});
    REQUIRE(max(et::Size2{5, 1}, et::Size2{1, 5}) == et::Size2{5, 5});
    REQUIRE(max(et::Size2{5, 7}, et::Size2{7, 9}) == et::Size2{7, 9});
    REQUIRE(max(et::Size2{5, 7}, et::Size2{3, 1}) == et::Size2{5, 7});
}

TEST_CASE("Coords clamp") {
    REQUIRE(clamp(et::Coords2{5, 4}, et::Coords2{0, 0}, et::Coords2{6, 6}) == et::Coords2{5, 4});
    REQUIRE(clamp(et::Coords2{5, 4}, et::Coords2{6, 0}, et::Coords2{8, 3}) == et::Coords2{6, 3});
    REQUIRE(clamp(et::Coords2{5, 4}, et::Coords2{0, 5}, et::Coords2{2, 8}) == et::Coords2{2, 5});
}

TEST_CASE("Offset clamp") {
    REQUIRE(clamp(et::Offset2{5, 4}, et::Offset2{0, 0}, et::Offset2{6, 6}) == et::Offset2{5, 4});
    REQUIRE(clamp(et::Offset2{5, 4}, et::Offset2{6, 0}, et::Offset2{8, 3}) == et::Offset2{6, 3});
    REQUIRE(clamp(et::Offset2{5, 4}, et::Offset2{0, 5}, et::Offset2{2, 8}) == et::Offset2{2, 5});
}

TEST_CASE("Size clamp") {
    REQUIRE(clamp(et::Size2{5, 4}, et::Size2{0, 0}, et::Size2{6, 6}) == et::Size2{5, 4});
    REQUIRE(clamp(et::Size2{5, 4}, et::Size2{6, 0}, et::Size2{8, 3}) == et::Size2{6, 3});
    REQUIRE(clamp(et::Size2{5, 4}, et::Size2{0, 5}, et::Size2{2, 8}) == et::Size2{2, 5});
}

TEST_CASE("Coords member Size addition") {
    REQUIRE((et::Coords2{0, 0} += et::Size2{1, 2}) == et::Coords2{1, 2});
    REQUIRE((et::Coords2{10, 5} += et::Size2{1, 7}) == et::Coords2{11, 12});
    REQUIRE((et::Coords2{12, 18} += et::Size2{20, 18}) == et::Coords2{32, 36});
    REQUIRE((et::Coords2{2, 13} += et::Size2{3, 11}) == et::Coords2{5, 24});
}

TEST_CASE("Coords member Offset addition") {
    REQUIRE((et::Coords2{0, 0} += et::Offset2{1, 2}) == et::Coords2{1, 2});
    REQUIRE((et::Coords2{10, 5} += et::Offset2{1, 7}) == et::Coords2{11, 12});
    REQUIRE((et::Coords2{12, 18} += et::Offset2{20, 18}) == et::Coords2{32, 36});
    REQUIRE((et::Coords2{2, 13} += et::Offset2{3, 11}) == et::Coords2{5, 24});
    SECTION("Negative offset") {
        REQUIRE((et::Coords2{0, 0} += et::Offset2{-1, -2}) == et::Coords2{0, 0});
        REQUIRE((et::Coords2{10, 5} += et::Offset2{-1, -7}) == et::Coords2{9, 0});
        REQUIRE((et::Coords2{12, 18} += et::Offset2{-20, -18}) == et::Coords2{0, 0});
        REQUIRE((et::Coords2{2, 13} += et::Offset2{-3, -11}) == et::Coords2{0, 2});
    }
}

TEST_CASE("Coords member Offset subtraction") {
    constexpr auto m = std::numeric_limits<et::Offset2::offset_type>::max() - 1;
    REQUIRE((et::Coords2{0, 0} -= et::Offset2{1, 2}) == et::Coords2{0, 0});
    REQUIRE((et::Coords2{10, 5} -= et::Offset2{1, 7}) == et::Coords2{9, 0});
    REQUIRE((et::Coords2{12, 18} -= et::Offset2{20, 18}) == et::Coords2{0, 0});
    REQUIRE((et::Coords2{2, 13} -= et::Offset2{3, 11}) == et::Coords2{0, 2});
    REQUIRE((et::Coords2{m, 0} -= et::Offset2{0, m}) == et::Coords2{m, 0});
    REQUIRE((et::Coords2{0, m} -= et::Offset2{m, 0}) == et::Coords2{0, m});

    SECTION("Negative offset") {
        REQUIRE((et::Coords2{0, 0} -= et::Offset2{-1, -2}) == et::Coords2{1, 2});
        REQUIRE((et::Coords2{10, 5} -= et::Offset2{-1, -7}) == et::Coords2{11, 12});
        REQUIRE((et::Coords2{12, 18} -= et::Offset2{-20, -18}) == et::Coords2{32, 36});
        REQUIRE((et::Coords2{2, 13} -= et::Offset2{-3, -11}) == et::Coords2{5, 24});
    }
}

TEST_CASE("Coords to Size") { REQUIRE(static_cast<et::Size2>(et::Coords2{5, 6}) == et::Size2{5, 6}); }

TEST_CASE("Coords to Offset") { REQUIRE(static_cast<et::Offset2>(et::Coords2{5, 6}) == et::Offset2{5, 6}); }


TEST_CASE("Offset member subtraction") {
    REQUIRE((et::Offset2{0, 0} -= et::Offset2{1, 2}) == et::Offset2{-1, -2});
    REQUIRE((et::Offset2{10, 5} -= et::Offset2{1, 7}) == et::Offset2{9, -2});
    REQUIRE((et::Offset2{12, 18} -= et::Offset2{20, 18}) == et::Offset2{-8, 0});
    REQUIRE((et::Offset2{2, 13} -= et::Offset2{3, 11}) == et::Offset2{-1, 2});
}

TEST_CASE("Offset member addition") {
    REQUIRE((et::Offset2{0, 0} += et::Offset2{1, 2}) == et::Offset2{1, 2});
    REQUIRE((et::Offset2{10, 5} += et::Offset2{1, 7}) == et::Offset2{11, 12});
    REQUIRE((et::Offset2{12, 18} += et::Offset2{20, 18}) == et::Offset2{32, 36});
    REQUIRE((et::Offset2{2, 13} += et::Offset2{3, 11}) == et::Offset2{5, 24});
    REQUIRE((et::Offset2{10, -5} += et::Offset2{0, 1}) == et::Offset2{10, -4});
    REQUIRE((et::Offset2{10, -5} += et::Offset2{-2, -1}) == et::Offset2{8, -6});
}

TEST_CASE("Size member addition") {
    REQUIRE((et::Size2{0, 0} += et::Size2{1, 2}) == et::Size2{1, 2});
    REQUIRE((et::Size2{10, 5} += et::Size2{1, 7}) == et::Size2{11, 12});
    REQUIRE((et::Size2{12, 18} += et::Size2{20, 18}) == et::Size2{32, 36});
    REQUIRE((et::Size2{2, 13} += et::Size2{3, 11}) == et::Size2{5, 24});
}

TEST_CASE("Size to Offset") { REQUIRE(static_cast<et::Offset2>(et::Size2{5, 6}) == et::Offset2{5, 6}); }

TEST_CASE("Coords Offset addition") {
    REQUIRE((et::Coords2{0, 0} + et::Offset2{1, 2}) == et::Coords2{1, 2});
    REQUIRE((et::Coords2{10, 5} + et::Offset2{1, 7}) == et::Coords2{11, 12});
    REQUIRE((et::Coords2{12, 18} + et::Offset2{20, 18}) == et::Coords2{32, 36});
    REQUIRE((et::Coords2{2, 13} + et::Offset2{3, 11}) == et::Coords2{5, 24});
    SECTION("Negative offset") {
        REQUIRE((et::Coords2{0, 0} + et::Offset2{-1, -2}) == et::Coords2{0, 0});
        REQUIRE((et::Coords2{10, 5} + et::Offset2{-1, -7}) == et::Coords2{9, 0});
        REQUIRE((et::Coords2{12, 18} + et::Offset2{-20, -18}) == et::Coords2{0, 0});
        REQUIRE((et::Coords2{2, 13} + et::Offset2{-3, -11}) == et::Coords2{0, 2});
    }
}


TEST_CASE("Coords Size addition") {
    REQUIRE(et::Coords2{0, 0} + et::Size2{1, 2} == et::Coords2{1, 2});
    REQUIRE(et::Coords2{10, 5} + et::Size2{1, 7} == et::Coords2{11, 12});
    REQUIRE(et::Coords2{12, 18} + et::Size2{20, 18} == et::Coords2{32, 36});
    REQUIRE(et::Coords2{2, 13} + et::Size2{3, 11} == et::Coords2{5, 24});
}

TEST_CASE("Offset addition") {
    REQUIRE(et::Offset2{0, 0} + et::Offset2{1, 2} == et::Offset2{1, 2});
    REQUIRE(et::Offset2{10, 5} + et::Offset2{1, 7} == et::Offset2{11, 12});
    REQUIRE(et::Offset2{2, 13} + et::Offset2{3, 11} == et::Offset2{5, 24});
    REQUIRE(et::Offset2{-1, -5} + et::Offset2{3, 11} == et::Offset2{2, 6});
    REQUIRE(et::Offset2{10, -5} + et::Offset2{0, 1} == et::Offset2{10, -4});
    REQUIRE(et::Offset2{10, -5} + et::Offset2{-2, -1} == et::Offset2{8, -6});
}

TEST_CASE("Size addition") {
    REQUIRE((et::Size2{0, 0} + et::Size2{1, 2}) == et::Size2{1, 2});
    REQUIRE((et::Size2{10, 5} + et::Size2{1, 7}) == et::Size2{11, 12});
    REQUIRE((et::Size2{12, 18} + et::Size2{20, 18}) == et::Size2{32, 36});
    REQUIRE((et::Size2{2, 13} + et::Size2{3, 11}) == et::Size2{5, 24});
}

TEST_CASE("Coords Offset subtraction") {
    constexpr auto m = std::numeric_limits<et::Offset2::offset_type>::max();
    REQUIRE((et::Coords2{0, 0} - et::Offset2{1, 2}) == et::Coords2{0, 0});
    REQUIRE((et::Coords2{10, 5} - et::Offset2{1, 7}) == et::Coords2{9, 0});
    REQUIRE((et::Coords2{12, 18} - et::Offset2{20, 18}) == et::Coords2{0, 0});
    REQUIRE((et::Coords2{2, 13} - et::Offset2{3, 11}) == et::Coords2{0, 2});
    REQUIRE((et::Coords2{m, 0} - et::Offset2{0, m}) == et::Coords2{m, 0});
    REQUIRE((et::Coords2{0, m} - et::Offset2{m, 0}) == et::Coords2{0, m});

    SECTION("Negative offset") {
        REQUIRE((et::Coords2{0, 0} - et::Offset2{-1, -2}) == et::Coords2{1, 2});
        REQUIRE((et::Coords2{10, 5} - et::Offset2{-1, -7}) == et::Coords2{11, 12});
        REQUIRE((et::Coords2{12, 18} - et::Offset2{-20, -18}) == et::Coords2{32, 36});
        REQUIRE((et::Coords2{2, 13} - et::Offset2{-3, -11}) == et::Coords2{5, 24});
    }
}


TEST_CASE("Coords subtraction") {
    constexpr auto m = std::numeric_limits<et::Offset2::offset_type>::max() - 1;
    REQUIRE(et::Coords2{0, 0} - et::Coords2{1, 2} == et::Offset2{-1, -2});
    REQUIRE(et::Coords2{10, 5} - et::Coords2{1, 7} == et::Offset2{9, -2});
    REQUIRE(et::Coords2{12, 18} - et::Coords2{20, 18} == et::Offset2{-8, 0});
    REQUIRE(et::Coords2{2, 13} - et::Coords2{3, 11} == et::Offset2{-1, 2});
    REQUIRE(et::Coords2{m, 0} - et::Coords2{0, m} == et::Offset2{m, -m});
    REQUIRE(et::Coords2{0, m} - et::Coords2{m, 0} == et::Offset2{-m, m});
}

TEST_CASE("Size subtraction") {
    constexpr auto m = std::numeric_limits<et::Offset2::offset_type>::max();
    REQUIRE(et::Size2{0, 0} - et::Size2{1, 2} == et::Size2{0, 0});
    REQUIRE(et::Size2{10, 5} - et::Size2{1, 7} == et::Size2{9, 0});
    REQUIRE(et::Size2{12, 18} - et::Size2{20, 18} == et::Size2{0, 0});
    REQUIRE(et::Size2{2, 13} - et::Size2{3, 11} == et::Size2{0, 2});
    REQUIRE(et::Size2{m, 0} - et::Size2{0, m} == et::Size2{m, 0});
    REQUIRE(et::Size2{0, m} - et::Size2{m, 0} == et::Size2{0, m});
}

TEST_CASE("Offset subtraction") {
    REQUIRE(et::Offset2{0, 0} - et::Offset2{1, 2} == et::Offset2{-1, -2});
    REQUIRE(et::Offset2{10, 5} - et::Offset2{1, 7} == et::Offset2{9, -2});
    REQUIRE(et::Offset2{12, 18} - et::Offset2{20, 18} == et::Offset2{-8, 0});
    REQUIRE(et::Offset2{2, 13} - et::Offset2{3, 11} == et::Offset2{-1, 2});
}

TEST_CASE("Offset unary minus") { REQUIRE(et::Offset2{5, -3} == (-et::Offset2{-5, 3})); }


TEST_CASE("Coords pretty-print") {
    std::stringstream s;
    s << et::Coords2{15, 9};
    REQUIRE(s.str() == "{r=15, c=9}");
}


TEST_CASE("Offset pretty-print") {
    std::stringstream s;
    SECTION("Positive") {
        s << et::Offset2{15, 9};
        REQUIRE(s.str() == "{r=+15, c=+9}");
    }
    SECTION("Negative") {
        s << et::Offset2{-15, -9};
        REQUIRE(s.str() == "{r=-15, c=-9}");
    }
    SECTION("Zero") {
        s << et::Offset2{0, 0};
        REQUIRE(s.str() == "{r=+0, c=+0}");
    }
}

TEST_CASE("Size pretty-print") {
    std::stringstream s;
    s << et::Size2{15, 9};
    REQUIRE(s.str() == "{r=15, c=9}");
}
