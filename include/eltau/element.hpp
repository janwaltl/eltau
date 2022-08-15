/*******************************************************************************
 * @file element.hpp
 * @copyright Copyright 2022 Jan Waltl.
 * @license	This file is released under ElTau project's license, see LICENSE.
 ******************************************************************************/
#pragma once

#include <functional>
#include <memory>
#include <string>
#include <type_traits>

#include <eltau/screen.hpp>

namespace eltau {

/*******************************************************************************
 * @brief Basic TUI drawable element.
 ******************************************************************************/
class Element {
public:
    Element() = default;
    /*******************************************************************************
     * @brief Default copy ctor.
     ******************************************************************************/
    Element(const Element& other) = default;
    /*******************************************************************************
     * @brief Default move ctor.
     ******************************************************************************/
    Element(Element&& other) noexcept = default;
    /*******************************************************************************
     * @brief Default copy assignment.
     ******************************************************************************/
    Element&
    operator=(const Element& other) = default;
    /*******************************************************************************
     * @brief Default move assignment.
     ******************************************************************************/
    Element&
    operator=(Element&& other) noexcept = default;

    /*******************************************************************************
     * @brief Default destructor.
     ******************************************************************************/
    virtual ~Element() noexcept = default;

    /*******************************************************************************
     * @brief Recalculate preferred minimal size given the restrictions.
     *
     * It is the responsibility of the element to truncate its content if it does
     * not fit within the restrictions.
     *
     * @param max_size Maximum allowed size of the element.
     * @return Preferred size of the element, must not exceed @p max_size.
     ******************************************************************************/
    Vector
    calc_pref_size(Vector max_size);

    /*******************************************************************************
     * @brief Draw the element to the assigned window.
     *
     * The element can use this window in its entirety. Calculated based on the
     * previously queried preferred size via calc_pref_size().
     *
     * @param window Window assigned to this element.
     ******************************************************************************/
    void
    draw(DrawingWindow& window);

    /*******************************************************************************
     * @brief Last recaulculated preferred size.
     *
     * @return Empty if the size has not been calculated yet.
     ******************************************************************************/
    Vector
    get_last_pref_size() const noexcept;

private:
    /*******************************************************************************
     * @brief Virtual version of calc_pref_size().
     *
     * Derived class must implement for its element.
     ******************************************************************************/
    virtual Vector
    do_calc_pref_size(Vector max_size) = 0;

    /*******************************************************************************
     * @brief Virtual version of draw().
     *
     * Derived class must implement for its element.
     ******************************************************************************/
    virtual void
    do_draw(DrawingWindow& window) = 0;

    /*! Cached preferred size. */
    Vector m_last_pref_size{};
};


/*******************************************************************************
 * @brief Basic styled UTF-8 text with optional wrapping.
 ******************************************************************************/
class Text : public Element {
public:
    /*******************************************************************************
     * @brief New Text element.
     *
     * Respects newline in the input string.
     *
     * @param text String to show, UTF-8 supported.
     * @param wrap_limit Optional automatic wrapping. 0 is no wrapping.
     ******************************************************************************/
    Text(std::string_view text, std::size_t wrap_limit);

private:
    /*******************************************************************************
     * @brief Return the space needed to render text.
     *
     * By default, the text is rendered in one-line unless wrapped or if contains
     * an explicit newline.
     * Wrapping is either based on `wrap_limit` or the size limits.
     *
     * @param max_size limits on the size.
     * @return Calculated necessary size.
     ******************************************************************************/
    Vector
    do_calc_pref_size(Vector max_size) override;

    /*******************************************************************************
     * @brief Draw text to the assigned window.
     *
     * Wraps the text if necessary, cuts off the rest that cannot fit if there are
     * not enough lines.
     ******************************************************************************/
    void
    do_draw(DrawingWindow& window) override;

    /*! Text to render. */
    std::string m_text;
    /*! Hard wrap-limit on the text. */
    std::size_t m_wrap_limit;
};

// Notes:
// Is container aligned or should parent control that?
// Element | hcenter | vcenter | hleft | hright | vup | vdown | hflex | vflex(weight) |  center | flex | scrollable
//-> AlignedElement
//	- Or just set fields of every element <- YEA.


// VContainer{elems...,fmt_string=""}
// VContainer{{weights,elems}...,fmt_string=" =-+"}

template <typename... Elems>
class HContainer : public Element {

public:
    template <typename... Ts>
    explicit HContainer(Ts&&... elems);

private:
    Vector
    do_calc_pref_size(Vector max_size) override;

    void
    do_draw(DrawingWindow& window) override;

    std::tuple<Elems...> m_elems;
};

template <typename... Ts>
HContainer(Ts...) -> HContainer<std::decay_t<Ts>...>;

template <typename... Elems>
template <typename... Ts>
HContainer<Elems...>::HContainer(Ts&&... elems) : m_elems{std::forward<Ts>(elems)...} {}

template <typename... Elems>
Vector
HContainer<Elems...>::do_calc_pref_size(Vector max_size) {
    // Without alignment and flex.
    // 	If we are flex, claim maximum but still calc them.

    auto elems = std::apply([](auto&... v) { return std::array{static_cast<Element&>(v)...}; });

    auto remaining{max_size};
    for (std::size_t i = 0; i < elems.size(); ++i) {
        remaining -= elems[i]->calc_pref_size(remaining);
    };
    return max_size - remaining;
}

template <typename... Elems>
void
HContainer<Elems...>::do_draw(DrawingWindow& window) {

    // Sane iteration
    auto elems = std::apply([](auto&... v) { return std::array{static_cast<Element&>(v)...}; });
    for (std::size_t i = 0; i < elems.size(); ++i) {
        // Restrict to its preferred size.
        auto s = elems[i]->get_last_pref_size();
        auto r = elems[i]->draw(window.sub_win(0, s));
        // Shift right (because we are horizontal container, rows are fixed globally for us).
        window = window.sub_win({.m_row = 0, .m_col = s.m_col});
    };
}

} // namespace eltau
