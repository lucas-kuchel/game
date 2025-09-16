#include <Systems/Renderer.hpp>

#include <Systems/Renderer/Metal.hpp>
#include <Systems/Renderer/OpenGL.hpp>
#include <Systems/Renderer/Vulkan.hpp>

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
                    case Resources::BufferAttribute::F32_2x2:
                    case Resources::BufferAttribute::F32_2x3:
                    case Resources::BufferAttribute::F32_2x4:
                    case Resources::BufferAttribute::F32_3x2:
                    case Resources::BufferAttribute::F32_3x3:
                    case Resources::BufferAttribute::F32_3x4:
                    case Resources::BufferAttribute::F32_4x2:
                    case Resources::BufferAttribute::F32_4x3:
                    case Resources::BufferAttribute::F32_4x4:
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

        Resources::PipelineHandle handle;

        if (mRasterPipelines.FreeList.empty())
        {
            handle.ID = mRasterPipelines.Generations.size();
            mRasterPipelines.Generations.push_back(0);
        }
        else
        {
            handle.ID = mRasterPipelines.FreeList.back();
            handle.Generation = mRasterPipelines.Generations[handle.ID];
            mRasterPipelines.FreeList.pop_back();
        }

        auto& info = mRasterPipelines.Data.Insert(handle.ID, Resources::RasterPipelineData());

        info.BackendMetadata = nullptr;
        info.CullFace = descriptor.CullFace;
        info.FrontFace = descriptor.FrontFace;
        info.DepthComparison = descriptor.DepthComparison;
        info.DepthTest = descriptor.DepthTest;
        info.DepthWrite = descriptor.DepthWrite;
        info.FillMode = descriptor.FillMode;
        info.PixelShader = descriptor.PixelShader;
        info.VertexShader = descriptor.VertexShader;
        info.Topology = descriptor.Topology;
        info.VertexBufferFormats = descriptor.VertexBufferFormats;

        std::visit([&](auto& backend)
                   { backend->CreatePipeline(info); }, mBackend);

        return handle;
    }

    Resources::RasterPipelineData Renderer::GetRasterPipelineData(const Resources::PipelineHandle& handle)
    {
        if (!mRasterPipelines.Data.Contains(handle.ID) || mRasterPipelines.Generations[handle.ID] != handle.Generation)
        {
            throw Debug::Exception(Debug::ErrorCode::INVALID_ARGUMENT, "Resources::RasterPipelineDescriptor Systems::Renderer::GetRasterPipelineInfo(const Resources::PipelineHandle&):\n"
                                                                       "invalid argument\n"
                                                                       "provided pipeline does not exist or is not a rasterising pipeline");
        }

        return mRasterPipelines.Data.Get(handle.ID);
    }

    void Renderer::DeletePipeline(const Resources::PipelineHandle& handle)
    {
        if (!mRasterPipelines.Data.Contains(handle.ID) || mRasterPipelines.Generations[handle.ID] != handle.Generation)
        {
            throw Debug::Exception(Debug::ErrorCode::INVALID_ARGUMENT, "void Systems::Renderer::DeletePipeline(const Resources::PipelineHandle&):\n"
                                                                       "invalid argument\n"
                                                                       "provided pipeline does not exist");
        }

        auto& info = mRasterPipelines.Data.Get(handle.ID);

        std::visit([&](auto& backend)
                   { backend->DeletePipeline(info); }, mBackend);

        mRasterPipelines.Generations[handle.ID]++;
        mRasterPipelines.FreeList.push_back(handle.ID);
        mRasterPipelines.Data.Remove(handle.ID);
    }
}