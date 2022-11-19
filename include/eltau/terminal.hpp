/*******************************************************************************
 * @file terminal.hpp
 * @copyright Copyright 2022 Jan Waltl.
 * @license	This file is released under ElTau project's license, see LICENSE.
 ******************************************************************************/

#include <memory>

#include <eltau/element.hpp>
#include <eltau/screen.hpp>

namespace eltau {

/*******************************************************************************
 * @brief Terminal without caching.
 ******************************************************************************/
class EagerTerminal {
public:
    /*******************************************************************************
     * @brief New full-screen terminal with specified root element.
     *
     * @param root The root of the TUI to draw.
     * @param sreen Screen to draw to.
     ******************************************************************************/
    explicit EagerTerminal(std::unique_ptr<Element> root, std::unique_ptr<Screen> screen);

    /*******************************************************************************
     * @brief Draw TUI.
     *
     * Uncached version that always re-draws the whole terminal screen.
     ******************************************************************************/
    void
    draw();

private:
    /*! Root TUI element to draw. */
    std::unique_ptr<Element> m_root;
    /*! Screen to draw on. */
    std::unique_ptr<Screen> m_screen;
};
} // namespace eltau
