#pragma once

#include <new>

#include <spark/types/core.hpp>
#include <spark/types/span.hpp>

#include <spark/utilities/values.hpp>

namespace spark {
    // @brief doubles the size of the container, or sets to 1 if size == 0
    // @note the default growth policy for all spark containers
    class double_growth_policy {
    public:
        template <typename T = uint64>
        requires(is_unsigned<T>)
        inline constexpr static T expand(T value) noexcept {
            return max(static_cast<T>(1), 2 * value);
        }
    };

    // @brief a dynamically sized contiguous list of elements
    // @note operates on a FILO basis (first in = last out)
    template <typename T, typename U = uint64, typename V = double_growth_policy>
    requires(is_unsigned<U>)
    class list {
    public:
        using size_type = U;
        using type = T;
        using growth_policy = V;

        inline constexpr list() noexcept = default;

        inline constexpr ~list() noexcept {
            clear();
        }

        template <typename... Args>
        inline constexpr list(Args&&... args) noexcept
            : size_(sizeof...(Args)), capacity_(sizeof...(Args)) {
            if (capacity_ == 0) {
                return;
            }

            data_ = static_cast<type*>(operator new(sizeof(type) * capacity_));

            size_type i = 0;

            ((new (&data_[i++]) type(forward<Args>(args))), ...);
        }

        inline constexpr list(const list& other)
            : size_(other.size_), capacity_(other.capacity_) {
            if (capacity_ == 0) {
                return;
            }

            data_ = static_cast<type*>(operator new(sizeof(type) * capacity_));

            for (size_type i = 0; i < size_; i++) {
                new (&data_[i]) type(other.data_[i]);
            }
        }

        inline constexpr list(list&& other) noexcept
            : size_(other.size_), capacity_(other.capacity_), data_(other.data_) {
            other.data_ = nullptr;
            other.size_ = 0;
            other.capacity_ = 0;
        }

        inline constexpr list& operator=(const list& other) {
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

        inline constexpr list& operator=(list&& other) noexcept {
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

        [[nodiscard]] inline constexpr type& operator[](size_type index) noexcept {
            return data_[index];
        }

        [[nodiscard]] inline constexpr const type& operator[](size_type index) const noexcept {
            return data_[index];
        }

        inline constexpr operator span<T>() noexcept {
            return span<T>(data_, size_);
        }

        inline constexpr operator span<const T>() const noexcept {
            return span<const T>(data_, size_);
        }

        // @brief sorts elements with a provided algorithm and condition
        template <typename S, typename C>
        inline constexpr void sort() noexcept {
            S::template sort<T, C>(data_, size_);
        }

        // @brief erases and resets the entire list
        inline constexpr void clear() noexcept {
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
        inline constexpr type& push(T&& value) noexcept {
            if (size_ >= capacity_) {
                reserve(growth_policy::expand(capacity_));
            }

            new (&data_[size_]) type(forward<T>(value));

            return data_[size_++];
        }

        // @brief constructs and appends a new element to the list
        // @param arguments for construction of the element
        // @returns reference to the new element
        template <typename... Args>
        inline constexpr type& emplace(Args&&... args) noexcept {
            if (size_ >= capacity_) {
                reserve(growth_policy::expand(capacity_));
            }

            new (static_cast<void*>(&data_[size_])) type(forward<Args>(args)...);

            return data_[size_++];
        }

        // @brief swaps the elements at the provided locations
        inline constexpr void swap(size_type a, size_type b) noexcept {
            if (a == b) {
                return;
            }

            type temporary = data_[a];
            data_[a] = data_[b];
            data_[b] = temporary;
        }

        // @brief removes the end element from the list
        inline constexpr void pop() noexcept {
            if (size_ > 0) {
                data_[--size_].~type();
            }
        }

        // @brief allocates additional space in the list
        // @param new capacity
        // @note will only reallocate if new capacity > current capacity
        inline constexpr void reserve(size_type newCapacity) noexcept {
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
        inline constexpr void resize(size_type newSize) noexcept {
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
        inline constexpr void resize(size_type newSize, const type& value) noexcept {
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
        inline constexpr void trim() noexcept {
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
        [[nodiscard]] inline constexpr size_type size() const noexcept {
            return size_;
        }

        // @brief gives the current allocation size of the list
        [[nodiscard]] inline constexpr size_type capacity() const noexcept {
            return capacity_;
        }

        // @brief checks if the list is empty
        [[nodiscard]] inline constexpr bool empty() const noexcept {
            return size_ == 0;
        }

        // @brief provides access to the raw memory pointer
        [[nodiscard]] inline constexpr type* data() noexcept {
            return data_;
        }

        // @brief provides access to the raw memory pointer
        [[nodiscard]] inline constexpr const type* data() const noexcept {
            return data_;
        }

        // @brief provides a span over a region of this list
        // @param the offset into this list to start from
        // @param the size of the product span
        [[nodiscard]] inline constexpr span<T> sublist(size_type offset, size_type size) noexcept {
            return span<T>(data_ + offset, size);
        }

        // @brief provides a span over a region of this list
        // @param the offset into this list to start from
        // @param the size of the product span
        [[nodiscard]] inline constexpr span<T> sublist(size_type offset, size_type size) const noexcept {
            return span<T>(data_ + offset, size);
        }

        // @brief provides the first element in the list
        [[nodiscard]] inline constexpr type& first() noexcept {
            return *data_;
        }

        // @brief provides the last element in the list
        [[nodiscard]] inline constexpr type& last() noexcept {
            return *(data_ + size_ - 1);
        }

        // @brief provides the first element in the list
        [[nodiscard]] inline constexpr const type& first() const {
            return *data_;
        }

        // @brief provides the last element in the list
        [[nodiscard]] inline constexpr const type& last() const noexcept {
            return *(data_ + size_ - 1);
        }

        inline constexpr type* begin() noexcept {
            return data_;
        }

        inline constexpr type* end() noexcept {
            return data_ + size_;
        }

        inline constexpr const type* begin() const noexcept {
            return data_;
        }

        inline constexpr const type* end() const noexcept {
            return data_ + size_;
        }

    private:
        size_type size_ = 0;
        size_type capacity_ = 0;
        type* data_ = nullptr;
    };
}