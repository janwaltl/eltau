/*******************************************************************************
 * @file shared_state.hpp
 * @copyright Copyright 2022 Jan Waltl.
 * @license	This file is released under ElPhi project's license, see LICENSE.
 ******************************************************************************/
#include <concepts>
#include <cstdint>
#include <memory>

namespace eltau {

struct SharedStateBaseImpl {
public:
    void
    next() noexcept;
    std::size_t
    current() const noexcept;

private:
    std::size_t m_version;
};


template <typename State>
struct SharedStateImpl : public SharedStateBaseImpl {
public:
    template <typename... Args>
    requires(std::constructible_from<State, Args...>) explicit SharedStateImpl(Args&&... args);

    const State*
    get() const;

    // Will inc count in ~(), but we either have to hope this object does not go out of scope
    // or it too has to have its own m_state.
    auto
    borrow();

private:
    std::shared_ptr<State> m_state;
};

template <typename State>
using SharedState = std::shared_ptr<SharedStateImpl<State>>;
} // namespace eltau
