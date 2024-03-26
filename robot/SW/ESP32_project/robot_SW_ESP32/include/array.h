#ifndef array_h
#define array_h

#include <cstdint>
#include <utility>

namespace custom{

template <typename T, std::size_t arr_len>
class array{
public:
    array() noexcept : m_size{ arr_len } {};

    array(array<T, arr_len>&& a) noexcept : m_size{ arr_len }{
        auto iter = begin();
        for(auto& x : a){
            *iter = std::move(x);
            ++iter;
        }
    }

    array(const array<T, arr_len>& a) noexcept : m_size{ arr_len }{
            for(std::size_t i = 0; i < arr_len; ++i){
                m_array[i] = a.m_array[i];
            }

    }

    T& operator[](int i) noexcept { 
        return m_array[i]; // assume that user handles indexes properly
    }

    const T& operator[](int i) const noexcept {
        return m_array[i];
    }

    T* begin() noexcept {
        return &m_array[0];
    }

    T* end() noexcept {
        return begin() + m_size;
    }

    const T* begin() const noexcept{
        return &m_array[0];
    }

    const T* end() const noexcept{
        return begin() + m_size;
    }

    std::size_t size() noexcept {
        return m_size;
    }

    bool operator==(const array<T, arr_len>& a) noexcept{
        for(std::size_t i = 0; i < arr_len; ++i){
            if(m_array[i] != a.m_array[i]){
                return false;
            }
        }

        return true;
    }

    bool operator!=(const array<T, arr_len>& a) noexcept{
        return !(*this == a);
    }

    array<T, arr_len>& operator=(const array<T, arr_len>& a) noexcept{
        for(std::size_t i = 0; i < arr_len; ++i){
            m_array[i] = a.m_array[i];
        }

        return *this;
    }

    array<T, arr_len>& operator=(array<T, arr_len>&& a) noexcept{
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

#endif