#include <Systems/Renderer.hpp>

namespace Systems
{
    Resources::PipelineHandle Renderer::CreatePipeline(const Resources::PipelineDescriptor& descriptor)
    {
        Resources::PipelineHandle handle;

        for (const auto& attribute : descriptor.VertexInput)
        {
            switch (attribute.Type)
            {
                case Resources::BufferAttributeType::F32_2x2:
                case Resources::BufferAttributeType::F32_2x3:
                case Resources::BufferAttributeType::F32_2x4:
                case Resources::BufferAttributeType::F32_3x2:
                case Resources::BufferAttributeType::F32_3x3:
                case Resources::BufferAttributeType::F32_3x4:
                case Resources::BufferAttributeType::F32_4x2:
                case Resources::BufferAttributeType::F32_4x3:
                case Resources::BufferAttributeType::F32_4x4:
                {
                    throw Debug::Exception(Debug::ErrorCode::INVALID_ARGUMENT, "Resources::PipelineHandle Systems::Renderer::CreatePipeline(const Resources::PipelineDescriptor&):\n"
                                                                               "invalid argument\n"
                                                                               "provided attribute type is not supported as a vertex input for a pipeline");
                }
                default:
                {
                    break;
                }
            }
        }

        auto size = descriptor.VertexInput.size();

        for (std::size_t i = 0; i < size; i++)
        {
            for (std::size_t j = i; j < size; j++)
            {
                if (i == j)
                {
                    continue;
                }

                auto& one = descriptor.VertexInput[i];
                auto& two = descriptor.VertexInput[j];

                if (one.Slot == two.Slot)
                {
                    throw Debug::Exception(Debug::ErrorCode::INVALID_ARGUMENT, "Resources::PipelineHandle Systems::Renderer::CreatePipeline(const Resources::PipelineDescriptor&):\n"
                                                                               "invalid argument\n"
                                                                               "two or more attributes are sharing slot {}",
                                           one.Slot);
                }
            }
        }

        for (const auto& buffer : descriptor.InputBuffers)
        {
            if (!mBufferData.Contains(buffer.ID) || mBufferGenerations[buffer.ID] != buffer.Generation)
            {
                throw Debug::Exception(Debug::ErrorCode::INVALID_ARGUMENT, "Resources::PipelineHandle Systems::Renderer::CreatePipeline(const Resources::PipelineDescriptor&):\n"
                                                                           "invalid argument\n"
                                                                           "provided input buffer does not exist");
            }

            auto& data = mBufferData.Get(buffer.ID);

            if (data.Type == Resources::BufferType::INDEX || data.Type == Resources::BufferType::VERTEX)
            {
                throw Debug::Exception(Debug::ErrorCode::INVALID_ARGUMENT, "Resources::PipelineHandle Systems::Renderer::CreatePipeline(const Resources::PipelineDescriptor&):\n"
                                                                           "invalid argument\n"
                                                                           "input buffers must be of type Resources::BufferType::CONSTANT or Resources::BufferType::STORAGE");
            }
        }

        bool vertexFound = false;
        bool pixelFound = false;
        bool geometryFound = false;

        for (const auto& shaderDescriptor : descriptor.Shaders)
        {
            switch (shaderDescriptor.Type)
            {
                case Resources::ShaderType::VERTEX:
                {
                    if (vertexFound)
                    {
                        throw Debug::Exception(Debug::ErrorCode::INVALID_ARGUMENT,
                                               "Resources::PipelineHandle Systems::Renderer::CreatePipeline(const Resources::PipelineDescriptor&):\n"
                                               "invalid argument\n"
                                               "multiple vertex shaders provided\n"
                                               "ensure exactly one vertex shader is included in the shader list");
                    }

                    vertexFound = true;

                    break;
                }
                case Resources::ShaderType::PIXEL:
                {
                    if (pixelFound)
                    {
                        throw Debug::Exception(Debug::ErrorCode::INVALID_ARGUMENT,
                                               "Resources::PipelineHandle Systems::Renderer::CreatePipeline(const Resources::PipelineDescriptor&):\n"
                                               "invalid argument\n"
                                               "multiple pixel/fragment shaders provided\n"
                                               "ensure exactly one pixel shader is included in the shader list");
                    }

                    pixelFound = true;

                    break;
                }
                case Resources::ShaderType::GEOMETRY:
                {
                    if (geometryFound)
                    {
                        throw Debug::Exception(Debug::ErrorCode::INVALID_ARGUMENT,
                                               "Resources::PipelineHandle Systems::Renderer::CreatePipeline(const Resources::PipelineDescriptor&):\n"
                                               "invalid argument\n"
                                               "multiple geometry shaders provided\n"
                                               "ensure at most one geometry shader is included in the shader list");
                    }

                    geometryFound = true;

                    break;
                }
            }
        }

        if (!vertexFound)
        {
            throw Debug::Exception(Debug::ErrorCode::INVALID_ARGUMENT,
                                   "Resources::PipelineHandle Systems::Renderer::CreatePipeline(const Resources::PipelineDescriptor&):\n"
                                   "invalid argument\n"
                                   "no vertex shader provided\n"
                                   "a vertex shader must be included in the shader list");
        }

        if (!pixelFound)
        {
            throw Debug::Exception(Debug::ErrorCode::INVALID_ARGUMENT,
                                   "Resources::PipelineHandle Systems::Renderer::CreatePipeline(const Resources::PipelineDescriptor&):\n"
                                   "invalid argument\n"
                                   "no pixel shader provided\n"
                                   "a pixel shader must be included in the shader list");
        }

        if (mPipelineFreeList.empty())
        {
            handle.ID = mPipelineGenerations.size();
            mPipelineGenerations.push_back(0);
            mPipelineData.Insert(handle.ID, descriptor);
        }
        else
        {
            handle.ID = mPipelineFreeList.back();
            handle.Generation = mPipelineGenerations[handle.ID];
            mPipelineFreeList.pop_back();
            mPipelineData.Insert(handle.ID, descriptor);
        }

        std::visit([&](auto& backend)
                   { backend->CreatePipeline(handle, descriptor); }, mBackend);

        return handle;
    }

    Resources::PipelineDescriptor Renderer::GetPipelineInfo(const Resources::PipelineHandle& handle)
    {
        if (!mPipelineData.Contains(handle.ID) || mPipelineGenerations[handle.ID] != handle.Generation)
        {
            throw Debug::Exception(Debug::ErrorCode::INVALID_ARGUMENT, "Resources::PipelineDescriptor Systems::Renderer::GetPipelineInfo(const Resources::PipelineHandle&):\n"
                                                                       "invalid argument\n"
                                                                       "provided pipeline does not exist");
        }

        return mPipelineData.Get(handle.ID);
    }

    void Renderer::DeletePipeline(const Resources::PipelineHandle& handle)
    {
        if (!mPipelineData.Contains(handle.ID) || mPipelineGenerations[handle.ID] != handle.Generation)
        {
            throw Debug::Exception(Debug::ErrorCode::INVALID_ARGUMENT, "void Systems::Renderer::DeletePipeline(const Resources::PipelineHandle&):\n"
                                                                       "invalid argument\n"
                                                                       "provided pipeline does not exist");
        }

        std::visit([&](auto& backend)
                   { backend->DeletePipeline(handle); }, mBackend);

        mPipelineGenerations[handle.ID]++;
        mPipelineFreeList.push_back(handle.ID);
        mPipelineData.Remove(handle.ID);
    }
}