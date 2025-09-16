#pragma once

#include <Debug/Exception.hpp>
#include <Systems/Context.hpp>

#include <functional>
#include <string>
#include <string_view>
#include <vector>

#include <glm/glm.hpp>

struct VkInstance_T;
using VkInstance = VkInstance_T*;

struct VkSurfaceKHR_T;
using VkSurfaceKHR = VkSurfaceKHR_T*;

struct VkAllocationCallbacks;

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
        COCOA_LAYER,

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
        Space,
        LeftControl,
        RightControl,
        LeftSuper,
        RightSuper,
        LeftAlt,
        RightAlt,
        Capslock,
        Tab,
        Escape,
        LeftShift,
        RightShift,
        Home,
        End,
        PageUp,
        PageDown,
        Insert,
        Delete,
        Enter,
        Backspace,
        Tilde,
        Equal,
        Minus,
        NumpadPlus,
        NumpadMinus,
        Forwardslash,
        Backslash,
        Semicolon,
        Apostrophe,
        Comma,
        Period,
        Row0,
        Row1,
        Row2,
        Row3,
        Row4,
        Row5,
        Row6,
        Row7,
        Row8,
        Row9,
        Numpad0,
        Numpad1,
        Numpad2,
        Numpad3,
        Numpad4,
        Numpad5,
        Numpad6,
        Numpad7,
        Numpad8,
        Numpad9,
        UpArrow,
        DownArrow,
        LeftArrow,
        RightArrow,
    };

    enum class MouseButton
    {
        Left,
        Right,
        Middle,
        Forward,
        Back,
    };

    enum class PressableState
    {
        Pressed,
        Released,
        Held,
        None,
    };

    enum class CursorMode
    {
        Normal,
        Hidden,
        Disabled,
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

    template <>
    class WindowInteractionLayer<WindowInteractive::COCOA_LAYER>
    {
    private:
        friend class Window;

        explicit WindowInteractionLayer(void* handle);

        void* mHandle;

    public:
        void* GetCocoaWindow();
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
        void MTL_Init();

        template <WindowInteractive I>
        void ValidateInteractionLayerRequest() const
        {
            switch (I)
            {
                case WindowInteractive::OPENGL_LAYER:
                {
                    if (mContext.get().Get<ContextAttribute::RENDERER>() != RendererBackend::OPENGL)
                    {
                        throw Debug::Exception(Debug::ErrorCode::INVALID_ARGUMENT, "Systems::WindowInteractionLayer<WindowInteractive::OPENGL> Systems::Window::CreateInteractionLayer<WindowInteractive::OPENGL_LAYER>():\n"
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
                case WindowInteractive::COCOA_LAYER:
                {
                    if (mContext.get().Get<ContextAttribute::RENDERER>() != RendererBackend::METAL)
                    {
                        throw Debug::Exception(Debug::ErrorCode::INVALID_ARGUMENT, "Systems::WindowInteractionLayer<WindowInteractive::VULKAN> Systems::Window::CreateInteractionLayer<WindowInteractive::COCOA_LAYER>():\n"
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