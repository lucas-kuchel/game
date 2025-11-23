#pragma once

#include <engine/traits.hpp>

#include <cmath>
#include <cstdlib>

namespace engine {
    template <typename T, std::size_t N>
    requires(IsFloatingType<T> && IsInRange<std::size_t, 2, 4, N>)
    struct Vector;

    template <typename T>
    struct Vector<T, 2> {
    public:
        Vector(T x, T y)
            : x(x), y(y) {
        }

        Vector(T i = 0.0)
            : x(i), y(i) {
        }

        [[nodiscard]] T& operator[](std::size_t index) noexcept {
            return reinterpret_cast<T*>(this)[index];
        }

        [[nodiscard]] const T& operator[](std::size_t index) const noexcept {
            return reinterpret_cast<const T*>(this)[index];
        }

        [[nodiscard]] Vector operator+(const Vector& other) const noexcept {
            return {
                .x = x + other.x,
                .y = y + other.y,
            };
        }

        [[nodiscard]] Vector operator-(const Vector& other) const noexcept {
            return {
                .x = x - other.x,
                .y = y - other.y,
            };
        }

        [[nodiscard]] Vector operator*(const Vector& other) const noexcept {
            return {
                .x = x * other.x,
                .y = y * other.y,
            };
        }

        [[nodiscard]] Vector operator/(const Vector& other) const noexcept {
            return {
                .x = x / other.x,
                .y = y / other.y,
            };
        }

        [[nodiscard]] Vector operator+(const T& other) const noexcept {
            return {
                .x = x + other,
                .y = y + other,
            };
        }

        [[nodiscard]] Vector operator-(const T& other) const noexcept {
            return {
                .x = x - other,
                .y = y - other,
            };
        }

        [[nodiscard]] Vector operator*(const T& other) const noexcept {
            return {
                .x = x * other,
                .y = y * other,
            };
        }

        [[nodiscard]] Vector operator/(const T& other) const noexcept {
            return {
                .x = x / other,
                .y = y / other,
            };
        }

        [[nodiscard]] static inline T dot(const Vector<T, 2>& one, const Vector<T, 2>& two) {
            return (one.x * two.x) + (one.y * two.y);
        }

        [[nodiscard]] static inline T magnitude(const Vector<T, 2>& vec) noexcept {
            return std::sqrt((vec.x * vec.x) + (vec.y * vec.y));
        }

        [[nodiscard]] static inline T length(const Vector<T, 2>& vec) noexcept {
            return std::sqrt((vec.x * vec.x) + (vec.y * vec.y));
        }

        T x;
        T y;
    };

    template <typename T>
    struct Vector<T, 3> {
    public:
        Vector(T x, T y, T z)
            : x(x), y(y), z(z) {
        }

        Vector(T i = 0.0)
            : x(i), y(i), z(i) {
        }

        [[nodiscard]] T& operator[](std::size_t index) noexcept {
            return reinterpret_cast<T*>(this)[index];
        }

        [[nodiscard]] const T& operator[](std::size_t index) const noexcept {
            return reinterpret_cast<const T*>(this)[index];
        }

        [[nodiscard]] Vector operator+(const Vector& other) const noexcept {
            return {
                .x = x + other.x,
                .y = y + other.y,
                .z = z + other.z,
            };
        }

        [[nodiscard]] Vector operator-(const Vector& other) const noexcept {
            return {
                .x = x - other.x,
                .y = y - other.y,
                .z = z - other.z,
            };
        }

        [[nodiscard]] Vector operator*(const Vector& other) const noexcept {
            return {
                .x = x * other.x,
                .y = y * other.y,
                .z = z * other.z,
            };
        }

        [[nodiscard]] Vector operator/(const Vector& other) const noexcept {
            return {
                .x = x / other.x,
                .y = y / other.y,
                .z = z / other.z,
            };
        }

        [[nodiscard]] Vector operator+(const T& other) const noexcept {
            return {
                .x = x + other,
                .y = y + other,
                .z = z + other,
            };
        }

        [[nodiscard]] Vector operator-(const T& other) const noexcept {
            return {
                .x = x - other,
                .y = y - other,
                .z = z - other,
            };
        }

        [[nodiscard]] Vector operator*(const T& other) const noexcept {
            return {
                .x = x * other,
                .y = y * other,
                .z = z * other,
            };
        }

        [[nodiscard]] Vector operator/(const T& other) const noexcept {
            return {
                .x = x / other,
                .y = y / other,
                .z = z / other,
            };
        }

        [[nodiscard]] static inline T dot(const Vector<T, 3>& one, const Vector<T, 3>& two) noexcept {
            return (one.x * two.x) + (one.y * two.y) + (one.z * two.z);
        }

        [[nodiscard]] static inline T magnitude(const Vector<T, 3>& vec) noexcept {
            return std::sqrt((vec.x * vec.x) + (vec.y * vec.y) + (vec.z * vec.z));
        }

        [[nodiscard]] static inline T length(const Vector<T, 3>& vec) noexcept {
            return std::sqrt((vec.x * vec.x) + (vec.y * vec.y) + (vec.z * vec.z));
        }

        [[nodiscard]] static inline Vector<T, 3> cross(const Vector<T, 3>& one, const Vector<T, 3>& two) noexcept {
            return Vector<T, 3>{
                .x = (one.x * two.z) - (one.z * two.y),
                .y = (one.z * two.x) - (one.x * two.z),
                .z = (one.x * two.y) - (one.y * two.x),
            };
        }

        T x;
        T y;
        T z;
    };

    template <typename T>
    struct Vector<T, 4> {
    public:
        Vector(T x, T y, T z, T w)
            : x(x), y(y), z(z), w(w) {
        }

        Vector(T i = 0.0)
            : x(i), y(i), z(i), w(i) {
        }

        [[nodiscard]] T& operator[](std::size_t index) noexcept {
            return reinterpret_cast<T*>(this)[index];
        }

        [[nodiscard]] const T& operator[](std::size_t index) const noexcept {
            return reinterpret_cast<const T*>(this)[index];
        }

        [[nodiscard]] Vector operator+(const Vector& other) const noexcept {
            return {
                .x = x + other.x,
                .y = y + other.y,
                .z = z + other.z,
                .w = w + other.w,
            };
        }

        [[nodiscard]] Vector operator-(const Vector& other) const noexcept {
            return {
                .x = x - other.x,
                .y = y - other.y,
                .z = z - other.z,
                .w = w - other.z,
            };
        }

        [[nodiscard]] Vector operator*(const Vector& other) const noexcept {
            return {
                .x = x * other.x,
                .y = y * other.y,
                .z = z * other.z,
                .w = w * other.z,
            };
        }

        [[nodiscard]] Vector operator/(const Vector& other) const noexcept {
            return {
                .x = x / other.x,
                .y = y / other.y,
                .z = z / other.z,
                .w = w / other.z,
            };
        }

        [[nodiscard]] Vector operator+(const T& other) const noexcept {
            return {
                .x = x + other,
                .y = y + other,
                .z = z + other,
                .w = w + other,
            };
        }

        [[nodiscard]] Vector operator-(const T& other) const noexcept {
            return {
                .x = x - other,
                .y = y - other,
                .z = z - other,
                .w = w - other,
            };
        }

        [[nodiscard]] Vector operator*(const T& other) const noexcept {
            return {
                .x = x * other,
                .y = y * other,
                .z = z * other,
                .w = w * other,
            };
        }

        [[nodiscard]] Vector operator/(const T& other) const noexcept {
            return {
                .x = x / other,
                .y = y / other,
                .z = z / other,
                .w = w / other,
            };
        }

        [[nodiscard]] static inline T dot(const Vector<T, 4>& one, const Vector<T, 4>& two) noexcept {
            return (one.x * two.x) + (one.y * two.y) + (one.z * two.z) + (one.w * two.w);
        }

        [[nodiscard]] static inline T magnitude(const Vector<T, 4>& vec) noexcept {
            return std::sqrt((vec.x * vec.x) + (vec.y * vec.y) + (vec.z * vec.z) + (vec.w * vec.w));
        }

        [[nodiscard]] static inline T length(const Vector<T, 4>& vec) noexcept {
            return std::sqrt((vec.x * vec.x) + (vec.y * vec.y) + (vec.z * vec.z) + (vec.w * vec.w));
        }

        T x;
        T y;
        T z;
        T w;
    };

    template <typename T>
    using Vector2 = Vector<T, 2>;

    template <typename T>
    using Vector3 = Vector<T, 3>;

    template <typename T>
    using Vector4 = Vector<T, 4>;

    using Vector2f = Vector2<float>;
    using Vector3f = Vector3<float>;
    using Vector4f = Vector4<float>;

    using Vector2d = Vector2<double>;
    using Vector3d = Vector3<double>;
    using Vector4d = Vector4<double>;
}