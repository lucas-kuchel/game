#include <Game/Instance.hpp>

#include <glm/gtc/quaternion.hpp>

#include <chrono>

namespace Game
{
    Instance::Instance(Systems::Context& context, Systems::Renderer& renderer, Systems::Window& window)
        : mContext(context), mRenderer(renderer), mWindow(window)
    {
        mRegistry.CreateEntity<CameraComponent3D, ConstantBufferComponent, TransformComponent3D, PlayerComponent>();
        mRegistry.CreateEntity<BasicMeshComponent, RenderableComponent, ConstantBufferComponent, TransformComponent3D>();
        mRegistry.CreateEntity<BasicMeshComponent, RenderableComponent, ConstantBufferComponent>();

        CreateBasicMeshPipeline();

        auto cameraBitmask = mRegistry.MakeBitmask<CameraComponent3D>();
        auto bufferBitmask = mRegistry.MakeBitmask<ConstantBufferComponent>();
        auto meshBitmask = mRegistry.MakeBitmask<BasicMeshComponent>();
        auto renderableBitmask = mRegistry.MakeBitmask<RenderableComponent>();
        auto transformBitmask = mRegistry.MakeBitmask<TransformComponent3D>();
        auto playerBitmask = mRegistry.MakeBitmask<PlayerComponent>();

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

            if ((meshBitmask | bufferBitmask) == (bitmask & (meshBitmask | bufferBitmask)))
            {
                for (auto [entity, buffer] : mRegistry.GetEntityView<ConstantBufferComponent>(archetype))
                {
                    Resources::BufferDescriptor constantDescriptor = {
                        .Size = sizeof(glm::fmat4),
                    };

                    buffer.Handle = mRenderer.CreateBuffer(constantDescriptor);

                    glm::fmat4 identityModel(1.0f);

                    mRenderer.SetBufferData(buffer.Handle, {.Data = &identityModel, .Stride = sizeof(glm::fmat4), .Offset = 0});

                    mConstantBuffer = buffer.Handle;
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

            if ((transformBitmask) == (bitmask & (transformBitmask)))
            {
                for (auto [entity, transform] : mRegistry.GetEntityView<TransformComponent3D>(archetype))
                {
                    transform.Position = {0.0, 0.0, 0.0};
                    transform.Rotation = {0.0, 0.0, 0.0};
                    transform.Scale = {1.0, 1.0, 1.0};
                }
            }

            if (meshBitmask == (bitmask & meshBitmask))
            {
                for (auto [entity, mesh] : mRegistry.GetEntityView<BasicMeshComponent>(archetype))
                {
                    float radius = 0.5;
                    uint32_t latitudeSegments = 64;
                    uint32_t longitudeSegments = 64;

                    for (uint32_t lat = 0; lat <= latitudeSegments; ++lat)
                    {
                        float theta = lat * glm::pi<float>() / latitudeSegments;
                        float sinTheta = sin(theta);
                        float cosTheta = cos(theta);

                        for (uint32_t lon = 0; lon <= longitudeSegments; ++lon)
                        {
                            float phi = lon * 2.0f * glm::pi<float>() / longitudeSegments;
                            float sinPhi = sin(phi);
                            float cosPhi = cos(phi);

                            glm::vec3 pos;
                            pos.x = radius * sinTheta * cosPhi;
                            pos.y = radius * cosTheta;
                            pos.z = radius * sinTheta * sinPhi;

                            glm::vec4 color = {0.8, 0.4, 0.2, 1.0f};

                            mesh.Vertices.push_back({pos, color});
                        }
                    }

                    for (uint32_t lat = 0; lat < latitudeSegments; ++lat)
                    {
                        for (uint32_t lon = 0; lon < longitudeSegments; ++lon)
                        {
                            uint32_t first = lat * (longitudeSegments + 1) + lon;
                            uint32_t second = first + longitudeSegments + 1;

                            mesh.Triangles.push_back({first, second, first + 1});
                            mesh.Triangles.push_back({second, second + 1, first + 1});
                        }
                    }

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

            if ((renderableBitmask | meshBitmask | bufferBitmask) == (bitmask & (renderableBitmask | meshBitmask | bufferBitmask)))
            {
                for (auto [entity, mesh, renderable, buffer] : mRegistry.GetEntityView<BasicMeshComponent, RenderableComponent, ConstantBufferComponent>(archetype))
                {
                    Resources::SubmissionDescriptor submissionDescriptor = {
                        .ShaderStages = {
                            Resources::ShaderStageSubmissionDescriptor{
                                .ConstantBuffers = {mCameraBuffer, buffer.Handle},
                                .StorageBuffers = {},
                                .Stage = Resources::ShaderStage::VERTEX,
                            },
                            Resources::ShaderStageSubmissionDescriptor{
                                .ConstantBuffers = {},
                                .StorageBuffers = {},
                                .Stage = Resources::ShaderStage::PIXEL,
                            },
                        },
                        .VertexBuffers = {mesh.VertexBuffer},
                        .IndexBuffer = mesh.IndexBuffer,
                        .IndexBufferType = Resources::BufferAttributeType::R32_UINT,
                        .Pipeline = mBasicMeshPipeline,
                    };

                    renderable.Handle = mRenderer.CreateSubmission(submissionDescriptor);
                }
            }

            if (playerBitmask == (bitmask & playerBitmask))
            {
                for (auto [entity, player] : mRegistry.GetEntityView<PlayerComponent>(archetype))
                {
                    player.Sensitivity = 0.25;
                    player.Speed = 2.0;
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
        using namespace std::chrono;

        static high_resolution_clock::time_point lastTime = high_resolution_clock::now();
        auto currentTime = high_resolution_clock::now();
        duration<float> deltaTimeDuration = currentTime - lastTime;
        float deltaTime = deltaTimeDuration.count();
        lastTime = currentTime;

        auto cameraBitmask = mRegistry.MakeBitmask<CameraComponent3D>();
        auto bufferBitmask = mRegistry.MakeBitmask<ConstantBufferComponent>();
        auto transformBitmask = mRegistry.MakeBitmask<TransformComponent3D>();
        auto renderableBitmask = mRegistry.MakeBitmask<RenderableComponent>();
        auto meshBitmask = mRegistry.MakeBitmask<BasicMeshComponent>();
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
                        movement.z += 1.0;
                    }
                    if (keyboardLayer.GetKeyState(Systems::Key::S) == Systems::PressableState::HELD)
                    {
                        movement.z -= 1.0;
                    }
                    if (keyboardLayer.GetKeyState(Systems::Key::A) == Systems::PressableState::HELD)
                    {
                        movement.x -= 1.0;
                    }
                    if (keyboardLayer.GetKeyState(Systems::Key::D) == Systems::PressableState::HELD)
                    {
                        movement.x += 1.0;
                    }
                    if (keyboardLayer.GetKeyState(Systems::Key::SPACE) == Systems::PressableState::HELD)
                    {
                        movement.y += 1.0;
                    }
                    if (keyboardLayer.GetKeyState(Systems::Key::LSHIFT) == Systems::PressableState::HELD)
                    {
                        movement.y -= 1.0;
                    }

                    glm::fquat yaw = glm::angleAxis(glm::radians(transform.Rotation.y), glm::fvec3(0, 1, 0));
                    glm::fquat pitch = glm::angleAxis(glm::radians(transform.Rotation.x), glm::fvec3(1, 0, 0));

                    glm::fquat orientation = yaw * pitch;

                    glm::fvec3 forward = orientation * glm::fvec3(0, 0, -1);
                    glm::fvec3 right = orientation * glm::fvec3(1, 0, 0);
                    glm::fvec3 up = glm::fvec3(0, 1, 0);

                    transform.Position += (forward * movement.z + right * movement.x + up * movement.y) * player.Speed * deltaTime;

                    static glm::dvec2 prevCursor = mouseLayer.GetCursorState().Position;

                    glm::dvec2 cursorPos = mouseLayer.GetCursorState().Position;
                    glm::dvec2 mouseDelta = cursorPos - prevCursor;

                    prevCursor = cursorPos;

                    transform.Rotation.y += static_cast<float>(-mouseDelta.x) * player.Sensitivity;
                    transform.Rotation.x -= static_cast<float>(mouseDelta.y) * player.Sensitivity;

                    if (keyboardLayer.GetKeyState(Systems::Key::Q) == Systems::PressableState::HELD)
                    {
                        transform.Rotation.z += 8.0 * player.Speed * deltaTime;
                    }
                    if (keyboardLayer.GetKeyState(Systems::Key::E) == Systems::PressableState::HELD)
                    {
                        transform.Rotation.z -= 8.0 * player.Speed * deltaTime;
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

                    static double lastScrollY = 0.0;

                    auto scrollState = mouseLayer.GetScrollState();

                    double deltaY = scrollState.Offsets.y - lastScrollY;

                    camera.FOV -= static_cast<float>(deltaY);

                    lastScrollY = scrollState.Offsets.y;
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
                    camera.Projection = glm::perspectiveRH_ZO(glm::radians(camera.FOV), camera.Aspect, camera.NearPlane, camera.FarPlane);

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

            if ((meshBitmask | transformBitmask | bufferBitmask) == (bitmask & (meshBitmask | transformBitmask | bufferBitmask)))
            {
                for (auto [entity, transform, buffer] : mRegistry.GetEntityView<TransformComponent3D, ConstantBufferComponent>(archetype))
                {
                    static float time = 0.0f;
                    time += deltaTime;

                    float scaleAmount = 0.5f + 0.5f * std::sin(time);
                    transform.Scale = {scaleAmount, scaleAmount, scaleAmount};

                    float radius = 1.5f;
                    transform.Position.x = radius * std::cos(time);
                    transform.Position.z = radius * std::sin(time);
                    transform.Position.y = 0.5f * std::sin(time * 2.0f);

                    glm::fmat4 model = glm::mat4(1.0f);
                    model = glm::translate(model, transform.Position);
                    model *= glm::rotate(glm::mat4(1.0f), glm::radians(transform.Rotation.y), glm::vec3(0, 1, 0));
                    model *= glm::rotate(glm::mat4(1.0f), glm::radians(transform.Rotation.x), glm::vec3(1, 0, 0));
                    model *= glm::rotate(glm::mat4(1.0f), glm::radians(transform.Rotation.z), glm::vec3(0, 0, 1));
                    model = glm::scale(model, glm::clamp(transform.Scale, {0.1, 0.1, 0.1}, {1.0, 1.0, 1.0}));

                    Resources::BufferData bufferData = {
                        .Data = &model,
                        .Stride = sizeof(glm::fmat4),
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
        };

        Resources::BufferFormatDescriptor cameraFormat = {
            .Attributes = {
                Resources::BufferAttributeType::F32_4x4,
                Resources::BufferAttributeType::F32_4x4,
            },
        };

        Resources::BufferFormatDescriptor constantFormat = {
            .Attributes = {
                Resources::BufferAttributeType::F32_4x4,
            },
        };

        Resources::ShaderDescriptor vertexShaderDescriptor = {
            .Stage = Resources::ShaderStage::VERTEX,
            .ConstantBufferFormats = {cameraFormat, constantFormat},
            .StorageBufferFormats = {},
            .Path = "assets/shaders/basic.vertex.spv",
            .Function = "VSMain",
        };

        Resources::ShaderDescriptor pixelShaderDescriptor = {
            .Stage = Resources::ShaderStage::PIXEL,
            .ConstantBufferFormats = {},
            .StorageBufferFormats = {},
            .Path = "assets/shaders/basic.pixel.spv",
            .Function = "PSMain",
        };

        Resources::RasterPipelineDescriptor pipelineDescriptor = {
            .Shaders = {vertexShaderDescriptor, pixelShaderDescriptor},
            .VertexBufferFormats = {vertexFormat},
            .RasterState = {
                .Primitive = Resources::PipelinePrimitive::TRIANGLE_LIST,
                .FrontFace = Resources::PipelineFrontFace::CLOCKWISE,
                .PolygonMode = Resources::PipelinePolygonMode::SOLID,
                .FaceCulling = Resources::PipelineFaceCulling::BACKFACE,
            },
            .DepthState = {
                .Read = true,
                .Write = true,
                .Operation = Resources::DepthCompareOperation::LESS,
            },
        };

        mBasicMeshPipeline = mRenderer.CreatePipeline(pipelineDescriptor);
    }
}