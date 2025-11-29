#pragma once

#include <spark/types/filler.hpp>
#include <spark/types/index.hpp>
#include <spark/types/sparse_set.hpp>

#include <spark/ecs/bitset.hpp>
#include <spark/ecs/entity.hpp>

namespace spark {
    template <typename T = uint64>
    requires(is_unsigned<T>)
    class registry {
    public:
        using size_type = T;
        using entity_type = entity<size_type>;

        registry() = default;
        ~registry() {
            for (size_type i = 0; i < sparseSets_.size(); i++) {
                if (sparseSetDestructors_[i]) {
                    sparseSetDestructors_[i](&sparseSets_[i]);
                }
            }
        }

        registry(const registry&) = delete;
        registry(registry&&) noexcept = default;

        registry& operator=(const registry&) = delete;
        registry& operator=(registry&&) noexcept = default;

        [[nodiscard]] entity_type create() {
            size_type id = entities_.size();

            if (!entityFreeList_.empty()) {
                id = entityFreeList_.last();
                entityFreeList_.pop();

                entity_type& entity = entities_[id];

                entity.id_ = id;
                entity.generation_++;

                return entity;
            }
            else {
                entity_type& entity = entities_.emplace();

                entity.id_ = id;
                entity.generation_ = 0;

                return entity;
            }
        }

        [[nodiscard]] bool contains(entity_type entity) const {
            return entities_.size() > entity.id_ && entities_[entity.id_].generation_ == entity.generation_;
        }

        void destroy(entity_type entity) {
            if (!contains(entity)) {
                return;
            }

            entities_[entity.id_].id_ = entity_type::dead_sentinel;

            entityFreeList_.emplace(entity.id_);

            // TODO: remove entity from all sparse sets
            // will be done once component tracking is introduced
        }

        template <typename U, typename... Args>
        U& emplace(entity_type entity, Args&&... args) {
            using sparse_set_type = sparse_set<U, size_type>;

            size_type typeIndex = typeIndexer_.template index<U>();

            if (typeIndex + 1 > sparseSets_.size()) {
                sparseSets_.resize(typeIndex + 1);
                sparseSetDestructors_.resize(typeIndex + 1);

                new (&sparseSets_[typeIndex]) sparse_set<U, size_type>();

                sparseSetDestructors_[typeIndex] = [](void* ptr) {
                    reinterpret_cast<sparse_set_type*>(ptr)->~sparse_set_type();
                };
            }

            auto& sparseSetBytes = sparseSets_[typeIndex];
            auto& sparseSet = *reinterpret_cast<sparse_set_type*>(&sparseSetBytes);

            if (!sparseSet.contains(entity.id_)) {
                sparseSet.insert(entity.id_, forward<Args>(args)...);
            }

            return sparseSet.get(entity.id_);
        }

        template <typename U>
        U& get(entity_type entity) {
            using sparse_set_type = sparse_set<U, size_type>;

            size_type typeIndex = typeIndexer_.template index<U>();

            auto& sparseSetBytes = sparseSets_[typeIndex];
            auto& sparseSet = *reinterpret_cast<sparse_set_type*>(&sparseSetBytes);

            return sparseSet.get(entity.id_);
        }

        template <typename U>
        const U& get(entity_type entity) const {
            using sparse_set_type = sparse_set<U, size_type>;

            size_type typeIndex = typeIndexer_.template index<U>();

            auto& sparseSetBytes = sparseSets_[typeIndex];
            auto& sparseSet = *reinterpret_cast<sparse_set_type*>(&sparseSetBytes);

            return sparseSet.get(entity.id_);
        }

        template <typename U>
        void remove(entity_type entity) {
            using sparse_set_type = sparse_set<U, size_type>;

            size_type typeIndex = typeIndexer_.template index<U>();

            if (typeIndex + 1 > sparseSets_.size()) {
                return;
            }

            auto& sparseSetBytes = sparseSets_[typeIndex];
            auto& sparseSet = *reinterpret_cast<sparse_set_type*>(&sparseSetBytes);

            sparseSet.remove(entity.id_);
        }

    private:
        using sparse_set_filler = filler<sizeof(sparse_set<size_type, size_type>), alignof(sparse_set<size_type, size_type>)>;
        using destructor_function = void (*)(void*);

        list<destructor_function, size_type> sparseSetDestructors_;
        list<sparse_set_filler, size_type> sparseSets_;

        list<size_type, size_type> entityFreeList_;
        list<entity_type, size_type> entities_;

        type_indexer<size_type> typeIndexer_;
    };
}