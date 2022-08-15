/*******************************************************************************
 * @file basic.hpp
 * @copyright Copyright 2022 Jan Waltl.
 * @license	This file is released under ElTau project's license, see LICENSE.
 ******************************************************************************/


#include <memory>
#include <thread>

#include <eltau/element.hpp>
#include <eltau/terminal.hpp>

namespace {
using namespace std::chrono_literals;
constexpr auto c_tick = 100ms;
} // namespace
int
main() {

    eltau::EagerTerminal term{std::make_unique<eltau::Text>("Hello world", 0)};

    for (std::size_t i = 0; i < 100; ++i) {
        term.draw();
        std::this_thread::sleep_for(c_tick);
    }

    return 0;
}
