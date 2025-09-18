#include <Systems/Window.hpp>

#include <Debug/Exception.hpp>

#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>

#if defined(PLATFORM_APPLE)
#define GLFW_EXPOSE_NATIVE_COCOA

#include <GLFW/glfw3native.h>
#endif

namespace Systems
{
    Key MapKey(int glfwKey)
    {
        switch (glfwKey)
        {
            case GLFW_KEY_A:
                return Key::A;
            case GLFW_KEY_B:
                return Key::B;
            case GLFW_KEY_C:
                return Key::C;
            case GLFW_KEY_D:
                return Key::D;
            case GLFW_KEY_E:
                return Key::E;
            case GLFW_KEY_F:
                return Key::F;
            case GLFW_KEY_G:
                return Key::G;
            case GLFW_KEY_H:
                return Key::H;
            case GLFW_KEY_I:
                return Key::I;
            case GLFW_KEY_J:
                return Key::J;
            case GLFW_KEY_K:
                return Key::K;
            case GLFW_KEY_L:
                return Key::L;
            case GLFW_KEY_M:
                return Key::M;
            case GLFW_KEY_N:
                return Key::N;
            case GLFW_KEY_O:
                return Key::O;
            case GLFW_KEY_P:
                return Key::P;
            case GLFW_KEY_Q:
                return Key::Q;
            case GLFW_KEY_R:
                return Key::R;
            case GLFW_KEY_S:
                return Key::S;
            case GLFW_KEY_T:
                return Key::T;
            case GLFW_KEY_U:
                return Key::U;
            case GLFW_KEY_V:
                return Key::V;
            case GLFW_KEY_W:
                return Key::W;
            case GLFW_KEY_X:
                return Key::X;
            case GLFW_KEY_Y:
                return Key::Y;
            case GLFW_KEY_Z:
                return Key::Z;
            case GLFW_KEY_F1:
                return Key::F1;
            case GLFW_KEY_F2:
                return Key::F2;
            case GLFW_KEY_F3:
                return Key::F3;
            case GLFW_KEY_F4:
                return Key::F4;
            case GLFW_KEY_F5:
                return Key::F5;
            case GLFW_KEY_F6:
                return Key::F6;
            case GLFW_KEY_F7:
                return Key::F7;
            case GLFW_KEY_F8:
                return Key::F8;
            case GLFW_KEY_F9:
                return Key::F9;
            case GLFW_KEY_F10:
                return Key::F10;
            case GLFW_KEY_F11:
                return Key::F11;
            case GLFW_KEY_F12:
                return Key::F12;
            case GLFW_KEY_SPACE:
                return Key::Space;
            case GLFW_KEY_LEFT_CONTROL:
                return Key::LeftControl;
            case GLFW_KEY_RIGHT_CONTROL:
                return Key::RightControl;
            case GLFW_KEY_LEFT_SUPER:
                return Key::LeftSuper;
            case GLFW_KEY_RIGHT_SUPER:
                return Key::RightSuper;
            case GLFW_KEY_LEFT_ALT:
                return Key::LeftAlt;
            case GLFW_KEY_RIGHT_ALT:
                return Key::RightAlt;
            case GLFW_KEY_CAPS_LOCK:
                return Key::Capslock;
            case GLFW_KEY_TAB:
                return Key::Tab;
            case GLFW_KEY_ESCAPE:
                return Key::Escape;
            case GLFW_KEY_LEFT_SHIFT:
                return Key::LeftShift;
            case GLFW_KEY_RIGHT_SHIFT:
                return Key::RightShift;
            case GLFW_KEY_HOME:
                return Key::Home;
            case GLFW_KEY_END:
                return Key::End;
            case GLFW_KEY_PAGE_UP:
                return Key::PageUp;
            case GLFW_KEY_PAGE_DOWN:
                return Key::PageDown;
            case GLFW_KEY_INSERT:
                return Key::Insert;
            case GLFW_KEY_DELETE:
                return Key::Delete;
            case GLFW_KEY_ENTER:
                return Key::Enter;
            case GLFW_KEY_BACKSPACE:
                return Key::Backspace;
            case GLFW_KEY_GRAVE_ACCENT:
                return Key::Tilde;
            case GLFW_KEY_EQUAL:
                return Key::Equal;
            case GLFW_KEY_MINUS:
                return Key::Minus;
            case GLFW_KEY_SLASH:
                return Key::Forwardslash;
            case GLFW_KEY_BACKSLASH:
                return Key::Backslash;
            case GLFW_KEY_SEMICOLON:
                return Key::Semicolon;
            case GLFW_KEY_APOSTROPHE:
                return Key::Apostrophe;
            case GLFW_KEY_COMMA:
                return Key::Comma;
            case GLFW_KEY_PERIOD:
                return Key::Period;
            case GLFW_KEY_0:
                return Key::Row0;
            case GLFW_KEY_1:
                return Key::Row1;
            case GLFW_KEY_2:
                return Key::Row2;
            case GLFW_KEY_3:
                return Key::Row3;
            case GLFW_KEY_4:
                return Key::Row4;
            case GLFW_KEY_5:
                return Key::Row5;
            case GLFW_KEY_6:
                return Key::Row6;
            case GLFW_KEY_7:
                return Key::Row7;
            case GLFW_KEY_8:
                return Key::Row8;
            case GLFW_KEY_9:
                return Key::Row9;
            case GLFW_KEY_KP_ADD:
                return Key::NumpadPlus;
            case GLFW_KEY_KP_SUBTRACT:
                return Key::NumpadMinus;
            case GLFW_KEY_KP_0:
                return Key::Numpad0;
            case GLFW_KEY_KP_1:
                return Key::Numpad1;
            case GLFW_KEY_KP_2:
                return Key::Numpad2;
            case GLFW_KEY_KP_3:
                return Key::Numpad3;
            case GLFW_KEY_KP_4:
                return Key::Numpad4;
            case GLFW_KEY_KP_5:
                return Key::Numpad5;
            case GLFW_KEY_KP_6:
                return Key::Numpad6;
            case GLFW_KEY_KP_7:
                return Key::Numpad7;
            case GLFW_KEY_KP_8:
                return Key::Numpad8;
            case GLFW_KEY_KP_9:
                return Key::Numpad9;
            case GLFW_KEY_UP:
                return Key::UpArrow;
            case GLFW_KEY_DOWN:
                return Key::DownArrow;
            case GLFW_KEY_LEFT:
                return Key::LeftArrow;
            case GLFW_KEY_RIGHT:
                return Key::RightArrow;
        }

        throw;
    }

    MouseButton MapButton(int glfwButton)
    {
        switch (glfwButton)
        {
            case GLFW_MOUSE_BUTTON_LEFT:
                return MouseButton::Left;
            case GLFW_MOUSE_BUTTON_RIGHT:
                return MouseButton::Right;
            case GLFW_MOUSE_BUTTON_MIDDLE:
                return MouseButton::Middle;
            case GLFW_MOUSE_BUTTON_4:
                return MouseButton::Forward;
            case GLFW_MOUSE_BUTTON_5:
                return MouseButton::Back;
        }

        throw;
    }

    WindowInteractionLayer<WindowInteractive::Vulkan>::WindowInteractionLayer(GLFWwindow* handle)
        : mHandle(handle)
    {
        if (!glfwVulkanSupported())
        {
            throw Debug::Exception(Debug::ErrorCode::CONFLICT,
                                   "Systems::WindowInteractionLayer<WindowInteractive::Vulkan>::WindowInteractionLayer(void*):\n"
                                   "conflict error\n"
                                   "Vulkan is unsupported on this system. Cannot create a Vulkan window layer");
        }
    }

    Window::Window(const WindowDescriptor& descriptor)
        : mContext(descriptor.Context), mTitle(descriptor.Title), mSize(descriptor.Size), mVisibility(descriptor.Visibility), mShown(descriptor.Shown)
    {
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        mHandle = glfwCreateWindow(mSize[0], mSize[1], mTitle.c_str(), nullptr, nullptr);

        if (mHandle == nullptr)
        {
            throw Debug::Exception(Debug::ErrorCode::GENERAL_ERROR, "Systems::Window::Window(const WindowDescriptor&):\n"
                                                                    "general error\n"
                                                                    "window creation failed\n"
                                                                    "check window arguments");
        }

        glfwSetWindowUserPointer(mHandle, this);

        glfwSetWindowSizeCallback(mHandle, SizeCallback);
        glfwSetWindowCloseCallback(mHandle, CloseCallback);
        glfwSetWindowFocusCallback(mHandle, FocusCallback);
        glfwSetWindowIconifyCallback(mHandle, IconifyCallback);
        glfwSetKeyCallback(mHandle, KeyCallback);
        glfwSetMouseButtonCallback(mHandle, MouseButtonCallback);
        glfwSetCursorPosCallback(mHandle, MousePositionCallback);
        glfwSetScrollCallback(mHandle, MouseScrollCallback);
        glfwSetCharCallback(mHandle, CharCallback);

        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);

        switch (mVisibility)
        {
            case WindowVisibility::Fullscreen:
                glfwSetWindowMonitor(mHandle, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
                break;
            case WindowVisibility::Iconified:
                glfwIconifyWindow(mHandle);
                break;
            default:
                std::int32_t monitorX = 0;
                std::int32_t monitorY = 0;
                glfwGetMonitorPos(monitor, &monitorX, &monitorY);
                std::int32_t xpos = monitorX + (mode->width - static_cast<int>(mSize.x)) / 2;
                std::int32_t ypos = monitorY + (mode->height - static_cast<int>(mSize.y)) / 2;
                glfwSetWindowMonitor(mHandle, nullptr, xpos, ypos, mSize.x, mSize.y, 0);
                break;
        }

        if (mShown)
        {
            glfwShowWindow(mHandle);
        }
        else
        {
            glfwHideWindow(mHandle);
        }
    }

    Window::~Window()
    {
        glfwDestroyWindow(mHandle);

        mHandle = nullptr;
    }

    std::queue<WindowEvent>& Window::QueryEvents()
    {
        return mEventQueue;
    }

    void Window::Update()
    {
        if (mSizeDirty)
        {
            glfwSetWindowSize(mHandle, mSize[0], mSize[1]);

            mSizeDirty = false;
        }

        if (mTitleDirty)
        {
            glfwSetWindowTitle(mHandle, mTitle.c_str());

            mTitleDirty = false;
        }

        if (mVisibilityDirty)
        {
            GLFWmonitor* monitor = glfwGetPrimaryMonitor();
            const GLFWvidmode* mode = glfwGetVideoMode(monitor);

            switch (mVisibility)
            {
                case WindowVisibility::Fullscreen:
                    glfwSetWindowMonitor(mHandle, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
                    break;
                case WindowVisibility::Iconified:
                    glfwIconifyWindow(mHandle);
                    break;
                default:
                    std::int32_t monitorX = 0;
                    std::int32_t monitorY = 0;
                    glfwGetMonitorPos(monitor, &monitorX, &monitorY);
                    std::int32_t xpos = monitorX + (mode->width - static_cast<int>(mSize.x)) / 2;
                    std::int32_t ypos = monitorY + (mode->height - static_cast<int>(mSize.y)) / 2;
                    glfwSetWindowMonitor(mHandle, nullptr, xpos, ypos, mSize.x, mSize.y, 0);
                    break;
            }

            mVisibilityDirty = false;
        }

        if (mShownDirty)
        {
            if (mShown)
            {
                glfwShowWindow(mHandle);
            }
            else
            {
                glfwHideWindow(mHandle);
            }

            mShownDirty = false;
        }
    }

    template <>
    const WindowSize& Window::Get<WindowAttribute::Size>() const
    {
        return mSize;
    }

    template <>
    const WindowTitle& Window::Get<WindowAttribute::Title>() const
    {
        return mTitle;
    }

    template <>
    const WindowVisibility& Window::Get<WindowAttribute::Visibility>() const
    {
        return mVisibility;
    }

    template <>
    const bool& Window::Get<WindowAttribute::Shown>() const
    {
        return mShown;
    }

    template <>
    void Window::Set<WindowAttribute::Size>(const WindowSize& value)
    {
        mSize = value;
        mSizeDirty = true;
    }

    template <>
    void Window::Set<WindowAttribute::Title>(const WindowTitle& value)
    {
        mTitle = value;
        mTitleDirty = true;
    }

    template <>
    void Window::Set<WindowAttribute::Visibility>(const WindowVisibility& value)
    {
        mVisibility = value;
        mVisibilityDirty = true;
    }

    template <>
    void Window::Set<WindowAttribute::Shown>(const bool& value)
    {
        mShown = value;
        mShownDirty = true;
    }

    const char** WindowInteractionLayer<WindowInteractive::Vulkan>::GetRequiredInstanceExtensions(std::uint32_t* count)
    {
        if (count == nullptr)
        {
            throw Debug::Exception(Debug::ErrorCode::INVALID_ARGUMENT,
                                   "std::vector<std::string_view> Systems::WindowInteractionLayer<WindowInteractive::Vulkan>::GetRequiredInstanceExtensions():\n"
                                   "general error\n"
                                   "'count' parameter must not be nullptr");
        }

        const char** extensions = glfwGetRequiredInstanceExtensions(count);

        if (extensions == nullptr || *count == 0)
        {
            throw Debug::Exception(Debug::ErrorCode::GENERAL_ERROR,
                                   "std::vector<std::string_view> Systems::WindowInteractionLayer<WindowInteractive::Vulkan>::GetRequiredInstanceExtensions():\n"
                                   "general error\n"
                                   "no Vulkan extensions requested by window\n"
                                   "no extensions may be available");
        }

        return extensions;
    }

    VkSurfaceKHR WindowInteractionLayer<WindowInteractive::Vulkan>::CreateWindowSurface(VkInstance instance, const VkAllocationCallbacks* allocator)
    {
        VkSurfaceKHR surface;

        VkResult result = glfwCreateWindowSurface(instance, mHandle, allocator, &surface);

        if (result != VK_SUCCESS)
        {
            throw Debug::Exception(Debug::ErrorCode::GENERAL_ERROR,
                                   "VkSurfaceKHR Systems::WindowInteractionLayer<WindowInteractive::Vulkan>::CreateWindowSurface(VkInstance, const VkAllocationCallbacks*):\n"
                                   "general error\n"
                                   "failed to create Vulkan window surface");
        }

        return surface;
    }

    WindowInteractionLayer<WindowInteractive::CocoaBackend>::WindowInteractionLayer(GLFWwindow* handle)
        : mHandle(handle)
    {
    }

    void* WindowInteractionLayer<WindowInteractive::CocoaBackend>::GetNSWindow()
    {
#if defined(PLATFORM_APPLE)
        return glfwGetCocoaWindow(mHandle);
#endif

        throw Debug::Exception(Debug::ErrorCode::PERMISSION_DENIED,
                               "void* Systems::WindowInteractionLayer<WindowInteractive::CocoaBackend>::GetNSWindow():\n"
                               "permission denied\n"
                               "current platform does not support WindowInteractionLayer<CocoaBackend>\n"
                               " WindowInteractionLayer<CocoaBackend> is available for:\n"
                               "\t- macOS");
    }

    WindowInteractionLayer<WindowInteractive::Win32Backend>::WindowInteractionLayer(GLFWwindow* handle)
        : mHandle(handle)
    {
    }

    void* WindowInteractionLayer<WindowInteractive::Win32Backend>::GetHWND()
    {
#if defined(PLATFORM_WINDOWS)
        return glfwGetWin32Window(mHandle);
#endif

        throw Debug::Exception(Debug::ErrorCode::PERMISSION_DENIED,
                               "void* Systems::WindowInteractionLayer<WindowInteractive::Win32Backend>::GetWin32Window():\n"
                               "permission denied\n"
                               "current platform does not support WindowInteractionLayer<Win32Backend>\n"
                               " WindowInteractionLayer<Win32Backend> is available for:\n"
                               "\t- Windows");
    }

    void Window::SizeCallback(GLFWwindow* window, int width, int height)
    {
        auto* self = static_cast<Window*>(glfwGetWindowUserPointer(window));

        if (!self)
        {
            return;
        }

        WindowEvent event;

        event.Type = WindowEventType::WindowResize;

        event.ResizeInteraction.Size = {width, height};

        self->mSize = event.ResizeInteraction.Size;
        self->mEventQueue.push(event);
    }

    void Window::CloseCallback(GLFWwindow* window)
    {
        auto* self = static_cast<Window*>(glfwGetWindowUserPointer(window));

        if (!self)
        {
            return;
        }

        WindowEvent event;

        event.Type = WindowEventType::WindowClose;

        self->mEventQueue.push(event);
    }

    void Window::FocusCallback(GLFWwindow* window, int focused)
    {
        auto* self = static_cast<Window*>(glfwGetWindowUserPointer(window));

        if (!self)
        {
            return;
        }

        WindowEvent event;

        event.Type = WindowEventType::WindowFocus;
        event.FocusInteraction.Focused = focused;

        self->mEventQueue.push(event);
    }

    void Window::IconifyCallback(GLFWwindow* window, int iconified)
    {
        auto* self = static_cast<Window*>(glfwGetWindowUserPointer(window));

        if (!self)
        {
            return;
        }

        WindowEvent event;

        event.Type = WindowEventType::WindowVisibility;
        event.VisibilityInteraction.Visibility = iconified ? WindowVisibility::Iconified : WindowVisibility::Windowed;

        self->mEventQueue.push(event);
        self->mVisibility = iconified ? WindowVisibility::Iconified : WindowVisibility::Windowed;
    }

    void Window::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int)
    {
        auto* self = static_cast<Window*>(glfwGetWindowUserPointer(window));

        if (!self || (action != GLFW_PRESS && action != GLFW_RELEASE))
        {
            return;
        }

        WindowEvent keyEvent;
        WindowEvent scancodeEvent;

        keyEvent.Type = WindowEventType::KeyInput;
        keyEvent.KeyInput.Input = MapKey(key);
        scancodeEvent.ScancodeInput.State = action == GLFW_PRESS ? PressableStatus::Pressed : PressableStatus::Released;

        scancodeEvent.Type = WindowEventType::ScancodeInput;
        scancodeEvent.ScancodeInput.Scancode = scancode;
        scancodeEvent.ScancodeInput.State = action == GLFW_PRESS ? PressableStatus::Pressed : PressableStatus::Released;

        self->mEventQueue.push(keyEvent);
        self->mEventQueue.push(scancodeEvent);
    }

    void Window::MouseButtonCallback(GLFWwindow* window, int button, int action, int)
    {
        auto* self = static_cast<Window*>(glfwGetWindowUserPointer(window));

        if (!self)
        {
            return;
        }

        if (action != GLFW_PRESS && action != GLFW_RELEASE)
        {
            return;
        }

        WindowEvent event;

        event.Type = WindowEventType::MouseButton;
        event.MouseButtonInput.Button = MapButton(button);
        event.MouseButtonInput.State = action == GLFW_PRESS ? PressableStatus::Pressed : PressableStatus::Released;

        self->mEventQueue.push(event);
    }

    void Window::MousePositionCallback(GLFWwindow* window, double x, double y)
    {
        auto* self = static_cast<Window*>(glfwGetWindowUserPointer(window));

        if (!self)
        {
            return;
        }

        WindowEvent event;

        event.Type = WindowEventType::MouseMove;
        event.MousePositionInput.Position = {x, y};

        self->mEventQueue.push(event);
    }

    void Window::MouseScrollCallback(GLFWwindow* window, double x, double y)
    {
        auto* self = static_cast<Window*>(glfwGetWindowUserPointer(window));

        if (!self)
        {
            return;
        }

        WindowEvent event;

        event.Type = WindowEventType::MouseScroll;
        event.MouseScrollInput.Offsets = {x, y};

        self->mEventQueue.push(event);
    }

    void Window::CharCallback(GLFWwindow* window, std::uint32_t codepoint)
    {
        auto* self = static_cast<Window*>(glfwGetWindowUserPointer(window));

        if (!self)
        {
            return;
        }

        WindowEvent event;

        event.Type = WindowEventType::CharInput;
        event.CharInput.Codepoint = codepoint;

        self->mEventQueue.push(event);
    }
}
