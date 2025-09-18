#pragma once

#include <Resources/Attachments.hpp>

#include <cstdlib>
#include <limits>
#include <vector>

namespace Resources
{
    struct RenderPassDescriptor
    {
        std::vector<ColourAttachmentDescriptor> ColourAttachments;

        DepthAttachmentDescriptor DepthAttachment;
        StencilAttachmentDescriptor StencilAttachment;
    };

    struct RenderPassData
    {
        std::vector<ColourAttachmentDescriptor> ColourAttachments;

        DepthAttachmentDescriptor DepthAttachment;
        StencilAttachmentDescriptor StencilAttachment;

        void* BackendMetadata = nullptr;
    };

    struct RenderPassHandle
    {
        std::size_t ID = std::numeric_limits<std::size_t>::max();
        std::size_t Generation = std::numeric_limits<std::size_t>::max();
    };
}