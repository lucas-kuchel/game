#pragma once

#include <Systems/Registry.hpp>
#include <Systems/Renderer.hpp>
#include <Systems/Window.hpp>

#include <Debug/Exception.hpp>
#include <Debug/Logging.hpp>

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace Game
{
    struct ConstantBufferComponent
    {
        Resources::BufferHandle Handle;
    };

    struct CameraComponent2D
    {
        float NearPlane = 0.0;
        float FarPlane = 0.0;
        float Aspect = 0.0;
    };

    struct CameraComponent3D
    {
        float FOV = 0.0;
        float NearPlane = 0.0;
        float FarPlane = 0.0;
        float Aspect = 0.0;

        glm::fmat4 Projection;
        glm::fmat4 View;
    };
}