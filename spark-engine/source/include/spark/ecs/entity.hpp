#pragma once

#include <spark/types/core.hpp>
#include <spark/types/traits.hpp>

namespace spark {
    template <typename T = uint64>
    requires(is_unsigned<T>)
    class entity {
    public:
        using size_type = T;

        entity() = default;
        ~entity() = default;

        entity(const entity&) = default;
        entity(entity&&) noexcept = default;

        entity& operator=(const entity&) = default;
        entity& operator=(entity&&) noexcept = default;

        bool operator==(const entity& other) const {
            return id_ = other.id_ && generation_ == other.generation_;
        }

        bool operator!=(const entity& other) const {
            return !(*this == other);
        }

        [[nodiscard]] size_type id() const {
            return id_;
        }

        [[nodiscard]] size_type generation() const {
            return generation_;
        }

    private:
        static constexpr size_type dead_sentinel = static_cast<size_type>(-1);

        size_type id_ = dead_sentinel;
        size_type generation_ = dead_sentinel;

        template <typename U>
        requires(is_unsigned<U>)
        friend class registry;
    };
}