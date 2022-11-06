/*******************************************************************************
 * @file terminal.cpp
 * @copyright Copyright 2022 Jan Waltl.
 * @license	This file is released under ElTau project's license, see LICENSE.
 ******************************************************************************/

#include <fmt/core.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

#include <eltau/terminal.hpp>

#include "eltau/exception.hpp"
#include "fmt/format.h"

namespace eltau {
namespace {

/*******************************************************************************
 * @throw EltauException if the size is unknown
 ******************************************************************************/
Vec2
get_screen_size() {
    struct winsize w;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == -1) {
        throw EltauException::from_errno("Cannot query the terminal size");
    }
    return {.m_row = w.ws_row, .m_col = w.ws_col};
}

struct termios orig_term {}; // NOLINT

void
restore_terminal() {
    // Switch to the original screen.
    fmt::print("\033[?1049l");
    // Restore the terminal.
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_term);
}

void
setup_terminal() {
    (void)tcgetattr(STDIN_FILENO, &orig_term);
    struct termios raw {
        orig_term
    };
    cfmakeraw(&raw);

    (void)std::atexit(restore_terminal);

    (void)tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
    // Disable output line buffering, required because we do not draw on line basis.
    (void)setvbuf(stdout, nullptr, _IONBF, 0);

    // Switch to an alternative screen -> preserves the history better.
    fmt::print("\033[?1049h");
}

} // namespace
EagerTerminal::EagerTerminal(std::unique_ptr<Element> root) : m_root{std::move(root)}, m_screen{get_screen_size()} {

    setup_terminal();
}
void

EagerTerminal::draw() {
    if (!m_root)
        return;

    const auto dims = m_screen.size();
    // Process elements

    // Draw to the screen buffer
    // m_root->calc_pref_size(dims);
    // m_root->draw(DrawingWindow{{{0, 0}, dims}, m_screen});

    // Draw the screen buffer to the screen.
    fmt::print("\033[H");
    for (std::size_t i = 0; i < dims.m_row; ++i) {
        auto line = m_screen.line(i);
        if (i > 0)
            fmt::print("\r\n");

        std::string x(line.size(), 'x');
        fmt::print("{}\r", x);
        for (const auto& cell : line) {
            break;
            if (cell.m_char[0] != 0)
                fmt::print("{}", cell.m_char.data());
        }
    }
}

} // namespace eltau
