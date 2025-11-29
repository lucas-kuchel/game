#pragma once

#include <spark/types/core.hpp>
#include <spark/types/list.hpp>
#include <spark/types/traits.hpp>

namespace spark {
    class bitset {
    public:
        using block_type = uint64;

        static constexpr uint64 block_size = sizeof(block_type) * 8;

        bitset() = default;

        explicit bitset(uint64 numBits) {
            resize(numBits);
        }

        void set(uint64 index, bool value) {
            ensureCapacity(index);

            block_type mask = block_type(1) << (index % block_size);

            if (value) {
                blocks_[index / block_size] |= mask;
            }
            else {
                blocks_[index / block_size] &= ~mask;
            }
        }

        void toggle(uint64 index) {
            ensureCapacity(index);

            blocks_[index / block_size] ^= block_type(1) << (index % block_size);
        }

        bool test(uint64 index) const {
            if (index / block_size >= blocks_.size()) {
                return false;
            }

            return (blocks_[index / block_size] >> (index % block_size)) & 1;
        }

        void resize(uint64 numBits) {
            uint64 neededBlocks = (numBits + block_size - 1) / block_size;

            if (neededBlocks > blocks_.size()) {
                blocks_.resize(neededBlocks, 0);
            }
        }

        uint64 size() const {
            return blocks_.size() * block_size;
        }

        bitset& operator|=(const bitset& other) {
            uint64 maxBlocks = max(blocks_.size(), other.blocks_.size());
            resize(maxBlocks * block_size);

            for (uint64 i = 0; i < other.blocks_.size(); i++) {
                blocks_[i] |= other.blocks_[i];
            }

            return *this;
        }

        bitset operator|(const bitset& other) const {
            bitset result = *this;
            result |= other;

            return result;
        }

        bitset& operator&=(const bitset& other) {
            uint64 minBlocks = min(blocks_.size(), other.blocks_.size());
            for (uint64 i = 0; i < minBlocks; i++) {
                blocks_[i] &= other.blocks_[i];
            }

            for (uint64 i = minBlocks; i < blocks_.size(); i++) {
                blocks_[i] = 0;
            }

            return *this;
        }

        bitset operator&(const bitset& other) const {
            bitset result = *this;
            result &= other;

            return result;
        }

        bitset& operator^=(const bitset& other) {
            uint64 maxBlocks = max(blocks_.size(), other.blocks_.size());
            resize(maxBlocks * block_size);

            for (uint64 i = 0; i < other.blocks_.size(); i++) {
                blocks_[i] ^= other.blocks_[i];
            }

            return *this;
        }

        bitset operator^(const bitset& other) const {
            bitset result = *this;
            result ^= other;

            return result;
        }

        bitset operator~() const {
            bitset result = *this;
            for (uint64 i = 0; i < result.blocks_.size(); i++) {
                result.blocks_[i] = ~result.blocks_[i];
            }

            return result;
        }

        bool operator==(const bitset& other) const {
            uint64 maxBlocks = max(blocks_.size(), other.blocks_.size());

            for (uint64 i = 0; i < maxBlocks; i++) {
                uint64 a = (i < blocks_.size()) ? blocks_[i] : 0;
                uint64 b = (i < other.blocks_.size()) ? other.blocks_[i] : 0;

                if (a != b) {
                    return false;
                }
            }

            return true;
        }

        bool operator!=(const bitset& other) const {
            return !(*this == other);
        }

    private:
        list<block_type> blocks_;

        void ensureCapacity(uint64 bitIndex) {
            uint64 requiredBlocks = (bitIndex / block_size) + 1;

            if (requiredBlocks > blocks_.size()) {
                blocks_.resize(requiredBlocks, 0);
            }
        }
    };
}