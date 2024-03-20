#pragma once

#include <cstdint>
#include <utility>

namespace custom{

template <typename T, std::size_t arr_len>
class array{
public:
    constexpr array() : m_size{ arr_len } {};
    constexpr array(array<T, arr_len>&& a) : m_size{ arr_len }{
        auto iter = begin();
        for(auto& x : a){
            *iter = std::move(x);
            ++iter;
        }
    }

    constexpr T& operator[](std::size_t i) noexcept { 
        return m_array[i]; // assume that user handles indexes properly
    }

    constexpr const T& operator[](std::size_t i) const noexcept {
        return (*this).operator[](i);
    }

    constexpr auto begin() noexcept {
        return &m_array[0];
    }

    constexpr auto end() noexcept {
        return begin() + m_size;
    }

    constexpr const auto begin() const noexcept{
        return &m_array[0];
    }

    constexpr const auto end() const noexcept{
        return begin() + m_size;
    }

    constexpr auto size() noexcept {
        return m_size;
    }

private:
    T m_array[arr_len];
    const std::size_t m_size;
};

}