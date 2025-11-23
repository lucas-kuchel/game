#pragma once

#include "vector.hpp"

#include <cstdlib>

namespace engine {
    template <typename T, std::size_t C, std::size_t R>
    requires(IsFloatingType<T> && IsInRange<std::size_t, 2, 4, C> && IsInRange<std::size_t, 2, 4, R>)
    class Matrix;

    template <typename T>
    class Matrix<T, 2, 2> {
    public:
        Matrix() = default;

        template <typename... Args>
        requires((SameTypes<Args, Vector2<T>> && ...) && sizeof...(Args) == 2)
        Matrix(const Args&... columns)
            : data(columns...) {
        }

        [[nodiscard]] static constexpr std::size_t columns() noexcept {
            return 2;
        }

        [[nodiscard]] static constexpr std::size_t rows() noexcept {
            return 2;
        }

        Vector2<T>& operator[](std::size_t index) {
            return data[index];
        }

        const Vector2<T>& operator[](std::size_t index) const {
            return data[index];
        }

        [[nodiscard]] auto operator*(const Matrix<T, 2, 2>& other) const -> Matrix<T, 2, 2> {
            const Matrix<T, 2, 2> rows = transpose(*this);

            return {
                Vector2<T>{Vector2<T>::dot(rows[0], other[0]), Vector2<T>::dot(rows[1], other[0])},
                Vector2<T>{Vector2<T>::dot(rows[0], other[1]), Vector2<T>::dot(rows[1], other[1])},
            };
        }

        [[nodiscard]] auto operator*(const Vector2<T>& other) const -> Vector2<T> {
            const Matrix<T, 2, 2> rows = transpose(*this);

            return Vector2<T>{Vector2<T>::dot(rows[0], other), Vector2<T>::dot(rows[1], other)};
        }

        [[nodiscard]] static inline Matrix<T, 2, 2> identity() noexcept {
            return {
                Vector2<T>{1.0, 0.0},
                Vector2<T>{0.0, 1.0},
            };
        }

        [[nodiscard]] static inline Matrix<T, 2, 2> transpose(const Matrix<T, 2, 2>& mat) noexcept {
            return {
                Vector2<T>{mat[0][0], mat[1][0]},
                Vector2<T>{mat[0][1], mat[1][1]},
            };
        }

        [[nodiscard]] static inline Matrix<T, 2, 2> scaling(const Vector2<T>& scaling) {
            return {
                Vector2<T>{scaling.x, 0.0},
                Vector2<T>{0.0, scaling.y},
            };
        }

    private:
        Vector2<T> data[2];
    };

    template <typename T>
    class Matrix<T, 3, 3> {
    public:
        Matrix() = default;

        template <typename... Args>
        requires((SameTypes<Args, Vector3<T>> && ...) && sizeof...(Args) == 3)
        Matrix(const Args&... columns)
            : data(columns...) {
        }

        [[nodiscard]] static constexpr std::size_t columns() noexcept {
            return 3;
        }

        [[nodiscard]] static constexpr std::size_t rows() noexcept {
            return 3;
        }

        Vector3<T>& operator[](std::size_t index) {
            return data[index];
        }

        const Vector3<T>& operator[](std::size_t index) const {
            return data[index];
        }

        [[nodiscard]] auto operator*(const Matrix<T, 3, 3>& other) const -> Matrix<T, 3, 3> {
            const Matrix<T, 3, 3> rows = transpose(*this);

            return {
                Vector3<T>{Vector3<T>::dot(rows[0], other[0]), Vector3<T>::dot(rows[1], other[0]), Vector3<T>::dot(rows[2], other[0])},
                Vector3<T>{Vector3<T>::dot(rows[0], other[1]), Vector3<T>::dot(rows[1], other[1]), Vector3<T>::dot(rows[2], other[1])},
                Vector3<T>{Vector3<T>::dot(rows[0], other[2]), Vector3<T>::dot(rows[1], other[2]), Vector3<T>::dot(rows[2], other[2])},
            };
        }

        [[nodiscard]] auto operator*(const Vector3<T>& other) const -> Vector3<T> {
            const Matrix<T, 3, 3> rows = transpose(*this);

            return Vector3<T>{Vector3<T>::dot(rows[0], other), Vector3<T>::dot(rows[1], other), Vector3<T>::dot(rows[2], other)};
        }

        [[nodiscard]] static inline Matrix<T, 3, 3> identity() noexcept {
            return {
                Vector3<T>{1.0, 0.0, 0.0},
                Vector3<T>{0.0, 1.0, 0.0},
                Vector3<T>{0.0, 0.0, 1.0},
            };
        }

        [[nodiscard]] static inline Matrix<T, 3, 3> transpose(const Matrix<T, 3, 3>& mat) noexcept {
            return {
                Vector3<T>{mat[0][0], mat[1][0], mat[2][0]},
                Vector3<T>{mat[0][1], mat[1][1], mat[2][1]},
                Vector3<T>{mat[0][2], mat[1][2], mat[2][2]},
            };
        }

        [[nodiscard]] static inline Matrix<T, 3, 3> scaling(const Vector3<T>& scaling) {
            return {
                Vector3<T>{scaling.x, 0.0, 0.0},
                Vector3<T>{0.0, scaling.y, 0.0},
                Vector3<T>{0.0, 0.0, scaling.z},
            };
        }

    private:
        Vector3<T> data[3];
    };

    template <typename T>
    class Matrix<T, 4, 4> {
    public:
        Matrix() = default;

        template <typename... Args>
        requires((SameTypes<Args, Vector4<T>> && ...) && sizeof...(Args) == 4)
        Matrix(const Args&... columns)
            : data(columns...) {
        }

        [[nodiscard]] static constexpr std::size_t columns() noexcept {
            return 4;
        }

        [[nodiscard]] static constexpr std::size_t rows() noexcept {
            return 4;
        }

        Vector4<T>& operator[](std::size_t index) {
            return data[index];
        }

        const Vector4<T>& operator[](std::size_t index) const {
            return data[index];
        }

        [[nodiscard]] auto operator*(const Matrix<T, 4, 4>& other) const -> Matrix<T, 4, 4> {
            const Matrix<T, 4, 4> rows = transpose(*this);

            return {
                Vector4<T>{Vector4<T>::dot(rows[0], other[0]), Vector4<T>::dot(rows[1], other[0]), Vector4<T>::dot(rows[2], other[0]), Vector4<T>::dot(rows[3], other[0])},
                Vector4<T>{Vector4<T>::dot(rows[0], other[1]), Vector4<T>::dot(rows[1], other[1]), Vector4<T>::dot(rows[2], other[1]), Vector4<T>::dot(rows[3], other[1])},
                Vector4<T>{Vector4<T>::dot(rows[0], other[2]), Vector4<T>::dot(rows[1], other[2]), Vector4<T>::dot(rows[2], other[2]), Vector4<T>::dot(rows[3], other[2])},
                Vector4<T>{Vector4<T>::dot(rows[0], other[3]), Vector4<T>::dot(rows[1], other[3]), Vector4<T>::dot(rows[2], other[3]), Vector4<T>::dot(rows[3], other[3])},
            };
        }

        [[nodiscard]] auto operator*(const Vector4<T>& other) const -> Vector4<T> {
            const Matrix<T, 4, 4> rows = transpose(*this);

            return Vector4<T>{Vector4<T>::dot(rows[0], other), Vector4<T>::dot(rows[1], other), Vector4<T>::dot(rows[2], other), Vector4<T>::dot(rows[3], other)};
        }

        [[nodiscard]] static inline Matrix<T, 4, 4> identity() noexcept {
            return {
                Vector4<T>{1.0, 0.0, 0.0, 0.0},
                Vector4<T>{0.0, 1.0, 0.0, 0.0},
                Vector4<T>{0.0, 0.0, 1.0, 0.0},
                Vector4<T>{0.0, 0.0, 0.0, 1.0},
            };
        }

        [[nodiscard]] static inline Matrix<T, 4, 4> transpose(const Matrix<T, 4, 4>& mat) noexcept {
            return {
                Vector4<T>{mat[0][0], mat[1][0], mat[2][0], mat[3][0]},
                Vector4<T>{mat[0][1], mat[1][1], mat[2][1], mat[3][1]},
                Vector4<T>{mat[0][2], mat[1][2], mat[2][2], mat[3][2]},
                Vector4<T>{mat[0][3], mat[1][3], mat[2][3], mat[3][3]},
            };
        }

        [[nodiscard]] static inline Matrix<T, 4, 4> translation(const Vector3<T>& translation) {
            return {
                Vector4<T>{1.0, 0.0, 0.0, 0.0},
                Vector4<T>{0.0, 1.0, 0.0, 0.0},
                Vector4<T>{0.0, 0.0, 1.0, 0.0},
                Vector4<T>{translation.x, translation.y, translation.z, 1.0},
            };
        }

        [[nodiscard]] static inline Matrix<T, 4, 4> scaling(const Vector3<T>& scaling) {
            return {
                Vector4<T>{scaling.x, 0.0, 0.0, 0.0},
                Vector4<T>{0.0, scaling.y, 0.0, 0.0},
                Vector4<T>{0.0, 0.0, scaling.z, 0.0},
                Vector4<T>{0.0, 0.0, 0.0, 1.0},
            };
        }

    private:
        Vector4<T> data[4];
    };

    template <typename T>
    using Matrix2x2 = Matrix<T, 2, 2>;

    template <typename T>
    using Matrix3x3 = Matrix<T, 3, 3>;

    template <typename T>
    using Matrix4x4 = Matrix<T, 4, 4>;

    using Matrix2x2f = Matrix2x2<float>;
    using Matrix3x3f = Matrix3x3<float>;
    using Matrix4x4f = Matrix4x4<float>;

    using Matrix2x2d = Matrix2x2<double>;
    using Matrix3x3d = Matrix3x3<double>;
    using Matrix4x4d = Matrix4x4<double>;
}