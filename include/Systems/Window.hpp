#pragma once

#include <Debug/Exception.hpp>
#include <Systems/Context.hpp>

#include <vulkan/vulkan.h>

#include <string>
#include <string_view>
#include <vector>

#include <glm/glm.hpp>

namespace Systems
{
    using WindowSize = glm::uvec2;
    using WindowTitle = std::string;
    using WindowContext = Context;

    enum class WindowAttribute
    {
        TITLE,
        SIZE,
        STATUS,
    };

    enum class WindowStatus
    {
        ACTIVE,
        INACTIVE,
    };

    enum class WindowInteractive
    {
        OPENGL_LAYER,
        VULKAN_LAYER,

        KEYBOARD_LAYER,
        MOUSE_LAYER,
    };

    enum class Key
    {
        A,
        B,
        C,
        D,
        E,
        F,
        G,
        H,
        I,
        J,
        K,
        L,
        M,
        N,
        O,
        P,
        Q,
        R,
        S,
        T,
        U,
        V,
        W,
        X,
        Y,
        Z,
        F1,
        F2,
        F3,
        F4,
        F5,
        F6,
        F7,
        F8,
        F9,
        F10,
        F11,
        F12,
        SPACE,
        LCTRL,
        RCTRL,
        LSUPER,
        RSUPER,
        LALT,
        RALT,
        CAPSLOCK,
        TAB,
        ESC,
        LSHIFT,
        RSHIFT,
        HOME,
        END,
        PGUP,
        PGDN,
        INSERT,
        DEL,
        ENTER,
        BACKSPACE,
        TILDE,
        PLUS,
        MINUS,
        FSLASH,
        BSLASH,
        SEMICOLON,
        QUOTE,
        COMMA,
        PERIOD,
        ROW_0,
        ROW_1,
        ROW_2,
        ROW_3,
        ROW_4,
        ROW_5,
        ROW_6,
        ROW_7,
        ROW_8,
        ROW_9,
        PAD_0,
        PAD_1,
        PAD_2,
        PAD_3,
        PAD_4,
        PAD_5,
        PAD_6,
        PAD_7,
        PAD_8,
        PAD_9,
        UP,
        DOWN,
        LEFT,
        RIGHT,
    };

    enum class MouseButton
    {
        LEFT,
        RIGHT,
        MIDDLE,
        FORWARD,
        BACK,
    };

    enum class PressableState
    {
        PRESSED,
        RELEASED,
        HELD,
        NONE,
    };

    enum class CursorMode
    {
        NORMAL,
        HIDDEN,
        DISABLED,
    };

    struct ScrollState
    {
        glm::dvec2 Offsets;
    };

    struct CursorState
    {
        glm::dvec2 Position;

        CursorMode Mode;
    };

    template <WindowInteractive>
    class WindowInteractionLayer;

    template <>
    class WindowInteractionLayer<WindowInteractive::OPENGL_LAYER>
    {
    private:
        friend class Window;

        explicit WindowInteractionLayer(void* handle);

        void* mHandle;

    public:
        void MakeContextCurrent();
        void SwapBuffers();
        void SetSwapInterval(int interval);
    };

    template <>
    class WindowInteractionLayer<WindowInteractive::VULKAN_LAYER>
    {
    private:
        friend class Window;

        explicit WindowInteractionLayer(void* handle);

        void* mHandle;

    public:
        std::vector<std::string_view> GetRequiredInstanceExtensions();
        VkSurfaceKHR CreateWindowSurface(VkInstance instance, const VkAllocationCallbacks* allocator = nullptr);
    };

    struct InputState;

    template <>
    class WindowInteractionLayer<WindowInteractive::KEYBOARD_LAYER>
    {
    private:
        friend class Window;

        explicit WindowInteractionLayer(void* handle);

        void* mHandle;
        InputState* mState;

    public:
        PressableState GetKeyState(Key key) const;
    };

    template <>
    class WindowInteractionLayer<WindowInteractive::MOUSE_LAYER>
    {
    private:
        friend class Window;

        explicit WindowInteractionLayer(void* handle);

        void* mHandle;
        InputState* mState;

    public:
        PressableState GetButtonState(MouseButton button) const;

        ScrollState GetScrollState() const;
        CursorState GetCursorState() const;

        void SetCursorState(const CursorState& state);
    };

    template <WindowAttribute A>
    struct WindowAttributeType;

    template <>
    struct WindowAttributeType<WindowAttribute::SIZE>
    {
        using Type = WindowSize;
    };

    template <>
    struct WindowAttributeType<WindowAttribute::TITLE>
    {
        using Type = WindowTitle;
    };

    template <>
    struct WindowAttributeType<WindowAttribute::STATUS>
    {
        using Type = WindowStatus;
    };

    struct WindowDescriptor
    {
        std::reference_wrapper<WindowContext> Context;

        WindowTitle Title = "New Window";
        WindowSize Size = {640u, 480u};
    };

    class Window
    {
    public:
        Window(const WindowDescriptor& descriptor);
        ~Window();

        Window(const Window&) = delete;
        Window& operator=(const Window&) = delete;
        Window(Window&&) = delete;
        Window& operator=(Window&&) = delete;

        void Update();

        template <WindowAttribute A>
        const WindowAttributeType<A>::Type& Get() const;

        template <WindowAttribute A>
        void Set(const WindowAttributeType<A>::Type& value);

        template <WindowInteractive I>
        WindowInteractionLayer<I> CreateInteractionLayer()
        {
            ValidateInteractionLayerRequest<I>();

            return WindowInteractionLayer<I>(mHandle);
        }

    private:
        void GL_Init();
        void VK_Init();

        template <WindowInteractive I>
        void ValidateInteractionLayerRequest() const
        {
            switch (I)
            {
                case WindowInteractive::OPENGL_LAYER:
                {
                    if (mContext.get().Get<ContextAttribute::RENDERER>() != RendererBackend::OPENGL)
                    {
                        throw Debug::Exception(Debug::ErrorCode::INVALID_ARGUMENT, "Systems::WindowInteractionLayer<WindowInteractive::OPENGL> Systems::Window::CreateInteractionLayer<WindowInteractive::OPENGL>():\n"
                                                                                   "invalid argument error\n"
                                                                                   "context renderer API and interaction layer API do not match");
                    }

                    break;
                }
                case WindowInteractive::VULKAN_LAYER:
                {
                    if (mContext.get().Get<ContextAttribute::RENDERER>() != RendererBackend::VULKAN)
                    {
                        throw Debug::Exception(Debug::ErrorCode::INVALID_ARGUMENT, "Systems::WindowInteractionLayer<WindowInteractive::VULKAN> Systems::Window::CreateInteractionLayer<WindowInteractive::VULKAN>():\n"
                                                                                   "invalid argument error\n"
                                                                                   "context renderer API and interaction layer API do not match");
                    }

                    break;
                }
                default:
                {
                    break;
                }
            }
        }

        std::reference_wrapper<WindowContext> mContext;

        WindowTitle mTitle;
        WindowSize mSize;
        WindowStatus mStatus = WindowStatus::INACTIVE;

        bool mTitleDirty = false;
        bool mSizeDirty = false;
        bool mStatusDirty = false;

        void* mHandle = nullptr;
    };
}