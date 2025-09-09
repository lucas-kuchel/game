#pragma once

#include <Types/BitsetTree.hpp>
#include <Types/SparseSet.hpp>

#include <Debug/Exception.hpp>

#include <Resources/Entity.hpp>

#include <bitset>
#include <limits>
#include <tuple>
#include <vector>

namespace Systems
{
    template <typename T>
    inline constexpr bool CanBeComponent = !std::is_pointer_v<T> &&
                                           !std::is_reference_v<T> &&
                                           !std::is_const_v<T> &&
                                           !std::is_volatile_v<T> &&
                                           !std::is_array_v<T> &&
                                           !std::is_function_v<T> &&
                                           !std::is_void_v<T> &&
                                           !std::is_same_v<T, std::nullptr_t>;

    template <typename... Ts>
    inline constexpr bool CanBeComponents = (CanBeComponent<Ts> && ...);

    template <typename...>
    inline constexpr bool ComponentsAreUnique = true;

    template <typename T, typename... Us>
    inline constexpr bool ComponentsAreUnique<T, Us...> = (!std::is_same_v<T, Us> && ...) && ComponentsAreUnique<Us...>;

    template <typename... Ts>
    requires(ComponentsAreUnique<Ts...> && CanBeComponents<Ts...> && sizeof...(Ts) > 0)
    class RegistryDescriptor
    {
    public:
        RegistryDescriptor() = delete;

        template <typename T>
        static constexpr bool Contains = (std::is_same_v<Ts, T> || ...);

        template <typename T>
        requires(Contains<T>)
        static constexpr std::size_t Index()
        {
            constexpr std::array<bool, sizeof...(Ts)> matchFlags = {std::is_same_v<T, Ts>...};

            for (std::size_t i = 0; i < matchFlags.size(); i++)
            {
                if (matchFlags[i])
                {
                    return i;
                }
            }

            return std::numeric_limits<std::size_t>::max();
        }
    };

    template <typename>
    inline constexpr bool IsRegistryDescriptor = false;

    template <typename... Ts>
    inline constexpr bool IsRegistryDescriptor<RegistryDescriptor<Ts...>> = true;

    template <typename T>
    requires(IsRegistryDescriptor<T>)
    class Registry;

    template <typename>
    inline constexpr bool IsRegistry = false;

    template <typename D>
    inline constexpr bool IsRegistry<Registry<D>> = true;

    template <typename R, typename... Ts>
    requires(IsRegistry<R> && ComponentsAreUnique<Ts...> && (R::DescriptorType::template Contains<Ts> && ...))
    class EntityView
    {
    public:
        using SizeType = std::size_t;

        class Iterator
        {
        public:
            Iterator(R* registry, Types::SparseSet<Resources::EntityHandle>* entities, SizeType index, SizeType end)
                : mRegistry(registry), mEntities(entities), mIndex(index), mEnd(end)
            {
            }

            auto operator*() const
            {
                Resources::EntityHandle targetEntity = mEntities->GetDense()[mIndex];

                return std::tuple<Resources::EntityHandle, Ts&...>(targetEntity, mRegistry->template GetSparseSet<Ts>().GetDense()[mRegistry->template GetSparseSet<Ts>().GetSparse()[targetEntity.ID]]...);
            }

            bool operator!=(const EntityView::Iterator&) const
            {
                return mIndex < mEnd;
            }

            EntityView::Iterator& operator++()
            {
                ++mIndex;

                return *this;
            }

        private:
            R* mRegistry;

            Types::SparseSet<Resources::EntityHandle>* mEntities;

            SizeType mIndex;
            SizeType mEnd;
        };

        using ConstIterator = const Iterator;

        EntityView(R* ecs, Types::SparseSet<Resources::EntityHandle>& entities)
            : mRegistry(ecs), mEnd(entities.GetDense().size()), mEntities(&entities)
        {
        }

        [[nodiscard]] Iterator begin()
        {
            return EntityView::Iterator(mRegistry, mEntities, 0, mEnd);
        }

        [[nodiscard]] Iterator end()
        {
            return EntityView::Iterator(mRegistry, mEntities, mEnd, mEnd);
        }

        [[nodiscard]] ConstIterator cbegin() const
        {
            return EntityView::Iterator(mRegistry, mEntities, 0, mEnd);
        }

        [[nodiscard]] ConstIterator cend() const
        {
            return EntityView::Iterator(mRegistry, mEntities, mEnd, mEnd);
        }

    private:
        R* mRegistry;

        SizeType mEnd;

        Types::SparseSet<Resources::EntityHandle>* mEntities;
    };

    template <typename... Ts>
    class Registry<RegistryDescriptor<Ts...>>
    {
    public:
        using BitsetType = std::bitset<sizeof...(Ts)>;
        using ArchetypeType = Types::SparseSet<Resources::EntityHandle>;
        using DescriptorType = RegistryDescriptor<Ts...>;

        Registry() = default;
        ~Registry() = default;

        Registry(const Registry&) = delete;
        Registry(Registry&&) noexcept = delete;

        Registry& operator=(const Registry&) = delete;
        Registry& operator=(Registry&&) noexcept = delete;

        [[nodiscard]] inline Resources::EntityHandle CreateBlankEntity()
        {
            if (mFreeList.empty())
            {
                std::size_t size = mEntities.size();

                mEntities.emplace_back(size, 0);
                mEntityMasks.emplace_back();

                return mEntities.back();
            }
            else
            {
                std::size_t index = mFreeList.back();
                Resources::EntityHandle& entity = mEntities[index];
                BitsetType& bitset = mEntityMasks[index];

                entity.Generation++;
                mFreeList.pop_back();
                bitset.reset();

                return entity;
            }
        }

        template <typename... TQueried>
        requires((RegistryDescriptor<Ts...>::template Contains<TQueried> && ...) && (sizeof...(TQueried) != 0))
        inline Resources::EntityHandle CreateEntity()
        {
            Resources::EntityHandle entity = CreateBlankEntity();
            std::size_t id = entity.ID;
            BitsetType mask = MakeBitmask<TQueried...>();

            mEntityMasks[id] = mask;

            ArchetypeType& archetype = mArchetypes.Insert(mask);

            archetype.Insert(id, entity);

            (AddEntityToSparseSet<TQueried>(entity, TQueried()), ...);

            return entity;
        }

        template <typename... TQueried>
        requires((RegistryDescriptor<Ts...>::template Contains<TQueried> && ...) && (sizeof...(TQueried) != 0))
        inline Resources::EntityHandle CreateEntity(TQueried&&... components)
        {
            Resources::EntityHandle entity = CreateBlankEntity();
            std::size_t id = entity.ID;
            BitsetType mask = MakeBitmask<TQueried...>();

            mEntityMasks[id] = mask;

            ArchetypeType& archetype = mArchetypes.Insert(mask);

            archetype.Insert(id, entity);

            (AddEntityToSparseSet<TQueried>(entity, std::forward<TQueried>(components)), ...);

            return entity;
        }

        inline void DestroyEntity(Resources::EntityHandle entity)
        {
            if (HasEntity(entity))
            {
                std::size_t id = entity.ID;
                std::size_t generation = entity.Generation;

                const BitsetType& bitset = mEntityMasks[id];
                ArchetypeType& archetype = mArchetypes.Get(bitset);

                RemoveEntityFromSparseSets(entity, bitset);

                auto& dense = archetype.GetDense();

                archetype.Remove(id);

                if (dense.size() == 0)
                {
                    mArchetypes.Remove(bitset);
                }

                mFreeList.push_back(id);
                mEntities[id] = {std::numeric_limits<std::size_t>::max(), generation};
                mEntityMasks[id].reset();
            }
            else
            {
                throw Debug::Exception(Debug::ErrorCode::OUT_OF_RANGE, "void Systems::Registry<D>::DestroyEntity(Resources::EntityHandle):\n"
                                                                       "out of range error\n"
                                                                       "entity does not exist and cannot be destroyed (ID = {}, Generation = {})",
                                       entity.ID, entity.Generation);
            }
        }

        [[nodiscard]] inline bool HasEntity(Resources::EntityHandle entity) const
        {
            std::size_t& id = entity.ID;
            std::size_t& generation = entity.Generation;

            return id < mEntities.size() && mEntities[id].Generation == generation && mEntities[id].ID != std::numeric_limits<std::size_t>::max();
        }

        template <typename U>
        requires(RegistryDescriptor<Ts...>::template Contains<U>)
        [[nodiscard]] inline bool EntityHasComponent(Resources::EntityHandle entity) const
        {
            std::size_t& id = entity.ID;

            if (HasEntity(entity))
            {
                constexpr std::size_t index = RegistryDescriptor<Ts...>::template Index<U>();

                return mEntityMasks[id].test(index);
            }

            return false;
        }

        template <typename... TQueried>
        requires((RegistryDescriptor<Ts...>::template Contains<TQueried> && ...) && sizeof...(TQueried) != 0)
        [[nodiscard]] inline bool EntityHasComponents(Resources::EntityHandle entity) const
        {
            return (EntityHasComponent<TQueried>(entity) && ...);
        }

        template <typename TComponent>
        requires(RegistryDescriptor<Ts...>::template Contains<TComponent>)
        inline void AddComponentToEntity(Resources::EntityHandle entity, TComponent&& component)
        {
            if (HasEntity(entity) && !EntityHasComponent<TComponent>(entity))
            {
                std::size_t& id = entity.ID;

                auto& set = std::get<Types::SparseSet<TComponent>>(mSparseSets);

                constexpr std::size_t index = RegistryDescriptor<Ts...>::template Index<TComponent>();

                BitsetType& targetBitset = mEntityMasks[id];
                BitsetType oldBitset = mEntityMasks[id];

                targetBitset.set(index);

                bool archetypeResult = UpdateArchetype(entity, oldBitset, targetBitset);

                if (archetypeResult)
                {
                    set.Insert(id, std::forward<TComponent>(component));
                }
            }
            else
            {
                throw Debug::Exception(Debug::ErrorCode::OUT_OF_RANGE, "void Systems::Registry<D>::AddComponentToEntity<T>(Resources::EntityHandle, TComponent&&):\n"
                                                                       "out of range error\n"
                                                                       "entity does not exist and cannot have components added to it (ID = {}, Generation = {})",
                                       entity.ID, entity.Generation);
            }
        }

        template <typename... TQueried>
        requires((RegistryDescriptor<Ts...>::template Contains<TQueried> && ...) && sizeof...(TQueried) != 0)
        inline void AddComponentsToEntity(Resources::EntityHandle entity, TQueried&&... components)
        {
            (AddComponentToEntity<TQueried>(entity, std::forward<TQueried>(components)), ...);
        }

        template <typename TComponent>
        requires(RegistryDescriptor<Ts...>::template Contains<TComponent>)
        inline void RemoveComponentFromEntity(Resources::EntityHandle entity)
        {
            if (HasEntity(entity))
            {
                auto& set = std::get<Types::SparseSet<TComponent>>(mSparseSets);

                std::size_t& id = entity.ID;

                constexpr std::size_t index = RegistryDescriptor<Ts...>::template Index<TComponent>();

                BitsetType& targetBitset = mEntityMasks[id];
                BitsetType oldBitset = mEntityMasks[id];

                targetBitset.reset(index);

                UpdateArchetype(entity, oldBitset, targetBitset);

                set.Remove(id);
            }
            else
            {
                throw Debug::Exception(Debug::ErrorCode::OUT_OF_RANGE, "void Systems::Registry<D>::RemoveComponentFromEntity<T>(Resources::EntityHandle):\n"
                                                                       "out of range error\n"
                                                                       "entity does not exist and cannot have components removed from it (ID = {}, Generation = {})",
                                       entity.ID, entity.Generation);
            }
        }

        template <typename... TQueried>
        requires((RegistryDescriptor<Ts...>::template Contains<TQueried> && ...) && sizeof...(TQueried) != 0)
        inline void RemoveComponentsFromEntity(Resources::EntityHandle entity)
        {
            return (RemoveComponentFromEntity<TQueried>(entity), ...);
        }

        [[nodiscard]] inline Types::BitsetTree<ArchetypeType, sizeof...(Ts)>& GetArchetypes()
        {
            return mArchetypes;
        }

        [[nodiscard]] inline const Types::BitsetTree<ArchetypeType, sizeof...(Ts)>& GetArchetypes() const
        {
            return mArchetypes;
        }

        [[nodiscard]] ArchetypeType& GetArchetype(const BitsetType& mask)
        {
            return mArchetypes.Get(mask);
        }

        [[nodiscard]] const ArchetypeType& GetArchetype(const BitsetType& mask) const
        {
            return mArchetypes.Get(mask);
        }

        template <typename TComponent>
        requires(RegistryDescriptor<Ts...>::template Contains<TComponent>)
        [[nodiscard]] inline constexpr Types::SparseSet<TComponent>& GetSparseSet()
        {
            return std::get<Types::SparseSet<TComponent>>(mSparseSets);
        }

        template <typename TComponent>
        requires(RegistryDescriptor<Ts...>::template Contains<TComponent>)
        [[nodiscard]] inline constexpr const Types::SparseSet<TComponent>& GetSparseSet() const
        {
            return std::get<Types::SparseSet<TComponent>>(mSparseSets);
        }

        template <typename... TQueried>
        requires((RegistryDescriptor<Ts...>::template Contains<TQueried> && ...) && sizeof...(TQueried) != 0)
        [[nodiscard]] static inline constexpr BitsetType MakeBitmask()
        {
            BitsetType bitset;

            (bitset.set(RegistryDescriptor<Ts...>::template Index<TQueried>()), ...);

            return bitset;
        }

        template <typename... TQueried>
        requires(RegistryDescriptor<Ts...>::template Contains<TQueried> && ...)
        [[nodiscard]] inline auto GetEntityView(ArchetypeType& archetype)
        {
            return EntityView<Registry<RegistryDescriptor<Ts...>>, TQueried...>(this, archetype);
        }

        template <typename... TQueried>
        requires(RegistryDescriptor<Ts...>::template Contains<TQueried> && ...)
        [[nodiscard]] inline auto GetEntityView(ArchetypeType& archetype) const
        {
            return EntityView<Registry<RegistryDescriptor<Ts...>>, TQueried...>(this, archetype);
        }

    private:
        [[nodiscard]] inline bool UpdateArchetype(Resources::EntityHandle entity, const BitsetType& oldBitset, const BitsetType& newBitset)
        {
            if (oldBitset == newBitset)
            {
                return false;
            }

            auto& oldArchetype = mArchetypes.Get(oldBitset);

            if (!oldArchetype.Remove(entity.ID))
            {
                return false;
            }

            auto& dense = oldArchetype.GetDense();

            if (dense.size() == 0)
            {
                mArchetypes.Remove(oldBitset);
            }

            ArchetypeType& archetype = mArchetypes.Insert(newBitset);
            auto& id = entity.ID;
            archetype.Insert(id, entity);

            return true;
        }

        template <std::size_t... Ns>
        inline void RemoveEntityFromSparseSetsImplementation(Resources::EntityHandle entity, const BitsetType& mask, std::index_sequence<Ns...>)
        {
            ((mask.test(Ns) ? (void)std::get<Ns>(mSparseSets).Remove(entity.ID) : (void)0), ...);
        }

        inline void RemoveEntityFromSparseSets(Resources::EntityHandle entity, const BitsetType& mask)
        {
            RemoveEntityFromSparseSetsImplementation(entity, mask, std::index_sequence_for<Ts...>{});
        }

        template <typename T, typename U>
        inline void AddEntityToSparseSet(Resources::EntityHandle& entity, U&& component)
        {
            Types::SparseSet<T>& sparseSet = std::get<Types::SparseSet<T>>(mSparseSets);
            sparseSet.Insert(entity.ID, std::forward<U>(component));
        }

        std::tuple<Types::SparseSet<Ts>...> mSparseSets;

        Types::BitsetTree<ArchetypeType, sizeof...(Ts)> mArchetypes;

        std::vector<BitsetType> mEntityMasks;
        std::vector<Resources::EntityHandle> mEntities;
        std::vector<std::size_t> mFreeList;
    };
}