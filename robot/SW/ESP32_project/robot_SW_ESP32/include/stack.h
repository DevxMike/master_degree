#ifndef stack_h
#define stack_h

#include "array.h"

namespace custom{

template<typename T, std::size_t stack_depth>
class stack{
public:
    stack() noexcept: 
        max_elem{ stack_depth }, elem_count{ 0 }, elements{  } {}

    stack(const stack<T, stack_depth>& s) noexcept:
        max_elem{ stack_depth }, elem_count{ s.elem_count }, elements{ s.elements } {}

    stack(stack<T, stack_depth>&& s) noexcept:
        max_elem{ stack_depth }, elem_count{ s.elem_count }, elements{ std::move(s.elements) } {
            s.elem_count = 0;
            }

    stack<T, stack_depth>& operator=(const stack<T, stack_depth>& s) noexcept {
        elem_count = s.elem_count;
        elements = s.elements;

        return *this;
    }
    stack<T, stack_depth>& operator=(stack<T, stack_depth>&& s) noexcept {
        elem_count = s.elem_count;
        s.elem_count = 0;
        elements = std::move(s.elements);

        return *this;
    }
    std::size_t count() const noexcept {
        return elem_count;
    }
    std::size_t size() const noexcept {
        return max_elem;
    }
    bool full() const noexcept {
        return max_elem == elem_count;
    }
    bool empty() const noexcept {
        return elem_count == 0;
    }
    T&& pop() noexcept {
        return std::move(elements[--elem_count]);
    }
    bool push(const T& e) noexcept {
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
    bool push(T&& e) noexcept {
        bool result = true;

        if(full()){
            result = false;
        }
        else{
            elements[elem_count++] = std::move(e);
        }

        return result;
    }
    bool operator==(const stack<T, stack_depth>& s) noexcept {
        return elem_count == s.elem_count && elements == s.elements;
    }
    bool operator!=(const stack<T, stack_depth>& s) noexcept {
        return !(*this == s);
    }

private:
    const std::size_t max_elem;
    std::size_t elem_count;
    array<T, stack_depth> elements;
};

}

#endif