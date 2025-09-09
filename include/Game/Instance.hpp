#pragma once

#include <Systems/Context.hpp>
#include <Systems/Registry.hpp>
#include <Systems/Renderer.hpp>
#include <Systems/Window.hpp>

#include <Debug/Exception.hpp>
#include <Debug/Logging.hpp>

#include <Game/BasicMesh.hpp>
#include <Game/Camera.hpp>
#include <Game/Player.hpp>
#include <Game/Transform.hpp>

namespace Game
{
    using RegistryDescriptor = Systems::RegistryDescriptor<
        CameraComponent3D,
        BasicMeshComponent,
        ConstantBufferComponent,
        RenderableComponent,
        TransformComponent3D,
        PlayerComponent>;

    class Instance
    {
    public:
        Instance(Systems::Context& context, Systems::Renderer& renderer, Systems::Window& window);
        ~Instance();

        void Update();

    private:
        Systems::Registry<RegistryDescriptor> mRegistry;

        Resources::PipelineHandle mBasicMeshPipeline;
        Resources::BufferHandle mCameraBuffer;

        Systems::Context& mContext;
        Systems::Renderer& mRenderer;
        Systems::Window& mWindow;

        void CreateBasicMeshPipeline();
    };
}