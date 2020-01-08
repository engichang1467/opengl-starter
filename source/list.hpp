#include <stdlib.h>
#include <algorithm>

#include "types.hpp"

template <typename T>
struct List
{
    u32   size;
    u32   capacity;
    void* buffer;
    List()
    {
        size = 0;
        capacity = 1;
        buffer = malloc(sizeof(T));
    }

    List(u32 newCapacity)
    {
        size = 0;
        capacity = newCapacity;
        buffer = malloc(capacity * sizeof(T));
    }

    ~List()
    {
        free(buffer);
    }

    T& operator[](u32 index)
    {
        return ((T*)buffer)[index];
    }

    void push(T element)
    {
        ((T*)buffer)[size] = element;
        size += 1;

        if (size == capacity)
        {
            resize(capacity * 4);
        }
    }

    T pop()
    {
        size -= 1;
        return ((T*)buffer)[size];
    }

    T getValue(u32 index)
    {
        return ((T*)buffer)[index];
    }

    T* getPtr(i32 index)
    {
        return (T*)(&buffer + index * sizeof(T));
    }

    void resize(u32 newCapacity)
    {
        void* newBuffer = malloc(newCapacity);
        u32   lastIndex = std::min(size, newCapacity) - 1;

        for (u32 index = 0; index <= lastIndex; index += 1)
        {
            ((T*)newBuffer)[index] = ((T*)buffer)[index];
        }

        free(buffer);
        buffer = newBuffer;
    }

    void each(void(func)(T))
    {
        for (u32 index = 0; index < size; index += 1)
        {
            func(((T*)buffer)[index]);
        }
    }

    template <typename F>
    List<F> map(F(func)(T))
    {
        auto list = List<F>();
        list.size = size;
        for (u32 index = 0; index < size; index += 1)
        {
            list[index] = func(((T*)buffer)[index]);
        }
        return list;
    }
};