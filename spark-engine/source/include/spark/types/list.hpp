#pragma once

#include <new>

#include <spark/types/core.hpp>
#include <spark/types/span.hpp>

#include <spark/utilities/values.hpp>

namespace spark {
    // @brief a dynamically sized contiguous list of elements
    // @note operates on a FILO basis (first in = last out)
    template <typename T, typename U = uint64>
    requires(is_unsigned<U>)
    class list {
    public:
        using size_type = U;
        using type = T;

        list() = default;

        ~list() {
            clear();
        }

        template <typename... Args>
        list(Args&&... args)
            : size_(sizeof...(Args)), capacity_(sizeof...(Args)) {
            if (capacity_ == 0) {
                return;
            }

            data_ = static_cast<type*>(operator new(sizeof(type) * capacity_));

            size_type i = 0;

            ((new (&data_[i++]) type(forward<Args>(args))), ...);
        }

        list(const list& other)
            : size_(other.size_), capacity_(other.capacity_) {
            if (capacity_ == 0) {
                return;
            }

            data_ = static_cast<type*>(operator new(sizeof(type) * capacity_));

            for (size_type i = 0; i < size_; i++) {
                new (&data_[i]) type(other.data_[i]);
            }
        }

        list(list&& other) noexcept
            : size_(other.size_), capacity_(other.capacity_), data_(other.data_) {
            other.data_ = nullptr;
            other.size_ = 0;
            other.capacity_ = 0;
        }

        list& operator=(const list& other) {
            if (this == &other) {
                return *this;
            }

            clear();

            size_ = other.size_;
            capacity_ = other.capacity_;

            if (capacity_ > 0) {
                data_ = static_cast<type*>(operator new(sizeof(type) * capacity_));

                for (size_type i = 0; i < size_; i++) {
                    new (&data_[i]) type(other.data_[i]);
                }
            }

            return *this;
        }

        list& operator=(list&& other) noexcept {
            if (this == &other) {
                return *this;
            }

            clear();

            size_ = other.size_;
            capacity_ = other.capacity_;
            data_ = other.data_;

            other.data_ = nullptr;
            other.size_ = 0;
            other.capacity_ = 0;

            return *this;
        }

        type& operator[](size_type index) {
            return data_[index];
        }

        const type& operator[](size_type index) const {
            return data_[index];
        }

        operator span<T>() {
            return span<T>(data_, size_);
        }

        operator span<const T>() const {
            return span<const T>(data_, size_);
        }

        // @brief sorts elements with a provided algorithm and condition
        template <typename S, typename C>
        void sort() {
            S::template sort<T, C>(data_, size_);
        }

        // @brief erases and resets the entire list
        void clear() {
            if (data_ != nullptr) {
                for (size_type i = 0; i < size_; i++) {
                    data_[i].~type();
                }

                operator delete(data_);

                data_ = nullptr;
            }
            size_ = 0;
            capacity_ = 0;
        }

        // @brief appends a new element to the list
        // @param the new element
        // @returns reference to the new element
        type& push(T&& value) {
            if (size_ >= capacity_) {
                reserve(capacity_ == 0 ? 1 : capacity_ * 2);
            }

            new (&data_[size_]) type(forward<T>(value));

            return data_[size_++];
        }

        // @brief constructs and appends a new element to the list
        // @param arguments for construction of the element
        // @returns reference to the new element
        template <typename... Args>
        type& emplace(Args&&... args) {
            if (size_ >= capacity_) {
                reserve(capacity_ == 0 ? 1 : capacity_ * 2);
            }

            new (static_cast<void*>(&data_[size_])) type(forward<Args>(args)...);

            return data_[size_++];
        }

        // @brief swaps the elements at the provided locations
        void swap(size_type a, size_type b) {
            if (a == b) {
                return;
            }

            type temporary = data_[a];
            data_[a] = data_[b];
            data_[b] = temporary;
        }

        // @brief removes the end element from the list
        void pop() {
            if (size_ > 0) {
                data_[--size_].~type();
            }
        }

        // @brief allocates additional space in the list
        // @param new capacity
        // @note will only reallocate if new capacity > current capacity
        void reserve(size_type newCapacity) {
            if (newCapacity <= capacity_) {
                return;
            }

            type* newData = static_cast<type*>(operator new(sizeof(type) * newCapacity));

            for (size_type i = 0; i < size_; i++) {
                new (static_cast<void*>(&newData[i])) type(move(data_[i]));
                data_[i].~type();
            }

            operator delete(data_);

            data_ = newData;
            capacity_ = newCapacity;
        }

        // @brief resizes the list to the provided size
        // @param new size
        // @note new elements will be default constructed if new size > current size
        // @note old elements will be preserved if size permits
        void resize(size_type newSize) {
            if (newSize == 0) {
                clear();
                return;
            }

            if (newSize > capacity_) {
                reserve(newSize);
            }

            for (size_type i = size_; i < newSize; i++) {
                new (static_cast<void*>(&data_[i])) type;
            }

            for (size_type i = newSize; i < size_; i++) {
                data_[i].~type();
            }

            size_ = newSize;
        }

        // @brief resizes the list to the provided size
        // @param new size
        // @param value to use for new elements
        // @note old elements will be preserved if size permits
        void resize(size_type newSize, const type& value) {
            if (newSize == 0) {
                clear();
                return;
            }

            if (newSize > capacity_) {
                reserve(newSize);
            }

            for (size_type i = size_; i < newSize; i++) {
                new (&data_[i]) type(value);
            }

            for (size_type i = newSize; i < size_; i++) {
                data_[i].~type();
            }

            size_ = newSize;
        }

        // @brief frees all unused space
        // @note deallocates if size < capacity
        void trim() {
            if (size_ == capacity_) {
                return;
            }

            if (size_ == 0) {
                clear();
                return;
            }

            type* newData = static_cast<type*>(operator new(sizeof(type) * size_));

            for (size_type i = 0; i < size_; i++) {
                new (static_cast<void*>(&newData[i])) type(move(data_[i]));

                data_[i].~type();
            }

            operator delete(data_);

            data_ = newData;
            capacity_ = size_;
        }

        // @brief gives the size of the list
        size_type size() const {
            return size_;
        }

        // @brief gives the current allocation size of the list
        size_type capacity() const {
            return capacity_;
        }

        // @brief checks if the list is empty
        bool empty() const {
            return size_ == 0;
        }

        // @brief provides access to the raw memory pointer
        type* data() {
            return data_;
        }

        // @brief provides access to the raw memory pointer
        const type* data() const {
            return data_;
        }

        // @brief provides a span over a region of this list
        // @param the offset into this list to start from
        // @param the size of the product span
        span<T> sublist(size_type offset, size_type size) {
            return span<T>(data_ + offset, size);
        }

        // @brief provides a span over a region of this list
        // @param the offset into this list to start from
        // @param the size of the product span
        span<T> sublist(size_type offset, size_type size) const {
            return span<T>(data_ + offset, size);
        }

        // @brief provides the first element in the list
        type& first() {
            return *data_;
        }

        // @brief provides the last element in the list
        type& last() {
            return *(data_ + size_ - 1);
        }

        // @brief provides the first element in the list
        const type& first() const {
            return *data_;
        }

        // @brief provides the last element in the list
        const type& last() const {
            return *(data_ + size_ - 1);
        }

        type* begin() {
            return data_;
        }

        type* end() {
            return data_ + size_;
        }

        const type* begin() const {
            return data_;
        }

        const type* end() const {
            return data_ + size_;
        }

    private:
        size_type size_ = 0;
        size_type capacity_ = 0;
        type* data_ = nullptr;
    };
}