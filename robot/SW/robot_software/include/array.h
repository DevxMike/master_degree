#pragma once

#include <cstdint>
#include <utility>

namespace custom{

template <typename T, std::size_t arr_len>
class array{
public:
    array() : size{ arr_len } {};
    array(array<T, arr_len>&& a) : size{ arr_len }{
        auto iter = begin();
        for(auto& x : a){
            *iter = std::move(x);
            ++iter;
        }
    }

    constexpr T& operator[](std::size_t i) { 
        if(is_in_bounds(i)){
            return m_array[i];
        }
        else{
            throw;
        }
    }

    constexpr const T& operator[](std::size_t i) const{
        return (*this).operator[](i);
    }

    constexpr auto begin() noexcept {
        return &m_array[0];
    }

    constexpr auto end() noexcept {
        return begin() + size;
    }

private:
    bool is_in_bounds(std::size_t i) noexcept{
        if(i >= 0 && i < arr_len){
            return true;
        }
        else{
            return false;
        }
    }

    T m_array[arr_len];
    const std::size_t size;
};

}