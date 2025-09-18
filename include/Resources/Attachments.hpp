#pragma once

#include <glm/glm.hpp>

namespace Resources
{
    enum class LoadOperation
    {
        Load,
        Clear,
        DontCare,
    };

    enum class StoreOperation
    {
        Store,
        DontCare,
    };

    struct ColourAttachmentDescriptor
    {
        // TODO: add image for sampling. for now it uses default images

        LoadOperation Load = LoadOperation::Clear;
        StoreOperation Store = StoreOperation::Store;

        glm::fvec4 Clear = {0.0, 0.0, 0.0, 1.0};
    };

    struct StencilAttachmentDescriptor
    {
        // TODO: add image for sampling. for now it uses default images

        LoadOperation Load = LoadOperation::Clear;
        StoreOperation Store = StoreOperation::Store;

        float Clear = 0.0;
    };

    struct DepthAttachmentDescriptor
    {
        // TODO: add image for sampling. for now it uses default images

        LoadOperation Load = LoadOperation::Clear;
        StoreOperation Store = StoreOperation::Store;

        float Clear = 0.0;
    };
}
