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
        Resources::BufferHandle ConstantBuffer;

        Resources::SubmissionHandle Submission;

        static void Create(BasicMeshComponent& mesh, Systems::Renderer& renderer, Resources::PipelineHandle& pipeline);
        static void Update(BasicMeshComponent& mesh, Systems::Renderer& renderer);
        static void Delete(BasicMeshComponent& mesh, Systems::Renderer& renderer);

        static void Transform(BasicMeshComponent& mesh, TransformComponent2D& transform, Systems::Renderer& renderer);
        static void Transform(BasicMeshComponent& mesh, TransformComponent3D& transform, Systems::Renderer& renderer);
    };
}