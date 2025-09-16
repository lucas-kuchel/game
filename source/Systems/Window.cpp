#include <Systems/Window.hpp>

#include <Debug/Exception.hpp>

#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>

#if defined(PLATFORM_APPLE)
#define GLFW_EXPOSE_NATIVE_COCOA

#include <GLFW/glfw3native.h>
#endif

#include <cstring>

namespace Systems
{
    struct InputState
    {
        std::array<PressableState, 91> Keys = {PressableState::None};
        std::array<PressableState, 5> Buttons = {PressableState::Released};

        std::array<bool, 91> KeysDown = {false};
        std::array<bool, 5> ButtonsDown = {false};

        CursorState Cursor;
        ScrollState Scroll;
    };

    Key MapKey(int glfwKey)
    {
        switch (glfwKey)
        {
            case GLFW_KEY_A:
            {
                return Key::A;
            }
            case GLFW_KEY_B:
            {
                return Key::B;
            }
            case GLFW_KEY_C:
            {
                return Key::C;
            }
            case GLFW_KEY_D:
            {
                return Key::D;
            }
            case GLFW_KEY_E:
            {
                return Key::E;
            }
            case GLFW_KEY_F:
            {
                return Key::F;
            }
            case GLFW_KEY_G:
            {
                return Key::G;
            }
            case GLFW_KEY_H:
            {
                return Key::H;
            }
            case GLFW_KEY_I:
            {
                return Key::I;
            }
            case GLFW_KEY_J:
            {
                return Key::J;
            }
            case GLFW_KEY_K:
            {
                return Key::K;
            }
            case GLFW_KEY_L:
            {
                return Key::L;
            }
            case GLFW_KEY_M:
            {
                return Key::M;
            }
            case GLFW_KEY_N:
            {
                return Key::N;
            }
            case GLFW_KEY_O:
            {
                return Key::O;
            }
            case GLFW_KEY_P:
            {
                return Key::P;
            }
            case GLFW_KEY_Q:
            {
                return Key::Q;
            }
            case GLFW_KEY_R:
            {
                return Key::R;
            }
            case GLFW_KEY_S:
            {
                return Key::S;
            }
            case GLFW_KEY_T:
            {
                return Key::T;
            }
            case GLFW_KEY_U:
            {
                return Key::U;
            }
            case GLFW_KEY_V:
            {
                return Key::V;
            }
            case GLFW_KEY_W:
            {
                return Key::W;
            }
            case GLFW_KEY_X:
            {
                return Key::X;
            }
            case GLFW_KEY_Y:
            {
                return Key::Y;
            }
            case GLFW_KEY_Z:
            {
                return Key::Z;
            }
            case GLFW_KEY_F1:
            {
                return Key::F1;
            }
            case GLFW_KEY_F2:
            {
                return Key::F2;
            }
            case GLFW_KEY_F3:
            {
                return Key::F3;
            }
            case GLFW_KEY_F4:
            {
                return Key::F4;
            }
            case GLFW_KEY_F5:
            {
                return Key::F5;
            }
            case GLFW_KEY_F6:
            {
                return Key::F6;
            }
            case GLFW_KEY_F7:
            {
                return Key::F7;
            }
            case GLFW_KEY_F8:
            {
                return Key::F8;
            }
            case GLFW_KEY_F9:
            {
                return Key::F9;
            }
            case GLFW_KEY_F10:
            {
                return Key::F10;
            }
            case GLFW_KEY_F11:
            {
                return Key::F11;
            }
            case GLFW_KEY_F12:
            {
                return Key::F12;
            }
            case GLFW_KEY_SPACE:
            {
                return Key::Space;
            }
            case GLFW_KEY_LEFT_CONTROL:
            {
                return Key::LeftControl;
            }
            case GLFW_KEY_RIGHT_CONTROL:
            {
                return Key::RightControl;
            }
            case GLFW_KEY_LEFT_SUPER:
            {
                return Key::LeftSuper;
            }
            case GLFW_KEY_RIGHT_SUPER:
            {
                return Key::RightSuper;
            }
            case GLFW_KEY_LEFT_ALT:
            {
                return Key::LeftAlt;
            }
            case GLFW_KEY_RIGHT_ALT:
            {
                return Key::RightAlt;
            }
            case GLFW_KEY_CAPS_LOCK:
            {
                return Key::Capslock;
            }
            case GLFW_KEY_TAB:
            {
                return Key::Tab;
            }
            case GLFW_KEY_ESCAPE:
            {
                return Key::Escape;
            }
            case GLFW_KEY_LEFT_SHIFT:
            {
                return Key::LeftShift;
            }
            case GLFW_KEY_RIGHT_SHIFT:
            {
                return Key::RightShift;
            }
            case GLFW_KEY_HOME:
            {
                return Key::Home;
            }
            case GLFW_KEY_END:
            {
                return Key::End;
            }
            case GLFW_KEY_PAGE_UP:
            {
                return Key::PageUp;
            }
            case GLFW_KEY_PAGE_DOWN:
            {
                return Key::PageDown;
            }
            case GLFW_KEY_INSERT:
            {
                return Key::Insert;
            }
            case GLFW_KEY_DELETE:
            {
                return Key::Delete;
            }
            case GLFW_KEY_ENTER:
            {
                return Key::Enter;
            }
            case GLFW_KEY_BACKSPACE:
            {
                return Key::Backspace;
            }
            case GLFW_KEY_GRAVE_ACCENT:
            {
                return Key::Tilde;
            }
            case GLFW_KEY_EQUAL:
            {
                return Key::Equal;
            }
            case GLFW_KEY_MINUS:
            {
                return Key::Minus;
            }
            case GLFW_KEY_SLASH:
            {
                return Key::Forwardslash;
            }
            case GLFW_KEY_BACKSLASH:
            {
                return Key::Backslash;
            }
            case GLFW_KEY_SEMICOLON:
            {
                return Key::Semicolon;
            }
            case GLFW_KEY_APOSTROPHE:
            {
                return Key::Apostrophe;
            }
            case GLFW_KEY_COMMA:
            {
                return Key::Comma;
            }
            case GLFW_KEY_PERIOD:
            {
                return Key::Period;
            }
            case GLFW_KEY_0:
            {
                return Key::Row0;
            }
            case GLFW_KEY_1:
            {
                return Key::Row1;
            }
            case GLFW_KEY_2:
            {
                return Key::Row2;
            }
            case GLFW_KEY_3:
            {
                return Key::Row3;
            }
            case GLFW_KEY_4:
            {
                return Key::Row4;
            }
            case GLFW_KEY_5:
            {
                return Key::Row5;
            }
            case GLFW_KEY_6:
            {
                return Key::Row6;
            }
            case GLFW_KEY_7:
            {
                return Key::Row7;
            }
            case GLFW_KEY_8:
            {
                return Key::Row8;
            }
            case GLFW_KEY_9:
            {
                return Key::Row9;
            }
            case GLFW_KEY_KP_ADD:
            {
                return Key::NumpadPlus;
            }
            case GLFW_KEY_KP_SUBTRACT:
            {
                return Key::NumpadMinus;
            }
            case GLFW_KEY_KP_0:
            {
                return Key::Numpad0;
            }
            case GLFW_KEY_KP_1:
            {
                return Key::Numpad1;
            }
            case GLFW_KEY_KP_2:
            {
                return Key::Numpad2;
            }
            case GLFW_KEY_KP_3:
            {
                return Key::Numpad3;
            }
            case GLFW_KEY_KP_4:
            {
                return Key::Numpad4;
            }
            case GLFW_KEY_KP_5:
            {
                return Key::Numpad5;
            }
            case GLFW_KEY_KP_6:
            {
                return Key::Numpad6;
            }
            case GLFW_KEY_KP_7:
            {
                return Key::Numpad7;
            }
            case GLFW_KEY_KP_8:
            {
                return Key::Numpad8;
            }
            case GLFW_KEY_KP_9:
            {
                return Key::Numpad9;
            }
            case GLFW_KEY_UP:
            {
                return Key::UpArrow;
            }
            case GLFW_KEY_DOWN:
            {
                return Key::DownArrow;
            }
            case GLFW_KEY_LEFT:
            {
                return Key::LeftArrow;
            }
            case GLFW_KEY_RIGHT:
            {
                return Key::RightArrow;
            }
        }

        throw;
    }

    MouseButton MapButton(int glfwButton)
    {
        switch (glfwButton)
        {
            case GLFW_MOUSE_BUTTON_LEFT:
            {
                return MouseButton::Left;
            }
            case GLFW_MOUSE_BUTTON_RIGHT:
            {
                return MouseButton::Right;
            }
            case GLFW_MOUSE_BUTTON_MIDDLE:
            {
                return MouseButton::Middle;
            }
            case GLFW_MOUSE_BUTTON_4:
            {
                return MouseButton::Forward;
            }
            case GLFW_MOUSE_BUTTON_5:
            {
                return MouseButton::Back;
            }
        }

        throw;
    }

    void KeyCallback(GLFWwindow* window, int key, int, int action, int)
    {
        auto* state = static_cast<InputState*>(glfwGetWindowUserPointer(window));

        if (!state)
        {
            return;
        }

        size_t idx = static_cast<size_t>(MapKey(key));

        if (action == GLFW_PRESS)
        {
            state->KeysDown[idx] = true;
        }
        else if (action == GLFW_RELEASE)
        {
            state->KeysDown[idx] = false;
        }
    }

    void MouseCallback(GLFWwindow* window, int button, int action, int)
    {
        auto* state = static_cast<InputState*>(glfwGetWindowUserPointer(window));

        if (!state)
        {
            return;
        }

        size_t idx = static_cast<size_t>(MapButton(button));

        if (action == GLFW_PRESS)
        {
            state->ButtonsDown[idx] = true;
        }
        else if (action == GLFW_RELEASE)
        {
            state->ButtonsDown[idx] = false;
        }
    }

    void CursorCallback(GLFWwindow* window, double xpos, double ypos)
    {
        auto* state = static_cast<InputState*>(glfwGetWindowUserPointer(window));

        if (!state)
        {
            return;
        }

        state->Cursor.Position[0] = xpos;
        state->Cursor.Position[1] = ypos;
    }

    void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
    {
        auto* state = static_cast<InputState*>(glfwGetWindowUserPointer(window));

        if (!state)
        {
            return;
        }

        state->Scroll.Offsets[0] += xoffset;
        state->Scroll.Offsets[1] += yoffset;
    }

    WindowInteractionLayer<WindowInteractive::OpenGL>::WindowInteractionLayer(void* handle)
        : mHandle(handle)
    {
    }

    WindowInteractionLayer<WindowInteractive::Vulkan>::WindowInteractionLayer(void* handle)
        : mHandle(handle)
    {
    }

    Window::Window(const WindowDescriptor& descriptor)
        : mContext(descriptor.Context), mTitle(descriptor.Title), mSize(descriptor.Size)
    {
        const RendererBackend& rendererBackend = descriptor.Context.get().template Get<ContextAttribute::Renderer>();

        switch (rendererBackend)
        {
            case RendererBackend::OpenGL:
            {
                GL_Init();

                break;
            }
            case RendererBackend::Vulkan:
            {
                VK_Init();

                break;
            }
            case RendererBackend::Metal:
            {
                MTL_Init();

                break;
            }
        }

        mHandle = glfwCreateWindow(mSize[0], mSize[1], mTitle.c_str(), nullptr, nullptr);

        if (mHandle == nullptr)
        {
            throw Debug::Exception(Debug::ErrorCode::GENERAL_ERROR, "Systems::Window::Window(const WindowDescriptor&):\n"
                                                                    "general error\n"
                                                                    "window creation failed\n"
                                                                    "check window arguments");
        }

        InputState* inputState = new InputState();

        glfwSetWindowUserPointer(static_cast<GLFWwindow*>(mHandle), inputState);

        glfwSetKeyCallback(static_cast<GLFWwindow*>(mHandle), KeyCallback);
        glfwSetMouseButtonCallback(static_cast<GLFWwindow*>(mHandle), MouseCallback);
        glfwSetCursorPosCallback(static_cast<GLFWwindow*>(mHandle), CursorCallback);
        glfwSetScrollCallback(static_cast<GLFWwindow*>(mHandle), ScrollCallback);

        mStatus = WindowStatus::Active;
    }

    Window::~Window()
    {
        InputState* inputState = static_cast<InputState*>(glfwGetWindowUserPointer(static_cast<GLFWwindow*>(mHandle)));

        delete inputState;

        glfwDestroyWindow(static_cast<GLFWwindow*>(mHandle));

        mHandle = nullptr;
    }

    void Window::ManageEvents()
    {
        if (mSizeDirty)
        {
            glfwSetWindowSize(static_cast<GLFWwindow*>(mHandle), mSize[0], mSize[1]);

            mSizeDirty = false;
        }
        else
        {
            glm::ivec2 size;

            glfwGetWindowSize(static_cast<GLFWwindow*>(mHandle), &size[0], &size[1]);

            mSize = size;
        }

        if (mTitleDirty)
        {
            glfwSetWindowTitle(static_cast<GLFWwindow*>(mHandle), mTitle.c_str());
        }

        if (mStatusDirty)
        {
            bool should = false;

            if (mStatus == WindowStatus::Inactive)
            {
                should = true;
            }

            glfwSetWindowShouldClose(static_cast<GLFWwindow*>(mHandle), should);
        }
        else
        {
            bool should = glfwWindowShouldClose(static_cast<GLFWwindow*>(mHandle));

            if (should)
            {
                mStatus = WindowStatus::Inactive;
            }
            else
            {
                mStatus = WindowStatus::Active;
            }
        }

        auto* state = static_cast<InputState*>(glfwGetWindowUserPointer(static_cast<GLFWwindow*>(mHandle)));

        for (size_t i = 0; i < state->Keys.size(); i++)
        {
            bool down = state->KeysDown[i];
            PressableState previous = state->Keys[i];

            if (down)
            {
                if (previous == PressableState::None || previous == PressableState::Released)
                {
                    state->Keys[i] = PressableState::Pressed;
                }
                else
                {
                    state->Keys[i] = PressableState::Held;
                }
            }
            else
            {
                if (previous == PressableState::Pressed || previous == PressableState::Held)
                {
                    state->Keys[i] = PressableState::Released;
                }
                else
                {
                    state->Keys[i] = PressableState::None;
                }
            }
        }

        for (size_t i = 0; i < state->Buttons.size(); i++)
        {
            bool down = state->ButtonsDown[i];
            PressableState previous = state->Buttons[i];

            if (down)
            {
                if (previous == PressableState::None || previous == PressableState::Released)
                {
                    state->Buttons[i] = PressableState::Pressed;
                }
                else
                {
                    state->Buttons[i] = PressableState::Held;
                }
            }
            else
            {
                if (previous == PressableState::Pressed || previous == PressableState::Held)
                {
                    state->Buttons[i] = PressableState::Released;
                }
                else
                {
                    state->Buttons[i] = PressableState::None;
                }
            }
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
    const WindowStatus& Window::Get<WindowAttribute::Status>() const
    {
        return mStatus;
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
    void Window::Set<WindowAttribute::Status>(const WindowStatus& value)
    {
        mStatus = value;
        mStatusDirty = true;
    }

    void Window::GL_Init()
    {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    }

    void Window::VK_Init()
    {
        if (!glfwVulkanSupported())
        {
            throw Debug::Exception(Debug::ErrorCode::GENERAL_ERROR,
                                   "Systems::Window::Window():\n"
                                   "general error\n"
                                   "Vulkan is unsupported on this system");
        }

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    }

    void Window::MTL_Init()
    {
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    }

    void WindowInteractionLayer<WindowInteractive::OpenGL>::MakeContextCurrent()
    {
        glfwMakeContextCurrent(static_cast<GLFWwindow*>(mHandle));
    }

    void WindowInteractionLayer<WindowInteractive::OpenGL>::SwapBuffers()
    {
        glfwSwapBuffers(static_cast<GLFWwindow*>(mHandle));
    }

    void WindowInteractionLayer<WindowInteractive::OpenGL>::SetSwapInterval(int interval)
    {
        glfwSwapInterval(interval);
    }

    std::vector<std::string_view> WindowInteractionLayer<WindowInteractive::Vulkan>::GetRequiredInstanceExtensions()
    {
        std::uint32_t count = 0;

        const char** extensions = glfwGetRequiredInstanceExtensions(&count);

        if (extensions == nullptr || count == 0)
        {
            throw Debug::Exception(Debug::ErrorCode::GENERAL_ERROR,
                                   "std::vector<std::string_view> Systems::WindowInteractionLayer<WindowInteractive::VULKAN>::GetRequiredInstanceExtensions():\n"
                                   "general error\n"
                                   "no Vulkan extensions requested by window\n"
                                   "no extensions may be available");
        }

        return {extensions, extensions + count};
    }

    VkSurfaceKHR WindowInteractionLayer<WindowInteractive::Vulkan>::CreateWindowSurface(VkInstance instance, const VkAllocationCallbacks* allocator)
    {
        VkSurfaceKHR surface;

        VkResult result = glfwCreateWindowSurface(instance, static_cast<GLFWwindow*>(mHandle), allocator, &surface);

        if (result != VK_SUCCESS)
        {
            throw Debug::Exception(Debug::ErrorCode::GENERAL_ERROR,
                                   "VkSurfaceKHR Systems::WindowInteractionLayer<WindowInteractive::VULKAN>::CreateWindowSurface(VkInstance, const VkAllocationCallbacks*):\n"
                                   "general error\n"
                                   "failed to create Vulkan window surface");
        }

        return surface;
    }

    WindowInteractionLayer<WindowInteractive::CocoaBackend>::WindowInteractionLayer(void* handle)
        : mHandle(handle)
    {
    }

    void* WindowInteractionLayer<WindowInteractive::CocoaBackend>::GetCocoaWindow()
    {
#if defined(PLATFORM_APPLE)
        return glfwGetCocoaWindow(static_cast<GLFWwindow*>(mHandle));
#endif

        throw Debug::Exception(Debug::ErrorCode::PERMISSION_DENIED,
                               "void* Systems::WindowInteractionLayer<WindowInteractive::COCOA_LAYER>::GetCocoaWindow():\n"
                               "permission denied\n"
                               "current platform does not support WindowInteractionLayer<COCOA_LAYER>\n"
                               " WindowInteractionLayer<COCOA_LAYER> is available for:\n"
                               "\t- macOS");
    }

    WindowInteractionLayer<WindowInteractive::KeyboardInputs>::WindowInteractionLayer(void* handle)
        : mHandle(handle)
    {
        mState = static_cast<InputState*>(glfwGetWindowUserPointer(static_cast<GLFWwindow*>(mHandle)));
        if (!mState)
        {
            throw Debug::Exception(Debug::ErrorCode::GENERAL_ERROR,
                                   "Systems::WindowInteractionLayer<WindowInteractive::KEYBOARD_LAYER>::WindowInteractionLayer(void*):\n"
                                   "general error\n"
                                   "no input state bound to window");
        }
    }

    PressableState WindowInteractionLayer<WindowInteractive::KeyboardInputs>::GetKeyState(Key key) const
    {
        return mState->Keys[static_cast<size_t>(key)];
    }

    WindowInteractionLayer<WindowInteractive::MouseInputs>::WindowInteractionLayer(void* handle)
        : mHandle(handle)
    {
        mState = static_cast<InputState*>(glfwGetWindowUserPointer(static_cast<GLFWwindow*>(mHandle)));
        if (!mState)
        {
            throw Debug::Exception(Debug::ErrorCode::GENERAL_ERROR,
                                   "Systems::WindowInteractionLayer<WindowInteractive::MOUSE_LAYER>::WindowInteractionLayer(void*):\n"
                                   "general error\n"
                                   "no input state bound to window");
        }
    }

    PressableState WindowInteractionLayer<WindowInteractive::MouseInputs>::GetButtonState(MouseButton button) const
    {
        return mState->Buttons[static_cast<size_t>(button)];
    }

    ScrollState WindowInteractionLayer<WindowInteractive::MouseInputs>::GetScrollState() const
    {
        return mState->Scroll;
    }

    CursorState WindowInteractionLayer<WindowInteractive::MouseInputs>::GetCursorState() const
    {
        return mState->Cursor;
    }

    void WindowInteractionLayer<WindowInteractive::MouseInputs>::SetCursorState(const CursorState& state)
    {
        mState->Cursor = state;

        if (state.Mode == CursorMode::Disabled)
        {
            glfwSetInputMode(static_cast<GLFWwindow*>(mHandle), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
        else if (state.Mode == CursorMode::Hidden)
        {
            glfwSetInputMode(static_cast<GLFWwindow*>(mHandle), GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
        }
        else if (state.Mode == CursorMode::Normal)
        {
            glfwSetInputMode(static_cast<GLFWwindow*>(mHandle), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }

        glfwSetCursorPos(static_cast<GLFWwindow*>(mHandle), state.Position[0], state.Position[1]);
    }
}