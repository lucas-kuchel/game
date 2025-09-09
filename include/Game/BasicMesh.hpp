#pragma once

#include <Systems/Registry.hpp>
#include <Systems/Renderer.hpp>
#include <Systems/Window.hpp>

#include <Debug/Exception.hpp>
#include <Debug/Logging.hpp>

#include <Game/Transform.hpp>

#include <glm/glm.hpp>

namespace Game
{
    struct RenderableComponent
    {
        Resources::SubmissionHandle Handle;
    };

    struct BasicMeshVertex
    {
        glm::fvec3 Position;
        glm::fvec4 Colour;
    };

    using BasicMeshTriangle = glm::uvec3;

    struct BasicMeshComponent
    {
        std::vector<BasicMeshVertex> Vertices;
        std::vector<BasicMeshTriangle> Triangles;

        Resources::BufferHandle VertexBuffer;
        Resources::BufferHandle IndexBuffer;
    };
}