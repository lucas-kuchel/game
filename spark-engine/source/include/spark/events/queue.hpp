#pragma once

#include <spark/types/core.hpp>
#include <spark/types/list.hpp>
#include <spark/types/traits.hpp>

namespace spark {
    template <typename T, typename U = uint64>
    requires(is_unsigned<U>)
    class queue {
    public:
        using event_type = T;
        using size_type = U;

        queue() = default;
        ~queue() = default;

        

    private:
        list<event_type, size_type> list_;
    };
}