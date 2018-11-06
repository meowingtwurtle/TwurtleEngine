#pragma once

#define RC_NOEXCEPT_CONSTRUCT_ASSIGN_IF(type, cond)                                                                                        \
    type(type const&) noexcept(cond) = default;                                                                                            \
    type(type&&) noexcept(cond) = default;                                                                                                 \
    type& operator=(type const&) noexcept(cond) = default;                                                                                 \
    type& operator=(type&&) noexcept(cond) = default;

#define RC_NOEXCEPT_CONSTRUCT_ASSIGN(type) RC_NOEXCEPT_CONSTRUCT_ASSIGN_IF(type, true)
