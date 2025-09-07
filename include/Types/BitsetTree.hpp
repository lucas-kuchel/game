#pragma once

#include <bitset>
#include <cstdint>
#include <optional>
#include <utility>
#include <vector>

namespace Types
{
    template <typename T, std::uint32_t N>
    requires(N > 0)
    class BitsetTree
    {
    public:
        using Type = T;

        static constexpr std::uint32_t BitsetSize = N;
        static constexpr std::uint32_t BlockSize = 256;

        static constexpr std::uint32_t RoundedSize = (BitsetSize + 7) / 8 * 8;
        static constexpr std::uint32_t LevelCount = RoundedSize / 8;

        using Iterator = std::vector<std::pair<std::bitset<BitsetSize>, Type>>::iterator;
        using ConstIterator = std::vector<std::pair<std::bitset<BitsetSize>, Type>>::const_iterator;

        BitsetTree()
        {
            mRoot = mPool.Allocate();
            *mRoot = {};
        }

        ~BitsetTree() = default;

        BitsetTree(const BitsetTree& other)
            : mContiguous(other.mContiguous)
        {
            mRoot = CloneSubtree(other.mRoot, mPool);
        }

        BitsetTree(BitsetTree&& other) noexcept
            : mRoot(other.mRoot), mPool(std::move(other.mPool)), mContiguous(std::move(other.mContiguous))
        {
            other.mRoot = nullptr;
        }

        BitsetTree& operator=(const BitsetTree& other)
        {
            if (this == &other)
            {
                return *this;
            }

            BitsetTree temp(other);

            std::swap(mRoot, temp.mRoot);
            std::swap(mPool, temp.mPool);
            std::swap(mContiguous, temp.mContiguous);

            return *this;
        }

        BitsetTree& operator=(BitsetTree&& other) noexcept
        {
            if (this == &other)
            {
                return *this;
            }

            ClearTree(mRoot);

            mRoot = other.mRoot;
            mPool = std::move(other.mPool);
            mContiguous = std::move(other.mContiguous);

            other.mRoot = nullptr;

            return *this;
        }

        inline void Remove(const std::bitset<BitsetSize>& bitset)
        {
            RemoveRecursive(mRoot, bitset, 0);
        }

        [[nodiscard]] Type& Insert(const std::bitset<BitsetSize>& bitset)
        {
            Node* current = mRoot;

            for (std::uint32_t level = 0; level < LevelCount; level++)
            {
                std::uint8_t key = GetByte(bitset, level);

                Node*& next = current->Children[key];

                if (!next)
                {
                    next = mPool.Allocate();
                    *next = {};
                }

                current = next;
            }

            if (!current->ArchetypeIndex.has_value())
            {
                current->ArchetypeIndex = static_cast<std::uint32_t>(mContiguous.size());
                mContiguous.emplace_back(bitset, T{});
            }

            return mContiguous[current->ArchetypeIndex.value()].second;
        }

        [[nodiscard]] Type& Get(const std::bitset<BitsetSize>& bitset)
        {
            Node* current = mRoot;

            for (std::uint32_t level = 0; level < LevelCount; level++)
            {
                std::uint8_t key = GetByte(bitset, level);
                Node* next = current->Children[key];

                if (!next)
                {
                    throw;
                }

                current = next;
            }

            if (!current->ArchetypeIndex.has_value())
            {
                throw;
            }

            return mContiguous[current->ArchetypeIndex.value()].second;
        }

        [[nodiscard]] const Type& Get(const std::bitset<BitsetSize>& bitset) const
        {
            Node* current = mRoot;

            for (std::uint32_t level = 0; level < LevelCount; level++)
            {
                std::uint8_t key = GetByte(bitset, level);
                Node* next = current->Children[key];

                if (!next)
                {
                    throw;
                }

                current = next;
            }

            if (!current->ArchetypeIndex.has_value())
            {
                throw;
            }

            return mContiguous[current->ArchetypeIndex.value()].second;
        }

        [[nodiscard]] Iterator begin()
        {
            return mContiguous.begin();
        }

        [[nodiscard]] Iterator end()
        {
            return mContiguous.end();
        }

        [[nodiscard]] ConstIterator begin() const
        {
            return mContiguous.begin();
        }

        [[nodiscard]] ConstIterator end() const
        {
            return mContiguous.end();
        }

    private:
        struct Node
        {
            std::array<Node*, 256> Children{};
            std::optional<std::uint32_t> ArchetypeIndex;
        };

        class NodePool
        {
        public:
            NodePool()
                : Index(BlockSize)
            {
            }

            ~NodePool()
            {
                for (auto* block : Blocks)
                {
                    delete[] block;
                }
            }

            NodePool(const NodePool&) = default;
            NodePool(NodePool&&) noexcept = default;

            NodePool& operator=(const NodePool&) = default;
            NodePool& operator=(NodePool&&) noexcept = default;

            [[nodiscard]] Node* Allocate()
            {
                if (!FreeList.empty())
                {
                    Node* node = FreeList.back();
                    FreeList.pop_back();
                    return node;
                }

                if (Index >= BlockSize)
                {
                    AllocateBlock();
                }

                return &Blocks.back()[Index++];
            }

            void Deallocate(Node* ptr)
            {
                if (ptr)
                {
                    FreeList.push_back(ptr);
                }
            }

        private:
            void AllocateBlock()
            {
                Blocks.push_back(new Node[BlockSize]);
                Index = 0;
            }

            std::vector<Node*> Blocks;
            std::vector<Node*> FreeList;

            std::uint32_t Index;
        };

        void ClearTree(Node*& current)
        {
            if (!current)
            {
                return;
            }

            for (auto*& child : current->Children)
            {
                if (child)
                {
                    ClearTree(child);

                    mPool.Deallocate(child);

                    child = nullptr;
                }
            }

            mPool.Deallocate(current);
            current = nullptr;
        }

        [[nodiscard]] static std::uint8_t GetByte(const std::bitset<BitsetSize>& bitset, std::uint32_t byteIndex)
        {
            std::uint8_t result = 0;

            for (std::uint32_t i = 0; i < 8; i++)
            {
                std::uint32_t bitIndex = byteIndex * 8 + i;

                if (bitIndex < BitsetSize && bitset[bitIndex])
                {
                    result |= static_cast<std::uint8_t>(1u << i);
                }
            }

            return result;
        }

        [[nodiscard]] Node* FindNode(const std::bitset<BitsetSize>& bitset) const
        {
            Node* current = mRoot;

            for (std::uint32_t level = 0; level < LevelCount; level++)
            {
                std::uint8_t key = GetByte(bitset, level);
                Node* next = current->Children[key];

                if (!next)
                {
                    return nullptr;
                }

                current = next;
            }
            return current;
        }

        bool RemoveRecursive(Node*& current, const std::bitset<BitsetSize>& bitset, std::uint32_t level)
        {
            if (!current)
            {
                return false;
            }

            if (level == LevelCount)
            {
                if (current->ArchetypeIndex.has_value())
                {
                    std::uint32_t removedIndex = current->ArchetypeIndex.value();
                    current->ArchetypeIndex.reset();

                    if (!mContiguous.empty())
                    {
                        std::uint32_t lastIndex = static_cast<std::uint32_t>(mContiguous.size() - 1);

                        if (removedIndex != lastIndex)
                        {
                            auto movedPair = std::move(mContiguous[lastIndex]);

                            mContiguous[removedIndex] = std::move(movedPair);

                            Node* movedNode = FindNode(mContiguous[removedIndex].first);

                            if (movedNode)
                            {
                                movedNode->ArchetypeIndex = removedIndex;
                            }
                        }
                        mContiguous.pop_back();
                    }
                }
            }
            else
            {
                std::uint8_t key = GetByte(bitset, level);
                Node*& next = current->Children[key];

                if (RemoveRecursive(next, bitset, level + 1))
                {
                    mPool.Deallocate(next);
                    next = nullptr;
                }
            }

            if (current->ArchetypeIndex.has_value())
            {
                return false;
            }

            for (auto* child : current->Children)
            {
                if (child)
                {
                    return false;
                }
            }

            return true;
        }

        [[nodiscard]] Node* CloneSubtree(const Node* source, NodePool& pool)
        {
            if (!source)
            {
                return nullptr;
            }

            Node* clone = pool.Allocate();

            clone->ArchetypeIndex = source->ArchetypeIndex;

            for (std::uint32_t i = 0; i < 256; i++)
            {
                if (source->Children[i])
                {
                    clone->Children[i] = CloneSubtree(source->Children[i], pool);
                }
            }

            return clone;
        }

        Node* mRoot;
        NodePool mPool;

        std::vector<std::pair<std::bitset<BitsetSize>, Type>> mContiguous;
    };
}