#pragma once

namespace Systems
{
    enum class RendererBackend
    {
        OPENGL,
        VULKAN,
    };

    enum class ContextAttribute
    {
        RENDERER,
    };

    template <ContextAttribute A>
    struct ContextAttributeType;

    template <>
    struct ContextAttributeType<ContextAttribute::RENDERER>
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