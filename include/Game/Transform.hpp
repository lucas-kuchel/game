#pragma once

#include <glm/glm.hpp>

namespace Game
{
    struct TransformComponent2D
    {
        glm::fvec2 Position;
        glm::fvec2 Scale;
        glm::fvec2 Right;
        glm::fvec2 Up;

        float Rotation = 0.0;
    };

    struct TransformComponent3D
    {
        glm::fvec3 Position;
        glm::fvec3 Scale;
        glm::fvec3 Rotation;
        glm::fvec3 Right;
        glm::fvec3 Up;
        glm::fvec3 Forward;
    };
}