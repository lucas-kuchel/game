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
        std::array<PressableState, 91> Keys = {PressableState::NONE};
        std::array<PressableState, 5> Buttons = {PressableState::RELEASED};

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
                return Key::SPACE;
            }
            case GLFW_KEY_LEFT_CONTROL:
            {
                return Key::LCTRL;
            }
            case GLFW_KEY_RIGHT_CONTROL:
            {
                return Key::RCTRL;
            }
            case GLFW_KEY_LEFT_SUPER:
            {
                return Key::LSUPER;
            }
            case GLFW_KEY_RIGHT_SUPER:
            {
                return Key::RSUPER;
            }
            case GLFW_KEY_LEFT_ALT:
            {
                return Key::LALT;
            }
            case GLFW_KEY_RIGHT_ALT:
            {
                return Key::RALT;
            }
            case GLFW_KEY_CAPS_LOCK:
            {
                return Key::CAPSLOCK;
            }
            case GLFW_KEY_TAB:
            {
                return Key::TAB;
            }
            case GLFW_KEY_ESCAPE:
            {
                return Key::ESC;
            }
            case GLFW_KEY_LEFT_SHIFT:
            {
                return Key::LSHIFT;
            }
            case GLFW_KEY_RIGHT_SHIFT:
            {
                return Key::RSHIFT;
            }
            case GLFW_KEY_HOME:
            {
                return Key::HOME;
            }
            case GLFW_KEY_END:
            {
                return Key::END;
            }
            case GLFW_KEY_PAGE_UP:
            {
                return Key::PGUP;
            }
            case GLFW_KEY_PAGE_DOWN:
            {
                return Key::PGDN;
            }
            case GLFW_KEY_INSERT:
            {
                return Key::INSERT;
            }
            case GLFW_KEY_DELETE:
            {
                return Key::DEL;
            }
            case GLFW_KEY_ENTER:
            {
                return Key::ENTER;
            }
            case GLFW_KEY_BACKSPACE:
            {
                return Key::BACKSPACE;
            }
            case GLFW_KEY_GRAVE_ACCENT:
            {
                return Key::TILDE;
            }
            case GLFW_KEY_EQUAL:
            {
                return Key::PLUS;
            }
            case GLFW_KEY_MINUS:
            {
                return Key::MINUS;
            }
            case GLFW_KEY_SLASH:
            {
                return Key::FSLASH;
            }
            case GLFW_KEY_BACKSLASH:
            {
                return Key::BSLASH;
            }
            case GLFW_KEY_SEMICOLON:
            {
                return Key::SEMICOLON;
            }
            case GLFW_KEY_APOSTROPHE:
            {
                return Key::QUOTE;
            }
            case GLFW_KEY_COMMA:
            {
                return Key::COMMA;
            }
            case GLFW_KEY_PERIOD:
            {
                return Key::PERIOD;
            }
            case GLFW_KEY_0:
            {
                return Key::ROW_0;
            }
            case GLFW_KEY_1:
            {
                return Key::ROW_1;
            }
            case GLFW_KEY_2:
            {
                return Key::ROW_2;
            }
            case GLFW_KEY_3:
            {
                return Key::ROW_3;
            }
            case GLFW_KEY_4:
            {
                return Key::ROW_4;
            }
            case GLFW_KEY_5:
            {
                return Key::ROW_5;
            }
            case GLFW_KEY_6:
            {
                return Key::ROW_6;
            }
            case GLFW_KEY_7:
            {
                return Key::ROW_7;
            }
            case GLFW_KEY_8:
            {
                return Key::ROW_8;
            }
            case GLFW_KEY_9:
            {
                return Key::ROW_9;
            }
            case GLFW_KEY_KP_0:
            {
                return Key::PAD_0;
            }
            case GLFW_KEY_KP_1:
            {
                return Key::PAD_1;
            }
            case GLFW_KEY_KP_2:
            {
                return Key::PAD_2;
            }
            case GLFW_KEY_KP_3:
            {
                return Key::PAD_3;
            }
            case GLFW_KEY_KP_4:
            {
                return Key::PAD_4;
            }
            case GLFW_KEY_KP_5:
            {
                return Key::PAD_5;
            }
            case GLFW_KEY_KP_6:
            {
                return Key::PAD_6;
            }
            case GLFW_KEY_KP_7:
            {
                return Key::PAD_7;
            }
            case GLFW_KEY_KP_8:
            {
                return Key::PAD_8;
            }
            case GLFW_KEY_KP_9:
            {
                return Key::PAD_9;
            }
            case GLFW_KEY_UP:
            {
                return Key::UP;
            }
            case GLFW_KEY_DOWN:
            {
                return Key::DOWN;
            }
            case GLFW_KEY_LEFT:
            {
                return Key::LEFT;
            }
            case GLFW_KEY_RIGHT:
            {
                return Key::RIGHT;
            }
        }

        throw Debug::Exception(Debug::ErrorCode::CONFLICT, "[UNREACHABLE]");
    }

    MouseButton MapButton(int glfwButton)
    {
        switch (glfwButton)
        {
            case GLFW_MOUSE_BUTTON_LEFT:
                return MouseButton::LEFT;
            case GLFW_MOUSE_BUTTON_RIGHT:
                return MouseButton::RIGHT;
            case GLFW_MOUSE_BUTTON_MIDDLE:
                return MouseButton::MIDDLE;
            case GLFW_MOUSE_BUTTON_4:
                return MouseButton::FORWARD;
            case GLFW_MOUSE_BUTTON_5:
                return MouseButton::BACK;
        }
        throw Debug::Exception(Debug::ErrorCode::CONFLICT, "[UNREACHABLE]");
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

    WindowInteractionLayer<WindowInteractive::OPENGL_LAYER>::WindowInteractionLayer(void* handle)
        : mHandle(handle)
    {
    }

    WindowInteractionLayer<WindowInteractive::VULKAN_LAYER>::WindowInteractionLayer(void* handle)
        : mHandle(handle)
    {
    }

    Window::Window(const WindowDescriptor& descriptor)
        : mContext(descriptor.Context), mTitle(descriptor.Title), mSize(descriptor.Size)
    {
        const RendererBackend& rendererBackend = descriptor.Context.get().template Get<ContextAttribute::RENDERER>();

        switch (rendererBackend)
        {
            case RendererBackend::OPENGL:
            {
                GL_Init();

                break;
            }
            case RendererBackend::VULKAN:
            {
                VK_Init();

                break;
            }
            case RendererBackend::METAL:
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

        mStatus = WindowStatus::ACTIVE;
    }

    Window::~Window()
    {
        InputState* inputState = static_cast<InputState*>(glfwGetWindowUserPointer(static_cast<GLFWwindow*>(mHandle)));

        delete inputState;

        glfwDestroyWindow(static_cast<GLFWwindow*>(mHandle));

        mHandle = nullptr;
    }

    void Window::Update()
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

            if (mStatus == WindowStatus::INACTIVE)
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
                mStatus = WindowStatus::INACTIVE;
            }
            else
            {
                mStatus = WindowStatus::ACTIVE;
            }
        }

        auto* state = static_cast<InputState*>(glfwGetWindowUserPointer(static_cast<GLFWwindow*>(mHandle)));

        for (size_t i = 0; i < state->Keys.size(); i++)
        {
            bool down = state->KeysDown[i];
            PressableState previous = state->Keys[i];

            if (down)
            {
                if (previous == PressableState::NONE || previous == PressableState::RELEASED)
                {
                    state->Keys[i] = PressableState::PRESSED;
                }
                else
                {
                    state->Keys[i] = PressableState::HELD;
                }
            }
            else
            {
                if (previous == PressableState::PRESSED || previous == PressableState::HELD)
                {
                    state->Keys[i] = PressableState::RELEASED;
                }
                else
                {
                    state->Keys[i] = PressableState::NONE;
                }
            }
        }

        for (size_t i = 0; i < state->Buttons.size(); i++)
        {
            bool down = state->ButtonsDown[i];
            PressableState previous = state->Buttons[i];

            if (down)
            {
                if (previous == PressableState::NONE || previous == PressableState::RELEASED)
                {
                    state->Buttons[i] = PressableState::PRESSED;
                }
                else
                {
                    state->Buttons[i] = PressableState::HELD;
                }
            }
            else
            {
                if (previous == PressableState::PRESSED || previous == PressableState::HELD)
                {
                    state->Buttons[i] = PressableState::RELEASED;
                }
                else
                {
                    state->Buttons[i] = PressableState::NONE;
                }
            }
        }
    }

    template <>
    const WindowSize& Window::Get<WindowAttribute::SIZE>() const
    {
        return mSize;
    }

    template <>
    const WindowTitle& Window::Get<WindowAttribute::TITLE>() const
    {
        return mTitle;
    }

    template <>
    const WindowStatus& Window::Get<WindowAttribute::STATUS>() const
    {
        return mStatus;
    }

    template <>
    void Window::Set<WindowAttribute::SIZE>(const WindowSize& value)
    {
        mSize = value;
        mSizeDirty = true;
    }

    template <>
    void Window::Set<WindowAttribute::TITLE>(const WindowTitle& value)
    {
        mTitle = value;
        mTitleDirty = true;
    }

    template <>
    void Window::Set<WindowAttribute::STATUS>(const WindowStatus& value)
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

    void WindowInteractionLayer<WindowInteractive::OPENGL_LAYER>::MakeContextCurrent()
    {
        glfwMakeContextCurrent(static_cast<GLFWwindow*>(mHandle));
    }

    void WindowInteractionLayer<WindowInteractive::OPENGL_LAYER>::SwapBuffers()
    {
        glfwSwapBuffers(static_cast<GLFWwindow*>(mHandle));
    }

    void WindowInteractionLayer<WindowInteractive::OPENGL_LAYER>::SetSwapInterval(int interval)
    {
        glfwSwapInterval(interval);
    }

    std::vector<std::string_view> WindowInteractionLayer<WindowInteractive::VULKAN_LAYER>::GetRequiredInstanceExtensions()
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

    VkSurfaceKHR WindowInteractionLayer<WindowInteractive::VULKAN_LAYER>::CreateWindowSurface(VkInstance instance, const VkAllocationCallbacks* allocator)
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

    WindowInteractionLayer<WindowInteractive::COCOA_LAYER>::WindowInteractionLayer(void* handle)
        : mHandle(handle)
    {
    }

    void* WindowInteractionLayer<WindowInteractive::COCOA_LAYER>::GetCocoaWindow()
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

    WindowInteractionLayer<WindowInteractive::KEYBOARD_LAYER>::WindowInteractionLayer(void* handle)
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

    PressableState WindowInteractionLayer<WindowInteractive::KEYBOARD_LAYER>::GetKeyState(Key key) const
    {
        return mState->Keys[static_cast<size_t>(key)];
    }

    WindowInteractionLayer<WindowInteractive::MOUSE_LAYER>::WindowInteractionLayer(void* handle)
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

    PressableState WindowInteractionLayer<WindowInteractive::MOUSE_LAYER>::GetButtonState(MouseButton button) const
    {
        return mState->Buttons[static_cast<size_t>(button)];
    }

    ScrollState WindowInteractionLayer<WindowInteractive::MOUSE_LAYER>::GetScrollState() const
    {
        return mState->Scroll;
    }

    CursorState WindowInteractionLayer<WindowInteractive::MOUSE_LAYER>::GetCursorState() const
    {
        return mState->Cursor;
    }

    void WindowInteractionLayer<WindowInteractive::MOUSE_LAYER>::SetCursorState(const CursorState& state)
    {
        mState->Cursor = state;

        if (state.Mode == CursorMode::DISABLED)
        {
            glfwSetInputMode(static_cast<GLFWwindow*>(mHandle), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
        else if (state.Mode == CursorMode::HIDDEN)
        {
            glfwSetInputMode(static_cast<GLFWwindow*>(mHandle), GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
        }
        else if (state.Mode == CursorMode::NORMAL)
        {
            glfwSetInputMode(static_cast<GLFWwindow*>(mHandle), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }

        glfwSetCursorPos(static_cast<GLFWwindow*>(mHandle), state.Position[0], state.Position[1]);
    }
}