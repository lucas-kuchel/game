#pragma once

#include <spark/types/list.hpp>

namespace spark {
    template <typename T = uint64>
    requires(is_unsigned<T>)
    class type_indexer {
    public:
        using size_type = T;

        template <typename U>
        size_type index() {
            size_type globalID = type_indexer::globalIndex<U>();

            if (globalID >= typeMap_.size()) {
                typeMap_.resize(globalID + 1, size_type(-1));
            }

            if (typeMap_[globalID] == size_type(-1)) {
                typeMap_[globalID] = counter_++;
            }

            return typeMap_[globalID];
        }

        void reset() {
            typeMap_.clear();
            counter_ = 0;
        }

    private:
        template <typename>
        static size_type globalIndex() {
            static size_type id = static_cast<size_type>(globalCounter_++);

            return id;
        }

        spark::list<size_type> typeMap_;

        size_type counter_ = 0;

        inline static uint64 globalCounter_ = 0;
    };
}