#pragma once

#include <cstdint>
#include <utility>

namespace custom{

template <typename T, std::size_t arr_len>
class array{
public:
    constexpr array() noexcept : m_size{ arr_len } {};

    constexpr array(array<T, arr_len>&& a) noexcept : m_size{ arr_len }{
        auto iter = begin();
        for(auto& x : a){
            *iter = std::move(x);
            ++iter;
        }
    }

    constexpr array(const array<T, arr_len>& a) noexcept : m_size{ arr_len }{
            for(std::size_t i = 0; i < arr_len; ++i){
                m_array[i] = a.m_array[i];
            }

    }

    constexpr T& operator[](int i) noexcept { 
        return m_array[i]; // assume that user handles indexes properly
    }

    constexpr const T& operator[](int i) const noexcept {
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

    constexpr auto operator==(const array<T, arr_len>& a) noexcept{
        for(std::size_t i = 0; i < arr_len; ++i){
            if(m_array[i] != a.m_array[i]){
                return false;
            }
        }

        return true;
    }

    constexpr auto operator!=(const array<T, arr_len>& a) noexcept{
        return !(*this == a);
    }

    constexpr auto& operator=(const array<T, arr_len>& a) noexcept{
        for(std::size_t i = 0; i < arr_len; ++i){
            m_array[i] = a.m_array[i];
        }

        return *this;
    }

    constexpr auto& operator=(array<T, arr_len>&& a) noexcept{
        for(std::size_t i = 0; i < arr_len; ++i){
            m_array[i] = std::move(a.m_array[i]);
        }

        return *this;
    }

private:
    T m_array[arr_len];
    const std::size_t m_size;
};

}