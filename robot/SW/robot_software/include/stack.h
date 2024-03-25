#pragma once

#include "array.h"

namespace custom{

template<typename T, std::size_t stack_depth>
class stack{
public:
    constexpr stack() noexcept: 
        max_elem{ stack_depth }, elem_count{ 0 }, elements{  } {}

    constexpr stack(const stack<T, stack_depth>& s) noexcept:
        max_elem{ stack_depth }, elem_count{ s.elem_count }, elements{ s.elements } {}

    constexpr stack(stack<T, stack_depth>&& s) noexcept:
        max_elem{ stack_depth }, elem_count{ s.elem_count }, elements{ std::move(s.elements) } {
            s.elem_count = 0;
        }

    constexpr auto& operator=(const stack<T, stack_depth>& s) noexcept {
        elem_count = s.elem_count;
        elements = s.elements;

        return *this;
    }
    constexpr auto& operator=(stack<T, stack_depth>&& s) noexcept {
        elem_count = s.elem_count;
        s.elem_count = 0;
        elements = std::move(s.elements);

        return *this;
    }
    constexpr auto count() const noexcept {
        return elem_count;
    }
    constexpr auto size() const noexcept {
        return max_elem;
    }
    constexpr auto full() const noexcept {
        return max_elem == elem_count;
    }
    constexpr auto empty() const noexcept {
        return elem_count == 0;
    }
    constexpr T&& pop() noexcept {
        return std::move(elements[--elem_count]);
    }
    constexpr bool push(const T& e) noexcept {
        bool result = true;

        if(full()){
            // do nothing, cant push to a full stack
            result = false;
        }
        else{
            elements[elem_count++] = e;
        }

        return result;
    }
    constexpr bool push(T&& e) noexcept {
        bool result = true;

        if(full()){
            result = false;
        }
        else{
            elements[elem_count++] = std::move(e);
        }

        return result;
    }
    constexpr bool operator==(const stack<T, stack_depth>& s) noexcept {
        return elem_count == s.elem_count && elements == s.elements;
    }
    constexpr bool operator!=(const stack<T, stack_depth>& s) noexcept {
        return !(*this == s);
    }

private:
    const std::size_t max_elem;
    std::size_t elem_count;
    array<T, stack_depth> elements;
};

}