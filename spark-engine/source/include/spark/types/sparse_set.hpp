#pragma once

#include <spark/types/core.hpp>
#include <spark/types/list.hpp>
#include <spark/types/traits.hpp>

namespace spark {
    template <typename T, typename U = uint64>
    requires(is_unsigned<U>)
    class sparse_set {
    public:
        using type = T;
        using size_type = U;

        static constexpr size_type dead_index = static_cast<size_type>(-1);

        sparse_set() = default;
        ~sparse_set() = default;

        sparse_set(const sparse_set&) = default;
        sparse_set(sparse_set&&) noexcept = default;

        sparse_set& operator=(const sparse_set&) = default;
        sparse_set& operator=(sparse_set&&) noexcept = default;

        template <typename... Args>
        type& insert(size_type index, Args&&... args) {
            if (index >= sparse_.size()) {
                sparse_.resize(index + 1, dead_index);
            }

            if (sparse_[index] == dead_index) {
                sparse_[index] = dense_.size();
                dense_.emplace(forward<Args>(args)...);
                denseTable_.emplace(index);
            }

            return dense_[sparse_[index]];
        }

        void remove(size_type index) {
            if (!contains(index)) {
                return;
            }

            size_type denseIndex = sparse_[index];
            size_type lastDense = dense_.size() - 1;

            if (denseIndex != lastDense) {
                dense_.swap(denseIndex, lastDense);
                denseTable_.swap(denseIndex, lastDense);

                size_type movedIndex = denseTable_[denseIndex];
                sparse_[movedIndex] = denseIndex;
            }

            sparse_[index] = dead_index;

            dense_.pop();
            denseTable_.pop();
        }

        [[nodiscard]] bool contains(size_type index) const {
            return index < sparse_.size() && sparse_[index] != dead_index;
        }

        [[nodiscard]] type& get(size_type index) {
            return dense_[sparse_[index]];
        }

        [[nodiscard]] const type& get(size_type index) const {
            return dense_[sparse_[index]];
        }

        [[nodiscard]] size_type size() const {
            return dense_.size();
        }

        [[nodiscard]] bool empty() const {
            return dense_.empty();
        }

        type* begin() {
            return dense_.begin();
        }

        type* end() {
            return dense_.end();
        }

        const type* begin() const {
            return dense_.begin();
        }

        const type* end() const {
            return dense_.end();
        }

    private:
        list<type, size_type> dense_;
        list<size_type, size_type> denseTable_;
        list<size_type, size_type> sparse_;
    };
}