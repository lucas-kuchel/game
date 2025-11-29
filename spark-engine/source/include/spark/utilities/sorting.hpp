#pragma once

#include <random>

#include <spark/types/core.hpp>

#include <spark/utilities/values.hpp>

namespace spark {
    // @brief simple comparison-based sorting algorithm with O(n^2) time complexity
    // @note stable, in-place, and best suited for very small datasets
    struct bubble_sort {
        template <typename T, typename C>
        static void sort(T* data, spark::uint64 size) {
            if (size < 2) {
                return;
            }

            for (spark::uint64 i = 0; i < size; i++) {
                for (spark::uint64 j = 0; j + 1 < size - i; j++) {
                    if (C::compare(data[j + 1], data[j])) {
                        T temporary = move(data[j]);

                        data[j] = move(data[j + 1]);
                        data[j + 1] = move(temporary);
                    }
                }
            }
        }
    };

    // @brief sorting algorithm that repeatedly selects the smallest remaining element
    // @note in-place, not stable, always O(n^2) regardless of input order
    struct selection_sort {
        template <typename T, typename C>
        static void sort(T* data, spark::uint64 size) {
            if (size < 2) {
                return;
            }

            for (spark::uint64 i = 0; i < size; i++) {
                spark::uint64 minIndex = i;

                for (spark::uint64 j = i + 1; j < size; j++) {
                    if (C::compare(data[j], data[minIndex])) {
                        minIndex = j;
                    }
                }
                if (i != minIndex) {
                    T temporary = move(data[i]);

                    data[i] = move(data[minIndex]);
                    data[minIndex] = move(temporary);
                }
            }
        }
    };

    // @brief efficient insertion-based sorting algorithm for small or nearly-sorted ranges
    // @note stable, in-place, O(n) best case, O(n^2) worst case
    struct insertion_sort {
        template <typename T, typename C>
        static void sort(T* data, spark::uint64 size) {
            if (size < 2) {
                return;
            }

            for (spark::uint64 i = 1; i < size; i++) {
                T key = move(data[i]);

                spark::uint64 j = i;

                while (j > 0 && C::compare(key, data[j - 1])) {
                    data[j] = move(data[j - 1]);
                    --j;
                }

                data[j] = move(key);
            }
        }
    };

    // @brief bidirectional variant of bubble sort that sweeps from both ends
    // @note in-place, stable, slightly faster than bubble sort on some datasets
    struct cocktail_sort {
        template <typename T, typename C>
        static void sort(T* data, spark::uint64 size) {
            if (size < 2) {
                return;
            }

            bool swapped = true;
            spark::uint64 start = 0;
            spark::uint64 end = size - 1;

            while (swapped) {
                swapped = false;

                for (spark::uint64 i = start; i < end; i++) {
                    if (C::compare(data[i + 1], data[i])) {
                        T temporary = move(data[i]);
                        data[i] = move(data[i + 1]);
                        data[i + 1] = move(temporary);
                        swapped = true;
                    }
                }

                if (!swapped) {
                    break;
                }

                swapped = false;
                --end;

                for (spark::uint64 i = end; i > start; i--) {
                    if (C::compare(data[i], data[i - 1])) {
                        T temporary = move(data[i]);
                        data[i] = move(data[i - 1]);
                        data[i - 1] = move(temporary);
                        swapped = true;
                    }
                }

                ++start;
            }
        }
    };

    // @brief sorting algorithm that randomly shuffles elements until sorted
    // @note please don't use this in critical code, this purely exists as a joke
    struct bogo_sort {
        template <typename T, typename C>
        static void sort(T* data, spark::uint64 size) {
            if (size < 2) {
                return;
            }

            auto sorted = [&]() {
                for (spark::uint64 i = 1; i < size; i++) {
                    if (C::compare(data[i], data[i - 1])) {
                        return false;
                    }
                }
                return true;
            };

            std::mt19937_64 rng{std::random_device{}()};

            while (!sorted()) {
                for (spark::uint64 i = size - 1; i > 0; i--) {
                    std::uniform_int_distribution<spark::uint64> dist(0, i);
                    spark::uint64 j = dist(rng);

                    T temporary = spark::move(data[i]);
                    data[i] = spark::move(data[j]);
                    data[j] = spark::move(temporary);
                }
            }
        }
    };

    // @brief comparator for less-than comparisons
    class less {
    public:
        template <typename T>
        static bool compare(T& a, T& b) {
            return a < b;
        }
    };

    // @brief comparator for greater-than comparisons
    class greater {
    public:
        template <typename T>
        static bool compare(T& a, T& b) {
            return a > b;
        }
    };

    // @brief comparator for less-or-equal comparisons
    class less_equal {
    public:
        template <typename T>
        static bool compare(T& a, T& b) {
            return a <= b;
        }
    };

    // @brief comparator for greater-or-equal comparisons
    class greater_equal {
    public:
        template <typename T>
        static bool compare(T& a, T& b) {
            return a >= b;
        }
    };
}