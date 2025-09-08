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
    struct CameraComponent2D
    {
        glm::fmat4 Projection;
        glm::fmat4 View;

        float NearPlane = 0.0;
        float FarPlane = 0.0;
        float Aspect = 0.0;

        Resources::BufferHandle CameraBuffer;

        static void Create(CameraComponent2D& camera, Systems::Renderer& renderer, Systems::Window& window);
        static void Update(CameraComponent2D& camera, Systems::Renderer& renderer, Systems::Window& window);
        static void Delete(CameraComponent2D& camera, Systems::Renderer& renderer);
    };

    struct CameraComponent3D
    {
        glm::fmat4 Projection;
        glm::fmat4 View;

        float FOV = 0.0;
        float NearPlane = 0.0;
        float FarPlane = 0.0;
        float Aspect = 0.0;

        Resources::BufferHandle CameraBuffer;

        static void Create(CameraComponent3D& camera, Systems::Renderer& renderer, Systems::Window& window);
        static void Update(CameraComponent3D& camera, Systems::Renderer& renderer, Systems::Window& window);
        static void Delete(CameraComponent3D& camera, Systems::Renderer& renderer);
    };
}