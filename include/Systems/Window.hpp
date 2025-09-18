#pragma once

#include <Debug/Exception.hpp>
#include <Systems/Context.hpp>

#include <queue>
#include <string>
#include <string_view>
#include <vector>

#include <glm/glm.hpp>

struct VkInstance_T;
using VkInstance = VkInstance_T*;

struct VkSurfaceKHR_T;
using VkSurfaceKHR = VkSurfaceKHR_T*;

struct VkAllocationCallbacks;

struct GLFWwindow;

namespace Systems
{
    using WindowSize = glm::uvec2;
    using WindowTitle = std::string;
    using WindowContext = Context;

    enum class WindowVisibility
    {
        Fullscreen,
        Windowed,
        Iconified,
    };

    enum class WindowAttribute
    {
        Title,
        Size,
        Visibility,
        Shown,
    };

    enum class WindowInteractive
    {
        OpenGL,
        Vulkan,

        CocoaBackend,
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

    enum class PressableStatus
    {
        Pressed,
        Released,
    };

    enum class CursorMode
    {
        Normal,
        Hidden,
        Disabled,
    };

    enum class WindowEventType
    {
        KeyInput,
        ScancodeInput,
        CharInput,

        MouseButton,
        MouseMove,
        MouseScroll,

        WindowResize,
        WindowFocus,
        WindowVisibility,
        WindowClose
    };

    struct KeyEvent
    {
        Key Input;
        PressableStatus State;
    };

    struct ScancodeEvent
    {
        std::int32_t Scancode;
        PressableStatus State;
    };

    struct CharEvent
    {
        std::uint32_t Codepoint;
    };

    struct MouseButtonEvent
    {
        MouseButton Button;
        PressableStatus State;
    };

    struct MouseMoveEvent
    {
        glm::dvec2 Position;
    };

    struct MouseScrollEvent
    {
        glm::dvec2 Offsets;
    };

    struct WindowResizeEvent
    {
        glm::uvec2 Size;
    };

    struct WindowFocusEvent
    {
        bool Focused;
    };

    struct WindowCloseEvent
    {
    };

    struct WindowVisibilityEvent
    {
        WindowVisibility Visibility;
    };

    struct WindowEvent
    {
        WindowEventType Type;

        union
        {
            KeyEvent KeyInput;
            ScancodeEvent ScancodeInput;
            CharEvent CharInput;

            MouseButtonEvent MouseButtonInput;
            MouseMoveEvent MousePositionInput;
            MouseScrollEvent MouseScrollInput;

            WindowResizeEvent ResizeInteraction;
            WindowFocusEvent FocusInteraction;
            WindowVisibilityEvent VisibilityInteraction;
            WindowCloseEvent CloseInteraction;
        };
    };

    template <WindowInteractive>
    class WindowInteractionLayer;

    template <>
    class WindowInteractionLayer<WindowInteractive::OpenGL>
    {
    public:
        void MakeContextCurrent();
        void SwapBuffers();
        void SetSwapInterval(int interval);

    private:
        friend class Window;

        explicit WindowInteractionLayer(void* handle);

        void* mHandle;
    };

    template <>
    class WindowInteractionLayer<WindowInteractive::Vulkan>
    {
    public:
        std::vector<std::string_view> GetRequiredInstanceExtensions();
        VkSurfaceKHR CreateWindowSurface(VkInstance instance, const VkAllocationCallbacks* allocator = nullptr);

    private:
        friend class Window;

        explicit WindowInteractionLayer(void* handle);

        void* mHandle;
    };

    template <>
    class WindowInteractionLayer<WindowInteractive::CocoaBackend>
    {
    public:
        void* GetCocoaWindow();

    private:
        friend class Window;

        explicit WindowInteractionLayer(void* handle);

        void* mHandle;
    };

    template <WindowAttribute A>
    struct WindowAttributeType;

    template <>
    struct WindowAttributeType<WindowAttribute::Size>
    {
        using Type = WindowSize;
    };

    template <>
    struct WindowAttributeType<WindowAttribute::Title>
    {
        using Type = WindowTitle;
    };

    template <>
    struct WindowAttributeType<WindowAttribute::Visibility>
    {
        using Type = WindowVisibility;
    };

    template <>
    struct WindowAttributeType<WindowAttribute::Shown>
    {
        using Type = bool;
    };

    struct WindowDescriptor
    {
        WindowContext& Context;

        WindowTitle Title = "New Window";
        WindowSize Size = {640u, 480u};
        WindowVisibility Visibility = WindowVisibility::Windowed;

        bool Shown = true;
    };

    class Window
    {
    public:
        Window(const WindowDescriptor& descriptor);
        ~Window();

        std::queue<WindowEvent>& QueryEvents();

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
        void InitialiseHintsOpenGL();
        void InitialiseHintsDefault();

        template <WindowInteractive I>
        void ValidateInteractionLayerRequest() const
        {
            auto& renderer = mContext.Get<ContextAttribute::Renderer>();

            switch (I)
            {
                case WindowInteractive::OpenGL:
                    if (renderer != RendererBackend::OpenGL)
                    {
                        throw Debug::Exception(Debug::ErrorCode::INVALID_ARGUMENT, "Systems::WindowInteractionLayer<WindowInteractive::OPENGL> Systems::Window::CreateInteractionLayer<WindowInteractive::OPENGL_LAYER>():\n"
                                                                                   "invalid argument error\n"
                                                                                   "context renderer API and interaction layer API do not match");
                    }
                    break;
                case WindowInteractive::Vulkan:
                    if (renderer != RendererBackend::Vulkan)
                    {
                        throw Debug::Exception(Debug::ErrorCode::INVALID_ARGUMENT, "Systems::WindowInteractionLayer<WindowInteractive::VULKAN> Systems::Window::CreateInteractionLayer<WindowInteractive::VULKAN>():\n"
                                                                                   "invalid argument error\n"
                                                                                   "context renderer API and interaction layer API do not match");
                    }
                    break;
                case WindowInteractive::CocoaBackend:
                    if (renderer != RendererBackend::Metal)
                    {
                        throw Debug::Exception(Debug::ErrorCode::INVALID_ARGUMENT, "Systems::WindowInteractionLayer<WindowInteractive::VULKAN> Systems::Window::CreateInteractionLayer<WindowInteractive::COCOA_LAYER>():\n"
                                                                                   "invalid argument error\n"
                                                                                   "context renderer API and interaction layer API do not match");
                    }
                    break;
                default:
                    break;
            }
        }

        WindowContext& mContext;

        WindowTitle mTitle;
        WindowSize mSize;
        WindowVisibility mVisibility;
        bool mShown;

        bool mTitleDirty = false;
        bool mSizeDirty = false;
        bool mVisibilityDirty = false;
        bool mShownDirty = false;

        GLFWwindow* mHandle = nullptr;

        std::queue<WindowEvent> mEventQueue;

        template <WindowInteractive>
        friend class WindowInteractionLayer;

        static void SizeCallback(GLFWwindow* window, int width, int height);
        static void CloseCallback(GLFWwindow* window);
        static void FocusCallback(GLFWwindow* window, int focused);
        static void IconifyCallback(GLFWwindow* window, int iconified);
        static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
        static void CharCallback(GLFWwindow* window, std::uint32_t character);
        static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
        static void MousePositionCallback(GLFWwindow* window, double x, double y);
        static void MouseScrollCallback(GLFWwindow* window, double x, double y);
    };
}