/*******************************************************************************
 * @file terminal.cpp
 * @copyright Copyright 2022 Jan Waltl.
 * @license	This file is released under ElTau project's license, see LICENSE.
 ******************************************************************************/

#include <fmt/core.h>
#include <fmt/format.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

#include <eltau/exception.hpp>
#include <eltau/terminal.hpp>

namespace eltau {
namespace {

/*******************************************************************************
 * @throw EltauException if the size is unknown
 ******************************************************************************/
Size2
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

EagerTerminal::draw() {}

} // namespace eltau
