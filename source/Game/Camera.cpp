#include <Game/Camera.hpp>

namespace Game
{
    void CameraComponent2D::Create(CameraComponent2D& camera, Systems::Renderer& renderer, Systems::Window& window)
    {
        glm::ivec2 size = window.Get<Systems::WindowAttribute::SIZE>();

        camera.Aspect = static_cast<float>(size.x) / static_cast<float>(size.y);
        camera.Projection = glm::ortho(-camera.Aspect, camera.Aspect, -1.0f, 1.0f, camera.NearPlane, camera.FarPlane);

        Resources::BufferDescriptor descriptor = {
            .Type = Resources::BufferType::CONSTANT,
            .Attributes = {{.Type = Resources::BufferAttributeType::F32_4x4, .Slot = 0}, {.Type = Resources::BufferAttributeType::F32_4x4, .Slot = 1}},
            .Slot = 0,
            .Size = 2 * sizeof(glm::fmat4),
        };

        camera.CameraBuffer = renderer.CreateBuffer(descriptor);
    }

    void CameraComponent2D::Update(CameraComponent2D& camera, Systems::Renderer& renderer, Systems::Window& window)
    {
        glm::ivec2 size = window.Get<Systems::WindowAttribute::SIZE>();

        camera.Aspect = static_cast<float>(size.x) / static_cast<float>(size.y);
        camera.Projection = glm::ortho(-camera.Aspect, camera.Aspect, -1.0f, 1.0f, camera.NearPlane, camera.FarPlane);

        glm::fmat4 viewMatrix = {1.0f};

        camera.View = viewMatrix;

        std::array<glm::fmat4, 2> data = {
            camera.Projection,
            camera.View,
        };

        Resources::BufferData bufferData = {
            .Data = data.data(),
            .Size = sizeof(data),
            .Offset = 0,
        };

        renderer.SetBufferData(camera.CameraBuffer, bufferData);
    }

    void CameraComponent2D::Delete(CameraComponent2D& camera, Systems::Renderer& renderer)
    {
        renderer.DeleteBuffer(camera.CameraBuffer);
    }

    void CameraComponent3D::Create(CameraComponent3D& camera, Systems::Renderer& renderer, Systems::Window& window)
    {
        glm::ivec2 size = window.Get<Systems::WindowAttribute::SIZE>();

        camera.Aspect = static_cast<float>(size.x) / static_cast<float>(size.y);
        camera.Projection = glm::perspective(glm::radians(camera.FOV), camera.Aspect, camera.NearPlane, camera.FarPlane);

        Resources::BufferDescriptor descriptor = {
            .Type = Resources::BufferType::CONSTANT,
            .Attributes = {{.Type = Resources::BufferAttributeType::F32_4x4, .Slot = 0}, {.Type = Resources::BufferAttributeType::F32_4x4, .Slot = 1}},
            .Slot = 0,
            .Size = 2 * sizeof(glm::fmat4),
        };

        camera.CameraBuffer = renderer.CreateBuffer(descriptor);
    }

    void CameraComponent3D::Update(CameraComponent3D& camera, Systems::Renderer& renderer, Systems::Window& window)
    {
        glm::ivec2 size = window.Get<Systems::WindowAttribute::SIZE>();
        camera.Aspect = static_cast<float>(size.x) / static_cast<float>(size.y);
        camera.Projection = glm::perspective(glm::radians(camera.FOV), camera.Aspect, camera.NearPlane, camera.FarPlane);

        std::array<glm::fmat4, 2> data = {
            camera.Projection,
            camera.View,
        };

        Resources::BufferData bufferData = {
            .Data = data.data(),
            .Size = sizeof(data),
            .Offset = 0,
        };

        renderer.SetBufferData(camera.CameraBuffer, bufferData);
    }

    void CameraComponent3D::Delete(CameraComponent3D& camera, Systems::Renderer& renderer)
    {
        renderer.DeleteBuffer(camera.CameraBuffer);
    }
}