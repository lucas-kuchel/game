#pragma once

#include "vector.hpp"

#include <array>
#include <cstdlib>

namespace engine {
    class Matrix2x2 {
    public:
        Matrix2x2() {
            for (std::size_t i = 0; i < Size; i++) {
                data.at(i)[i] = 1;
            }
        }

        Matrix2x2(const Vector2& column1, const Vector2& column2) {
            data.at(0) = column1;
            data.at(1) = column2;
        }

        auto operator[](std::size_t index) -> Vector2& {
            return data.at(index);
        }

        auto operator[](std::size_t index) const -> const Vector2& {
            return data.at(index);
        }

        [[nodiscard]] auto operator*(const Matrix2x2& other) const -> Matrix2x2 {
            auto row = [](const Matrix2x2& mat, std::size_t index) {
                return Vector2{
                    .x = mat[index].x,
                    .y = mat[index].y,
                };
            };

            return {
                Vector2{
                    .x = dot(row(*this, 0), other[0]),
                    .y = dot(row(*this, 1), other[0]),
                },
                Vector2{
                    .x = dot(row(*this, 0), other[1]),
                    .y = dot(row(*this, 1), other[1]),
                },
            };
        }

    private:
        static constexpr std::size_t Size = 2;

        std::array<Vector2, 2> data;
    };
}