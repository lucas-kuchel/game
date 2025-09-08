#include <Game/Instance.hpp>

#include <glm/gtc/quaternion.hpp>

namespace Game
{
    Instance::Instance(Systems::Context& context, Systems::Renderer& renderer, Systems::Window& window)
        : mContext(context), mRenderer(renderer), mWindow(window)
    {
        mRegistry.CreateEntity<CameraComponent3D, TransformComponent3D>();
        mRegistry.CreateEntity<BasicMeshComponent, TransformComponent3D>();

        Resources::BufferHandle cameraBuffer;

        for (auto [entity, camera] : mRegistry.GetEntityView<CameraComponent3D>())
        {
            camera.FOV = 70.0;
            camera.NearPlane = 0.1;
            camera.FarPlane = 100.0;

            CameraComponent3D::Create(camera, mRenderer, mWindow);

            cameraBuffer = camera.CameraBuffer;
        }

        CreateBasicMeshPipeline(cameraBuffer);

        for (auto [entity, mesh] : mRegistry.GetEntityView<BasicMeshComponent>())
        {
            mesh.Vertices = {
                BasicMeshVertex{{-0.5f, -0.5f, 0.0f}, {1, 0, 0, 1}},
                BasicMeshVertex{{0.5f, -0.5f, 0.0f}, {0, 1, 0, 1}},
                BasicMeshVertex{{0.5f, 0.5f, 0.0f}, {0, 0, 1, 1}},
                BasicMeshVertex{{-0.5f, 0.5f, 0.0f}, {1, 1, 1, 1}},
            };

            mesh.Triangles = {
                BasicMeshTriangle{0, 1, 2},
                BasicMeshTriangle{0, 2, 3},
            };

            BasicMeshComponent::Create(mesh, mRenderer, mBasicMeshPipeline);
        }
    }

    Instance::~Instance()
    {
        for (auto [entity, mesh] : mRegistry.GetEntityView<BasicMeshComponent>())
        {
            BasicMeshComponent::Delete(mesh, mRenderer);
        }

        for (auto [entity, camera] : mRegistry.GetEntityView<CameraComponent3D>())
        {
            CameraComponent3D::Delete(camera, mRenderer);
        }
    }

    void Instance::Update()
    {
        for (auto [entity, camera, transform] : mRegistry.GetEntityView<CameraComponent3D, TransformComponent3D>())
        {
            CameraComponent3D::Transform(camera, transform);
        }

        for (auto [entity, camera] : mRegistry.GetEntityView<CameraComponent3D>())
        {
            CameraComponent3D::Update(camera, mRenderer, mWindow);
        }

        for (auto [entity, mesh, transform] : mRegistry.GetEntityView<BasicMeshComponent, TransformComponent3D>())
        {
            BasicMeshComponent::Transform(mesh, transform);
        }

        auto commandBuffer = mRenderer.CreateCommandBuffer();

        for (auto [entity, mesh] : mRegistry.GetEntityView<BasicMeshComponent>())
        {
            BasicMeshComponent::Update(mesh, mRenderer);

            commandBuffer.Submit(mesh.Submission);
        }

        mRenderer.DrawCommandBuffer(commandBuffer);
    }

    void Instance::CreateBasicMeshPipeline(Resources::BufferHandle& cameraBuffer)
    {
        std::vector<Resources::BufferAttributeDescriptor> vertexFormat = {
            {.Type = Resources::BufferAttributeType::R32G32B32_FLOAT, .Slot = 0},
            {.Type = Resources::BufferAttributeType::R32G32B32A32_FLOAT, .Slot = 1},
        };

        Resources::ShaderDescriptor vertexShaderDescriptor = {
            .Type = Resources::ShaderType::VERTEX,
            .Path = "assets/shaders/basic.vert",
        };

        Resources::ShaderDescriptor pixelShaderDescriptor = {
            .Type = Resources::ShaderType::PIXEL,
            .Path = "assets/shaders/basic.frag",
        };

        Resources::PipelineDescriptor pipelineDescriptor = {
            .Shaders = {vertexShaderDescriptor, pixelShaderDescriptor},
            .VertexInput = vertexFormat,
            .InputBuffers = {cameraBuffer},
            .Primitive = Resources::PipelinePrimitive::TRIANGLE_LIST,
            .FrontFace = Resources::PipelineFrontFace::CLOCKWISE,
            .PolygonMode = Resources::PipelinePolygonMode::SOLID,
            .FaceCulling = Resources::PipelineFaceCulling::FRONTFACE,
        };

        mBasicMeshPipeline = mRenderer.CreatePipeline(pipelineDescriptor);
    }
}