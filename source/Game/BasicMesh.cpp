#include <Game/BasicMesh.hpp>

namespace Game
{
    void BasicMeshComponent::Create(BasicMeshComponent& mesh, Systems::Renderer& renderer, Resources::PipelineHandle& pipeline)
    {
        Resources::BufferDescriptor vertexDescriptor = {
            .Type = Resources::BufferType::VERTEX,
            .Attributes = {{.Type = Resources::BufferAttributeType::R32G32B32_FLOAT, .Slot = 0},
                           {.Type = Resources::BufferAttributeType::R32G32B32A32_FLOAT, .Slot = 1}},
            .Slot = 0,
            .Size = mesh.Vertices.size() * sizeof(BasicMeshVertex),
        };

        Resources::BufferDescriptor indexDescriptor = {
            .Type = Resources::BufferType::INDEX,
            .Attributes = {{.Type = Resources::BufferAttributeType::R32_UINT, .Slot = 0}},
            .Slot = 0,
            .Size = mesh.Triangles.size() * sizeof(BasicMeshTriangle),
        };

        Resources::BufferDescriptor constantDescriptor = {
            .Type = Resources::BufferType::INDEX,
            .Attributes = {{.Type = Resources::BufferAttributeType::F32_4x4, .Slot = 0}},
            .Slot = 1,
            .Size = sizeof(glm::fmat4),
        };

        mesh.VertexBuffer = renderer.CreateBuffer(vertexDescriptor);
        mesh.IndexBuffer = renderer.CreateBuffer(indexDescriptor);

        renderer.SetBufferData(mesh.VertexBuffer, {.Data = mesh.Vertices.data(), .Size = sizeof(mesh.Vertices), .Offset = 0});
        renderer.SetBufferData(mesh.IndexBuffer, {.Data = mesh.Triangles.data(), .Size = sizeof(mesh.Triangles), .Offset = 0});

        Resources::SubmissionDescriptor submissionDescriptor = {
            .VertexBuffers = {mesh.VertexBuffer},
            .BufferInputs = {mesh.ConstantBuffer},
            .IndexBuffer = mesh.IndexBuffer,
            .Pipeline = pipeline,
        };

        mesh.Submission = renderer.CreateSubmission(submissionDescriptor);
    }

    void BasicMeshComponent::Update(BasicMeshComponent& mesh, Systems::Renderer& renderer)
    {
        renderer.SetBufferData(mesh.VertexBuffer, {.Data = mesh.Vertices.data(), .Size = sizeof(mesh.Vertices), .Offset = 0});
        renderer.SetBufferData(mesh.IndexBuffer, {.Data = mesh.Triangles.data(), .Size = sizeof(mesh.Triangles), .Offset = 0});
    }
    std::is_St void BasicMeshComponent::Delete(BasicMeshComponent& mesh, Systems::Renderer& renderer)
    {
        renderer.DeleteBuffer(mesh.VertexBuffer);
        renderer.DeleteBuffer(mesh.IndexBuffer);

        renderer.DeleteSubmission(mesh.Submission);
    }
}