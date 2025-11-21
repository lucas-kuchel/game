#pragma once

#include <cmath>
#include <cstdlib>

namespace engine {
    struct Vector2 {
        float x = 0.0;
        float y = 0.0;

        auto operator+(const Vector2& other) const -> Vector2 {
            return Vector2{
                .x = x + other.x,
                .y = y + other.y,
            };
        }

        auto operator-(const Vector2& other) const -> Vector2 {
            return Vector2{
                .x = x - other.x,
                .y = y - other.y,
            };
        }

        auto operator*(const Vector2& other) const -> Vector2 {
            return Vector2{
                .x = x * other.x,
                .y = y * other.y,
            };
        }

        auto operator/(const Vector2& other) const -> Vector2 {
            return Vector2{
                .x = x / other.x,
                .y = y / other.y,
            };
        }

        auto operator[](std::size_t index) const -> const float& {
            switch (index) {
                case 0:
                    return x;
                case 1:
                    return y;
                default:
                    return x;
            }
        }

        auto operator[](std::size_t index) -> float& {
            switch (index) {
                case 0:
                    return x;
                case 1:
                    return y;
                default:
                    return x;
            }
        }
    };

    struct Vector3 {
        float x;
        float y;
        float z;

        auto operator+(const Vector3& other) const -> Vector3 {
            return Vector3{
                .x = x + other.x,
                .y = y + other.y,
                .z = z + other.z,
            };
        }

        auto operator-(const Vector3& other) const -> Vector3 {
            return Vector3{
                .x = x - other.x,
                .y = y - other.y,
                .z = z - other.z,
            };
        }

        auto operator*(const Vector3& other) const -> Vector3 {
            return Vector3{
                .x = x * other.x,
                .y = y * other.y,
                .z = z * other.z,
            };
        }

        auto operator/(const Vector3& other) const -> Vector3 {
            return Vector3{
                .x = x / other.x,
                .y = y / other.y,
                .z = z / other.z,
            };
        }
    };

    struct Vector4 {
        float x;
        float y;
        float z;
        float w;

        auto operator+(const Vector4& other) const -> Vector4 {
            return Vector4{
                .x = x + other.x,
                .y = y + other.y,
                .z = z + other.z,
                .w = w + other.w,
            };
        }

        auto operator-(const Vector4& other) const -> Vector4 {
            return Vector4{
                .x = x - other.x,
                .y = y - other.y,
                .z = z - other.z,
                .w = w - other.w,
            };
        }

        auto operator*(const Vector4& other) const -> Vector4 {
            return Vector4{
                .x = x * other.x,
                .y = y * other.y,
                .z = z * other.z,
                .w = w * other.w,
            };
        }

        auto operator/(const Vector4& other) const -> Vector4 {
            return Vector4{
                .x = x / other.x,
                .y = y / other.y,
                .z = z / other.z,
                .w = w / other.w,
            };
        }
    };

    [[nodiscard]] inline auto dot(const Vector2& one, const Vector2& two) -> float {
        return (one.x * two.x) + (one.y * two.y);
    }

    [[nodiscard]] inline auto dot(const Vector3& one, const Vector3& two) -> float {
        return (one.x * two.x) + (one.y * two.y) + (one.z * two.z);
    }

    [[nodiscard]] inline auto dot(const Vector4& one, const Vector4& two) -> float {
        return (one.x * two.x) + (one.y * two.y) + (one.z * two.z) + (one.w * two.w);
    }

    [[nodiscard]] inline auto magnitude(const Vector2& one) -> float {
        return std::sqrt((one.x * one.x) + (one.y * one.y));
    }

    [[nodiscard]] inline auto magnitude(const Vector3& one) -> float {
        return std::sqrt((one.x * one.x) + (one.y * one.y) + (one.z * one.z));
    }

    [[nodiscard]] inline auto magnitude(const Vector4& one) -> float {
        return std::sqrt((one.x * one.x) + (one.y * one.y) + (one.z * one.z) + (one.w * one.w));
    }

    [[nodiscard]] inline auto length(const Vector2& one) -> float {
        return std::sqrt((one.x * one.x) + (one.y * one.y));
    }

    [[nodiscard]] inline auto length(const Vector3& one) -> float {
        return std::sqrt((one.x * one.x) + (one.y * one.y) + (one.z * one.z));
    }

    [[nodiscard]] inline auto length(const Vector4& one) -> float {
        return std::sqrt((one.x * one.x) + (one.y * one.y) + (one.z * one.z) + (one.w * one.w));
    }

    [[nodiscard]] inline auto cross(const Vector3& one, const Vector3& two) -> Vector3 {
        return Vector3{
            .x = (one.x * two.z) - (one.z * two.y),
            .y = (one.z * two.x) - (one.x * two.z),
            .z = (one.x * two.y) - (one.y * two.x),
        };
    }
}