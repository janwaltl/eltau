/*******************************************************************************
 * @file basic.hpp
 * @copyright Copyright 2022 Jan Waltl.
 * @license	This file is released under ElTau project's license, see LICENSE.
 ******************************************************************************/


#include <memory>
#include <thread>

#include <fmt/core.h>
#include <fmt/format.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

#include <eltau/exception.hpp>
#include <eltau/terminal.hpp>
#include <eltau/text.hpp>

namespace {
using namespace std::chrono_literals;
constexpr auto c_tick = 100ms;
/*******************************************************************************
 * @throw EltauException if the size is unknown
 ******************************************************************************/
eltau::Size2
get_screen_size() {
    struct winsize w;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == -1) {
        throw eltau::EltauException::from_errno("Cannot query the terminal size");
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

class TScreen : public eltau::Screen {
public:
    TScreen() : eltau::Screen(get_screen_size()) { setup_terminal(); }
    /*******************************************************************************
     * @brief Move-only.
     ******************************************************************************/
    TScreen(const TScreen&) = delete;

    /*******************************************************************************
     * @brief Default move ctor.
     ******************************************************************************/
    TScreen(TScreen&& other) noexcept = default;

    /*******************************************************************************
     * @brief Move-only.
     ******************************************************************************/
    TScreen&
    operator=(const TScreen&) = delete;

    /*******************************************************************************
     * @brief Default move assignment.
     ******************************************************************************/
    TScreen&
    operator=(TScreen&& other) noexcept = default;

    ~TScreen() override { restore_terminal(); }

private:
    eltau::Coords2
    do_write(const eltau::TerminalCell& cell) noexcept override {
        fmt::print("%c", cell);
        auto c = cursor();
        // TODO (jw) Solve wrapping based on screen size.
        return c + eltau::Offset2{0, 1};
    }
    eltau::Coords2
    do_advance_cursor(const eltau::Offset2& offset) noexcept override {
        return do_move_cursor(cursor() + offset);
    }

    eltau::Coords2
    do_move_cursor(const eltau::Coords2& dest) noexcept override {
        fmt::print("\033[{};{}H", dest.m_row, dest.m_col);
        // TODO (jw) Check whether the terminal really moved the cursor to this line/position.
        return dest;
    }
};
} // namespace
int
main() {
    eltau::EagerTerminal term{std::make_unique<eltau::ascii::Text>("Hello world"), std::make_unique<TScreen>()};

    for (std::size_t i = 0; i < 100; ++i) {
        term.draw();
        std::this_thread::sleep_for(c_tick);
    }

    return 0;
}
