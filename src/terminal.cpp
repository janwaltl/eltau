/*******************************************************************************
 * @file terminal.cpp
 * @copyright Copyright 2022 Jan Waltl.
 * @license	This file is released under ElTau project's license, see LICENSE.
 ******************************************************************************/
#include <eltau/exception.hpp>
#include <eltau/terminal.hpp>

namespace eltau {
EagerTerminal::EagerTerminal(std::unique_ptr<Element> root, std::unique_ptr<Screen> screen) :
    m_root{std::move(root)}, m_screen{std::move(screen)} {}
void

EagerTerminal::draw() {}

} // namespace eltau
