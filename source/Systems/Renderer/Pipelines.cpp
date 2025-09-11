#include <Systems/Renderer.hpp>

namespace Systems
{
    Resources::PipelineHandle Renderer::CreatePipeline(const Resources::RasterPipelineDescriptor& descriptor)
    {
        for (const auto& format : descriptor.VertexBufferFormats)
        {
            for (const auto& attribute : format.Attributes)
            {
                switch (attribute)
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
                        throw Debug::Exception(Debug::ErrorCode::INVALID_ARGUMENT, "Resources::PipelineHandle Systems::Renderer::CreatePipeline(const Resources::RasterPipelineDescriptor&):\n"
                                                                                   "invalid argument\n"
                                                                                   "provided attribute type is not supported as a vertex input for a pipeline");
                    }
                    default:
                    {
                        break;
                    }
                }
            }
        }

        bool vertexFound = false;
        bool pixelFound = false;
        bool geometryFound = false;

        for (const auto& shaderDescriptor : descriptor.Shaders)
        {
            switch (shaderDescriptor.Stage)
            {
                case Resources::ShaderStage::VERTEX:
                {
                    if (vertexFound)
                    {
                        throw Debug::Exception(Debug::ErrorCode::INVALID_ARGUMENT,
                                               "Resources::PipelineHandle Systems::Renderer::CreatePipeline(const Resources::RasterPipelineDescriptor&):\n"
                                               "invalid argument\n"
                                               "multiple vertex shaders provided\n"
                                               "ensure exactly one vertex shader is included in the shader list");
                    }

                    vertexFound = true;

                    break;
                }
                case Resources::ShaderStage::PIXEL:
                {
                    if (pixelFound)
                    {
                        throw Debug::Exception(Debug::ErrorCode::INVALID_ARGUMENT,
                                               "Resources::PipelineHandle Systems::Renderer::CreatePipeline(const Resources::RasterPipelineDescriptor&):\n"
                                               "invalid argument\n"
                                               "multiple pixel/fragment shaders provided\n"
                                               "ensure exactly one pixel shader is included in the shader list");
                    }

                    pixelFound = true;

                    break;
                }
                case Resources::ShaderStage::GEOMETRY:
                {
                    if (geometryFound)
                    {
                        throw Debug::Exception(Debug::ErrorCode::INVALID_ARGUMENT,
                                               "Resources::PipelineHandle Systems::Renderer::CreatePipeline(const Resources::RasterPipelineDescriptor&):\n"
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
                                   "Resources::PipelineHandle Systems::Renderer::CreatePipeline(const Resources::RasterPipelineDescriptor&):\n"
                                   "invalid argument\n"
                                   "no vertex shader provided\n"
                                   "a vertex shader must be included in the shader list");
        }

        if (!pixelFound)
        {
            throw Debug::Exception(Debug::ErrorCode::INVALID_ARGUMENT,
                                   "Resources::PipelineHandle Systems::Renderer::CreatePipeline(const Resources::RasterPipelineDescriptor&):\n"
                                   "invalid argument\n"
                                   "no pixel shader provided\n"
                                   "a pixel shader must be included in the shader list");
        }

        Resources::PipelineHandle handle;

        if (mPipelineFreeList.empty())
        {
            handle.ID = mPipelineGenerations.size();
            mPipelineGenerations.push_back(0);
            mRasterPipelineData.Insert(handle.ID, descriptor);
        }
        else
        {
            handle.ID = mPipelineFreeList.back();
            handle.Generation = mPipelineGenerations[handle.ID];
            mPipelineFreeList.pop_back();
            mRasterPipelineData.Insert(handle.ID, descriptor);
        }

        std::visit([&](auto& backend)
                   { backend->CreatePipeline(handle, descriptor); }, mBackend);

        return handle;
    }

    Resources::RasterPipelineDescriptor Renderer::GetRasterPipelineInfo(const Resources::PipelineHandle& handle)
    {
        if (!mRasterPipelineData.Contains(handle.ID) || mPipelineGenerations[handle.ID] != handle.Generation)
        {
            throw Debug::Exception(Debug::ErrorCode::INVALID_ARGUMENT, "Resources::RasterPipelineDescriptor Systems::Renderer::GetRasterPipelineInfo(const Resources::PipelineHandle&):\n"
                                                                       "invalid argument\n"
                                                                       "provided pipeline does not exist or is not a rasterising pipeline");
        }

        return mRasterPipelineData.Get(handle.ID);
    }

    void Renderer::DeletePipeline(const Resources::PipelineHandle& handle)
    {
        if (!mRasterPipelineData.Contains(handle.ID) || mPipelineGenerations[handle.ID] != handle.Generation)
        {
            throw Debug::Exception(Debug::ErrorCode::INVALID_ARGUMENT, "void Systems::Renderer::DeletePipeline(const Resources::PipelineHandle&):\n"
                                                                       "invalid argument\n"
                                                                       "provided pipeline does not exist");
        }

        auto& info = mRasterPipelineData.Get(handle.ID);

        std::visit([&](auto& backend)
                   { backend->DeletePipeline(handle, info); }, mBackend);

        mPipelineGenerations[handle.ID]++;
        mPipelineFreeList.push_back(handle.ID);
        mRasterPipelineData.Remove(handle.ID);
    }
}