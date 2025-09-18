#pragma once

namespace Systems
{
    enum class RendererBackend
    {
        DirectX12,
        Vulkan,
        Metal,
    };

    enum class ContextAttribute
    {
        Renderer,
    };

    template <ContextAttribute A>
    struct ContextAttributeType;

    template <>
    struct ContextAttributeType<ContextAttribute::Renderer>
    {
        using Type = RendererBackend;
    };

    struct ContextDescriptor
    {
        RendererBackend Renderer;
    };

    class Context
    {
    public:
        Context(const ContextDescriptor& descriptor);
        ~Context();

        void Update();

        template <ContextAttribute A>
        const ContextAttributeType<A>::Type& Get() const;

    private:
        RendererBackend mRenderer;
    };
}