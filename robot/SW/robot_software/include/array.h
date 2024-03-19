#pragma once

#include <cstdint>
#include <utility>

namespace custom{

template <typename T, std::size_t arr_len>
class array{
public:
    array() : m_size{ arr_len } {};
    array(array<T, arr_len>&& a) : m_size{ arr_len }{
        auto iter = begin();
        for(auto& x : a){
            *iter = std::move(x);
            ++iter;
        }
    }

    constexpr T& operator[](std::size_t i) { 
        return m_array[i]; // assume that user handles indexes properly
    }

    constexpr const T& operator[](std::size_t i) const{
        return (*this).operator[](i);
    }

    constexpr auto begin() noexcept {
        return &m_array[0];
    }

    constexpr auto end() noexcept {
        return begin() + m_size;
    }
    constexpr auto size(){
        return m_size;
    }

private:
    T m_array[arr_len];
    const std::size_t m_size;
};

}