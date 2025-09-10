#include <Game/Instance.hpp>

#include <glm/gtc/quaternion.hpp>

namespace Game
{
    Instance::Instance(Systems::Context& context, Systems::Renderer& renderer, Systems::Window& window)
        : mContext(context), mRenderer(renderer), mWindow(window)
    {
        mRegistry.CreateEntity<CameraComponent3D, ConstantBufferComponent, TransformComponent3D, PlayerComponent>();
        mRegistry.CreateEntity<BasicMeshComponent, RenderableComponent>();

        CreateBasicMeshPipeline();

        auto cameraBitmask = mRegistry.MakeBitmask<CameraComponent3D>();
        auto bufferBitmask = mRegistry.MakeBitmask<ConstantBufferComponent>();
        auto meshBitmask = mRegistry.MakeBitmask<BasicMeshComponent>();
        auto renderableBitmask = mRegistry.MakeBitmask<RenderableComponent>();
        auto transformBitmask = mRegistry.MakeBitmask<TransformComponent3D>();

        auto mouseLayer = mWindow.CreateInteractionLayer<Systems::WindowInteractive::MOUSE_LAYER>();

        auto cursorState = mouseLayer.GetCursorState();
        cursorState.Mode = Systems::CursorMode::DISABLED;
        mouseLayer.SetCursorState(cursorState);

        for (auto& [bitmask, archetype] : mRegistry.GetArchetypes())
        {
            if (cameraBitmask == (bitmask & cameraBitmask))
            {
                for (auto [entity, camera] : mRegistry.GetEntityView<CameraComponent3D>(archetype))
                {
                    camera.FOV = 70.0;
                    camera.NearPlane = 0.1;
                    camera.FarPlane = 100.0;

                    glm::ivec2 size = window.Get<Systems::WindowAttribute::SIZE>();

                    camera.Aspect = static_cast<float>(size.x) / static_cast<float>(size.y);
                }
            }

            if ((cameraBitmask | bufferBitmask) == (bitmask & (cameraBitmask | bufferBitmask)))
            {
                for (auto [entity, camera, buffer] : mRegistry.GetEntityView<CameraComponent3D, ConstantBufferComponent>(archetype))
                {
                    Resources::BufferDescriptor cameraDescriptor = {
                        .Size = 2 * sizeof(glm::fmat4),
                    };

                    buffer.Handle = mRenderer.CreateBuffer(cameraDescriptor);

                    mCameraBuffer = buffer.Handle;
                }
            }

            if ((cameraBitmask | transformBitmask) == (bitmask & (cameraBitmask | transformBitmask)))
            {
                for (auto [entity, camera, transform] : mRegistry.GetEntityView<CameraComponent3D, TransformComponent3D>(archetype))
                {
                    transform.Position = {0.0, 0.0, 2.0};
                    transform.Rotation = {0.0, 0.0, 0.0};
                    transform.Scale = {1.0, 1.0};
                }
            }

            if (meshBitmask == (bitmask & meshBitmask))
            {
                for (auto [entity, mesh] : mRegistry.GetEntityView<BasicMeshComponent>(archetype))
                {
                    mesh.Vertices = {
                        // Front face (like your quad)
                        BasicMeshVertex{{-0.5, -0.5, 0.5}, {1.0, 0.5, 0.0, 1.0}}, // v0
                        BasicMeshVertex{{0.5, -0.5, 0.5}, {0.5, 1.0, 0.5, 1.0}},  // v1
                        BasicMeshVertex{{0.5, 0.5, 0.5}, {0.0, 0.5, 1.0, 1.0}},   // v2
                        BasicMeshVertex{{-0.5, 0.5, 0.5}, {0.0, 0.0, 0.5, 1.0}},  // v3

                        // Back face (flipped color pattern: v1 blue → v3 red)
                        BasicMeshVertex{{-0.5, -0.5, -0.5}, {0.0, 1.0, 1.0, 1.0}}, // v4 - cyan
                        BasicMeshVertex{{0.5, -0.5, -0.5}, {1.0, 0.0, 1.0, 1.0}},  // v5 - magenta
                        BasicMeshVertex{{0.5, 0.5, -0.5}, {1.0, 1.0, 0.0, 1.0}},   // v6 - yellow
                        BasicMeshVertex{{-0.5, 0.5, -0.5}, {1.0, 1.0, 1.0, 1.0}},  // v7 - white
                    };

                    mesh.Triangles = {
                        // Front face
                        {0, 1, 2},
                        {0, 2, 3},

                        // Back face
                        {4, 6, 5},
                        {4, 7, 6},

                        // Left face
                        {4, 0, 3},
                        {4, 3, 7},

                        // Right face
                        {1, 5, 6},
                        {1, 6, 2},

                        // Bottom face
                        {4, 5, 1},
                        {4, 1, 0},

                        // Top face
                        {3, 2, 6},
                        {3, 6, 7},
                    };

                    Resources::BufferDescriptor vertexDescriptor = {
                        .Size = mesh.Vertices.size() * sizeof(BasicMeshVertex),
                    };

                    Resources::BufferDescriptor indexDescriptor = {
                        .Size = mesh.Triangles.size() * sizeof(BasicMeshTriangle),
                    };

                    mesh.VertexBuffer = renderer.CreateBuffer(vertexDescriptor);
                    mesh.IndexBuffer = renderer.CreateBuffer(indexDescriptor);

                    renderer.SetBufferData(mesh.VertexBuffer, {.Data = mesh.Vertices.data(), .Stride = mesh.Vertices.size() * sizeof(BasicMeshVertex), .Offset = 0});
                    renderer.SetBufferData(mesh.IndexBuffer, {.Data = mesh.Triangles.data(), .Stride = mesh.Triangles.size() * sizeof(BasicMeshTriangle), .Offset = 0});
                }
            }

            if ((renderableBitmask | meshBitmask) == (bitmask & (renderableBitmask | meshBitmask)))
            {
                for (auto [entity, mesh, renderable] : mRegistry.GetEntityView<BasicMeshComponent, RenderableComponent>(archetype))
                {
                    Resources::SubmissionDescriptor submissionDescriptor = {
                        .DataBuffers = {mCameraBuffer},
                        .VertexBuffers = {mesh.VertexBuffer},
                        .IndexBuffer = mesh.IndexBuffer,
                        .IndexBufferType = Resources::BufferAttributeType::R32_UINT,
                        .Pipeline = mBasicMeshPipeline,
                    };

                    renderable.Handle = mRenderer.CreateSubmission(submissionDescriptor);
                }
            }
        }
    }

    Instance::~Instance()
    {
        auto bufferBitmask = mRegistry.MakeBitmask<ConstantBufferComponent>();
        auto meshBitmask = mRegistry.MakeBitmask<BasicMeshComponent>();
        auto renderableBitmask = mRegistry.MakeBitmask<RenderableComponent>();

        for (auto& [bitmask, archetype] : mRegistry.GetArchetypes())
        {
            if (bufferBitmask == (bitmask | bufferBitmask))
            {
                for (auto [entity, buffer] : mRegistry.GetEntityView<ConstantBufferComponent>(archetype))
                {
                    mRenderer.DeleteBuffer(buffer.Handle);
                }
            }

            if (meshBitmask == (bitmask | meshBitmask))
            {
                for (auto [entity, mesh] : mRegistry.GetEntityView<BasicMeshComponent>(archetype))
                {
                    mRenderer.DeleteBuffer(mesh.VertexBuffer);
                    mRenderer.DeleteBuffer(mesh.IndexBuffer);
                }
            }

            if (renderableBitmask == (bitmask | renderableBitmask))
            {
                for (auto [entity, renderable] : mRegistry.GetEntityView<RenderableComponent>(archetype))
                {
                    mRenderer.DeleteSubmission(renderable.Handle);
                }
            }
        }
    }

    void Instance::Update()
    {
        auto cameraBitmask = mRegistry.MakeBitmask<CameraComponent3D>();
        auto bufferBitmask = mRegistry.MakeBitmask<ConstantBufferComponent>();
        auto transformBitmask = mRegistry.MakeBitmask<TransformComponent3D>();
        auto renderableBitmask = mRegistry.MakeBitmask<RenderableComponent>();
        auto playerBitmask = mRegistry.MakeBitmask<PlayerComponent>();

        auto commandBuffer = mRenderer.CreateCommandBuffer();

        auto keyboardLayer = mWindow.CreateInteractionLayer<Systems::WindowInteractive::KEYBOARD_LAYER>();
        auto mouseLayer = mWindow.CreateInteractionLayer<Systems::WindowInteractive::MOUSE_LAYER>();

        for (auto& [bitmask, archetype] : mRegistry.GetArchetypes())
        {
            if ((transformBitmask | playerBitmask) == (bitmask & (transformBitmask | playerBitmask)))
            {
                for (auto [entity, transform, player] : mRegistry.GetEntityView<TransformComponent3D, PlayerComponent>(archetype))
                {
                    glm::vec3 movement{0.0f};

                    if (keyboardLayer.GetKeyState(Systems::Key::W) == Systems::PressableState::HELD)
                    {
                        movement.z += 1.0f;
                    }
                    if (keyboardLayer.GetKeyState(Systems::Key::S) == Systems::PressableState::HELD)
                    {
                        movement.z -= 1.0f;
                    }
                    if (keyboardLayer.GetKeyState(Systems::Key::A) == Systems::PressableState::HELD)
                    {
                        movement.x -= 1.0f;
                    }
                    if (keyboardLayer.GetKeyState(Systems::Key::D) == Systems::PressableState::HELD)
                    {
                        movement.x += 1.0f;
                    }
                    if (keyboardLayer.GetKeyState(Systems::Key::SPACE) == Systems::PressableState::HELD)
                    {
                        movement.y += 1.0f;
                    }
                    if (keyboardLayer.GetKeyState(Systems::Key::LSHIFT) == Systems::PressableState::HELD)
                    {
                        movement.y -= 1.0f;
                    }

                    glm::fquat yaw = glm::angleAxis(glm::radians(transform.Rotation.y), glm::fvec3(0, 1, 0));
                    glm::fquat pitch = glm::angleAxis(glm::radians(transform.Rotation.x), glm::fvec3(1, 0, 0));

                    glm::fquat orientation = yaw * pitch;

                    glm::fvec3 forward = orientation * glm::fvec3(0, 0, -1);
                    glm::fvec3 right = orientation * glm::fvec3(1, 0, 0);
                    glm::fvec3 up = glm::fvec3(0, 1, 0);

                    transform.Position += (forward * movement.z + right * movement.x + up * movement.y) * player.Speed;

                    static glm::dvec2 prevCursor = mouseLayer.GetCursorState().Position;

                    glm::dvec2 cursorPos = mouseLayer.GetCursorState().Position;
                    glm::dvec2 mouseDelta = cursorPos - prevCursor;

                    prevCursor = cursorPos;

                    transform.Rotation.y += static_cast<float>(-mouseDelta.x) * player.Sensitivity;
                    transform.Rotation.x -= static_cast<float>(mouseDelta.y) * player.Sensitivity;

                    if (keyboardLayer.GetKeyState(Systems::Key::Q) == Systems::PressableState::HELD)
                    {
                        transform.Rotation.z += 0.1 * player.Sensitivity;
                    }
                    if (keyboardLayer.GetKeyState(Systems::Key::E) == Systems::PressableState::HELD)
                    {
                        transform.Rotation.z -= 0.1 * player.Sensitivity;
                    }

                    transform.Rotation.x = glm::clamp(transform.Rotation.x, -89.0f, 89.0f);
                }
            }

            if (cameraBitmask == (bitmask & cameraBitmask))
            {
                for (auto [entity, camera] : mRegistry.GetEntityView<CameraComponent3D>(archetype))
                {
                    glm::ivec2 size = mWindow.Get<Systems::WindowAttribute::SIZE>();
                    camera.Aspect = static_cast<float>(size.x) / static_cast<float>(size.y);
                }
            }

            if ((cameraBitmask | transformBitmask) == (bitmask & (cameraBitmask | transformBitmask)))
            {
                for (auto [entity, camera, transform] : mRegistry.GetEntityView<CameraComponent3D, TransformComponent3D>(archetype))
                {
                    glm::fquat pitch = glm::angleAxis(glm::radians(transform.Rotation.x), glm::fvec3(1, 0, 0));
                    glm::fquat yaw = glm::angleAxis(glm::radians(transform.Rotation.y), glm::fvec3(0, 1, 0));
                    glm::fquat roll = glm::angleAxis(glm::radians(transform.Rotation.z), glm::fvec3(0, 0, 1));

                    glm::fquat orientation = yaw * pitch * roll;

                    glm::fvec3 forward = glm::normalize(orientation * glm::fvec3(0, 0, -1));
                    glm::fvec3 up = glm::normalize(orientation * glm::fvec3(0, 1, 0));

                    camera.View = glm::lookAt(transform.Position, transform.Position + forward, up);
                }
            }

            if ((cameraBitmask | bufferBitmask) == (bitmask & (cameraBitmask | bufferBitmask)))
            {
                for (auto [entity, camera, buffer] : mRegistry.GetEntityView<CameraComponent3D, ConstantBufferComponent>(archetype))
                {
                    camera.Projection = glm::perspective(glm::radians(camera.FOV), camera.Aspect, camera.NearPlane, camera.FarPlane);

                    std::array<glm::fmat4, 2> data = {
                        camera.Projection,
                        camera.View,
                    };

                    Resources::BufferData bufferData = {
                        .Data = data.data(),
                        .Stride = sizeof(data),
                        .Offset = 0,
                    };

                    mRenderer.SetBufferData(buffer.Handle, bufferData);
                }
            }

            if (renderableBitmask == (bitmask & renderableBitmask))
            {
                for (auto [entity, renderable] : mRegistry.GetEntityView<RenderableComponent>(archetype))
                {
                    commandBuffer.Submit(renderable.Handle);
                }
            }
        }

        mRenderer.DrawCommandBuffer(commandBuffer);
    }

    void Instance::CreateBasicMeshPipeline()
    {
        Resources::BufferFormatDescriptor vertexFormat = {
            .Attributes = {
                Resources::BufferAttributeType::R32G32B32_FLOAT,
                Resources::BufferAttributeType::R32G32B32A32_FLOAT,
            },
            .Index = 0,
            .Type = Resources::BufferType::VERTEX,
        };

        Resources::BufferFormatDescriptor cameraFormat = {
            .Attributes = {
                Resources::BufferAttributeType::F32_4x4,
                Resources::BufferAttributeType::F32_4x4,
            },
            .Index = 0,
            .Type = Resources::BufferType::CONSTANT,
        };

        Resources::ShaderDescriptor vertexShaderDescriptor = {
            .Stage = Resources::ShaderStage::VERTEX,
            .SPIRVShaderPath = "assets/shaders/spv/basic.vertex.spv",
            .MetalShaderPath = "assets/shaders/metal/basic.metallib",
            .Function = "VSMain",
        };

        Resources::ShaderDescriptor pixelShaderDescriptor = {
            .Stage = Resources::ShaderStage::PIXEL,
            .SPIRVShaderPath = "assets/shaders/spv/basic.pixel.spv",
            .MetalShaderPath = "assets/shaders/metal/basic.metallib",
            .Function = "PSMain",
        };

        Resources::RasterPipelineDescriptor pipelineDescriptor = {
            .Shaders = {vertexShaderDescriptor, pixelShaderDescriptor},
            .DataBufferFormats = {cameraFormat},
            .VertexBufferFormats = {vertexFormat},
            .RasterState = {
                .Primitive = Resources::PipelinePrimitive::TRIANGLE_LIST,
                .FrontFace = Resources::PipelineFrontFace::CLOCKWISE,
                .PolygonMode = Resources::PipelinePolygonMode::SOLID,
                .FaceCulling = Resources::PipelineFaceCulling::FRONTFACE,
            },
        };

        mBasicMeshPipeline = mRenderer.CreatePipeline(pipelineDescriptor);
    }
}