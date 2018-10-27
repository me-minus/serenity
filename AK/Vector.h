#pragma once

#include "Assertions.h"
#include "OwnPtr.h"
#include "kmalloc.h"

namespace AK {

template<typename T> class Vector;

template<typename T>
class VectorImpl {
public:
    ~VectorImpl() { }
    static VectorImpl* create(size_t capacity)
    {
        size_t size = sizeof(VectorImpl) + sizeof(T) * capacity;
        void* slot = kmalloc(size);
        new (slot) VectorImpl(capacity);
        return (VectorImpl*)slot;
    }

    size_t size() const { return m_size; }
    size_t capacity() const { return m_capacity; }

    T& at(size_t i) { return *slot(i); }
    const T& at(size_t i) const { return *slot(i); }

    void remove(size_t index)
    {
        ASSERT(index < m_size);
        at(index).~T();
        for (size_t i = index + 1; i < m_size; ++i) {
            new (slot(i - 1)) T(move(at(i)));
            at(i).~T();
        }

        --m_size;
    }

private:
    friend class Vector<T>;

    VectorImpl(size_t capacity) : m_capacity(capacity) { }

    T* tail() { return reinterpret_cast<T*>(this + 1); }
    T* slot(size_t i) { return &tail()[i]; }

    const T* tail() const { return reinterpret_cast<const T*>(this + 1); }
    const T* slot(size_t i) const { return &tail()[i]; }

    size_t m_size { 0 };
    size_t m_capacity;
};

template<typename T>
class Vector {
public:
    Vector() { }
    ~Vector() { clear(); }

    Vector(Vector&& other)
        : m_impl(other.m_impl)
    {
        other.m_impl = nullptr;
    }

    Vector& operator=(Vector&& other)
    {
        if (this != &other) {
            m_impl = other.m_impl;
            other.m_impl = nullptr;
        }
        return *this;
    }

    void clear()
    {
        for (size_t i = 0; i < size(); ++i) {
            at(i).~T();
        }
        kfree(m_impl);
        m_impl = nullptr;
    }

    bool isEmpty() const { return size() == 0; }
    size_t size() const { return m_impl ? m_impl->size() : 0; }
    size_t capacity() const { return m_impl ? m_impl->capacity() : 0; }

    T* data() { return m_impl ? &at(0) : nullptr; }
    const T* data() const { return m_impl ? &at(0) : nullptr; }

    const T& at(size_t i) const { return m_impl->at(i); }
    T& at(size_t i) { return m_impl->at(i); }

    const T& operator[](size_t i) const { return at(i); }
    T& operator[](size_t i) { return at(i); }

    const T& first() const { return at(0); }
    T& first() { return at(0); }

    const T& last() const { return at(size() - 1); }
    T& last() { return at(size() - 1); }

    T takeLast()
    {
        ASSERT(!isEmpty());
        T value = move(last());
        last().~T();
        --m_impl->m_size;
        return value;
    }

    void remove(size_t index)
    {
        m_impl->remove(index);
    }

    void append(T&& value)
    {
        ensureCapacity(size() + 1);
        new (m_impl->slot(m_impl->m_size)) T(move(value));
        ++m_impl->m_size;
    }

    void append(const T& value)
    {
        ensureCapacity(size() + 1);
        new (m_impl->slot(m_impl->m_size)) T(value);
        ++m_impl->m_size;
    }

    void ensureCapacity(size_t neededCapacity)
    {
        if (capacity() >= neededCapacity)
            return;
        size_t newCapacity = paddedCapacity(neededCapacity);
        auto newImpl = VectorImpl<T>::create(newCapacity);
        if (m_impl) {
            newImpl->m_size = m_impl->m_size;
            for (size_t i = 0; i < size(); ++i) {
                new (newImpl->slot(i)) T(move(m_impl->at(i)));
                m_impl->at(i).~T();
            }
            kfree(m_impl);
        }
        m_impl = newImpl;
    }

    class Iterator {
    public:
        bool operator!=(const Iterator& other) { return m_index != other.m_index; }
        Iterator& operator++() { ++m_index; return *this; }
        T& operator*() { return m_vector[m_index]; }
    private:
        friend class Vector;
        Iterator(Vector& vector, size_t index) : m_vector(vector), m_index(index) { }
        Vector& m_vector;
        size_t m_index { 0 };
    };

    Iterator begin() { return Iterator(*this, 0); }
    Iterator end() { return Iterator(*this, size()); }

    class ConstIterator {
    public:
        bool operator!=(const ConstIterator& other) { return m_index != other.m_index; }
        ConstIterator& operator++() { ++m_index; return *this; }
        const T& operator*() const { return m_vector[m_index]; }
    private:
        friend class Vector;
        ConstIterator(const Vector& vector, const size_t index) : m_vector(vector), m_index(index) { }
        const Vector& m_vector;
        size_t m_index { 0 };
    };

    ConstIterator begin() const { return ConstIterator(*this, 0); }
    ConstIterator end() const { return ConstIterator(*this, size()); }

private:
    static size_t paddedCapacity(size_t capacity)
    {
        return max(size_t(4), capacity + (capacity / 4) + 4);
    }

    VectorImpl<T>* m_impl { nullptr };
};

}

using AK::Vector;

