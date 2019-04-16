#include "pch.h"
#include "glfwwindow.h"

#include "engine/input/keyboardkeycodes.h"

#if defined(PV_WINDOWING_API_GLFW) || defined(PV_WINDOWING_API_BOTH)

namespace prev {

	GlfwWindow * s_GlobalInstance;

	Window * Window::CreateGLFWWindow(const WindowDesc & windowDesc) {
		GlfwWindow * window = new GlfwWindow(windowDesc);
		if (!window->m_Status) {
			PV_POST_FATAL("Unable to create window");
			delete window;
			return nullptr;
		}
		return (Window *)window;
	}

	GlfwWindow::GlfwWindow(const WindowDesc & windowDesc) {

		s_GlobalInstance = this;
		m_Data.Width = windowDesc.Width;
		m_Data.Height = windowDesc.Height;
		m_Data.Title = windowDesc.Title;
		m_Data.CallbackFunction = BIND_EVENT_FN(GlfwWindow::DefaultEventCallbackFunction);
		m_Status = false;

		if (glfwInit() != GLFW_TRUE) {
			PV_POST_FATAL("Unable to initialize GLFW");
			m_Status = false;
			return;
		}

		GLFWmonitor * monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode * mode = glfwGetVideoMode(monitor);

		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RED_BITS, mode->redBits);
		glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
		glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
		glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

		m_Data.Window = glfwCreateWindow(m_Data.Width, m_Data.Height, m_Data.Title.c_str(), nullptr, nullptr);

		if (!m_Data.Window) {
			PV_POST_FATAL("Unable to Create GLFW Window");
			m_Status = false;
			return;
		}

		{
			m_KeyMap[GLFW_KEY_SPACE] = PV_KEYBOARD_KEY_SPACE;
			m_KeyMap[GLFW_KEY_0] = PV_KEYBOARD_KEY_0;
			m_KeyMap[GLFW_KEY_1] = PV_KEYBOARD_KEY_1;
			m_KeyMap[GLFW_KEY_2] = PV_KEYBOARD_KEY_2;
			m_KeyMap[GLFW_KEY_3] = PV_KEYBOARD_KEY_3;
			m_KeyMap[GLFW_KEY_4] = PV_KEYBOARD_KEY_4;
			m_KeyMap[GLFW_KEY_5] = PV_KEYBOARD_KEY_5;
			m_KeyMap[GLFW_KEY_6] = PV_KEYBOARD_KEY_6;
			m_KeyMap[GLFW_KEY_7] = PV_KEYBOARD_KEY_7;
			m_KeyMap[GLFW_KEY_8] = PV_KEYBOARD_KEY_8;
			m_KeyMap[GLFW_KEY_9] = PV_KEYBOARD_KEY_9;
			m_KeyMap[GLFW_KEY_A] = PV_KEYBOARD_KEY_A;
			m_KeyMap[GLFW_KEY_B] = PV_KEYBOARD_KEY_B;
			m_KeyMap[GLFW_KEY_C] = PV_KEYBOARD_KEY_C;
			m_KeyMap[GLFW_KEY_D] = PV_KEYBOARD_KEY_D;
			m_KeyMap[GLFW_KEY_E] = PV_KEYBOARD_KEY_E;
			m_KeyMap[GLFW_KEY_F] = PV_KEYBOARD_KEY_F;
			m_KeyMap[GLFW_KEY_G] = PV_KEYBOARD_KEY_G;
			m_KeyMap[GLFW_KEY_H] = PV_KEYBOARD_KEY_H;
			m_KeyMap[GLFW_KEY_I] = PV_KEYBOARD_KEY_I;
			m_KeyMap[GLFW_KEY_J] = PV_KEYBOARD_KEY_J;
			m_KeyMap[GLFW_KEY_K] = PV_KEYBOARD_KEY_K;
			m_KeyMap[GLFW_KEY_L] = PV_KEYBOARD_KEY_L;
			m_KeyMap[GLFW_KEY_M] = PV_KEYBOARD_KEY_M;
			m_KeyMap[GLFW_KEY_N] = PV_KEYBOARD_KEY_N;
			m_KeyMap[GLFW_KEY_O] = PV_KEYBOARD_KEY_O;
			m_KeyMap[GLFW_KEY_P] = PV_KEYBOARD_KEY_P;
			m_KeyMap[GLFW_KEY_Q] = PV_KEYBOARD_KEY_Q;
			m_KeyMap[GLFW_KEY_R] = PV_KEYBOARD_KEY_R;
			m_KeyMap[GLFW_KEY_S] = PV_KEYBOARD_KEY_S;
			m_KeyMap[GLFW_KEY_T] = PV_KEYBOARD_KEY_T;
			m_KeyMap[GLFW_KEY_U] = PV_KEYBOARD_KEY_U;
			m_KeyMap[GLFW_KEY_V] = PV_KEYBOARD_KEY_V;
			m_KeyMap[GLFW_KEY_W] = PV_KEYBOARD_KEY_W;
			m_KeyMap[GLFW_KEY_X] = PV_KEYBOARD_KEY_X;
			m_KeyMap[GLFW_KEY_Y] = PV_KEYBOARD_KEY_Y;
			m_KeyMap[GLFW_KEY_Z] = PV_KEYBOARD_KEY_Z;
			m_KeyMap[GLFW_KEY_ESCAPE] = PV_KEYBOARD_KEY_ESCAPE;
			m_KeyMap[GLFW_KEY_ENTER] = PV_KEYBOARD_KEY_ENTER;
			m_KeyMap[GLFW_KEY_TAB] = PV_KEYBOARD_KEY_TAB;
			m_KeyMap[GLFW_KEY_BACKSPACE] = PV_KEYBOARD_KEY_BACKSPACE;
			m_KeyMap[GLFW_KEY_INSERT] = PV_KEYBOARD_KEY_INSERT;
			m_KeyMap[GLFW_KEY_DELETE] = PV_KEYBOARD_KEY_DELETE;
			m_KeyMap[GLFW_KEY_RIGHT] = PV_KEYBOARD_KEY_RIGHT;
			m_KeyMap[GLFW_KEY_LEFT] = PV_KEYBOARD_KEY_LEFT;
			m_KeyMap[GLFW_KEY_DOWN] = PV_KEYBOARD_KEY_DOWN;
			m_KeyMap[GLFW_KEY_UP] = PV_KEYBOARD_KEY_UP;
			m_KeyMap[GLFW_KEY_PAGE_UP] = PV_KEYBOARD_KEY_PAGEUP;
			m_KeyMap[GLFW_KEY_PAGE_DOWN] = PV_KEYBOARD_KEY_PAGEDOWN;
			m_KeyMap[GLFW_KEY_HOME] = PV_KEYBOARD_KEY_HOME;
			m_KeyMap[GLFW_KEY_END] = PV_KEYBOARD_KEY_END;
			m_KeyMap[GLFW_KEY_CAPS_LOCK] = PV_KEYBOARD_KEY_CAPITAL;
			m_KeyMap[GLFW_KEY_PRINT_SCREEN] = PV_KEYBOARD_KEY_PRINT;
			m_KeyMap[GLFW_KEY_PAUSE] = PV_KEYBOARD_KEY_PAUSE;
			m_KeyMap[GLFW_KEY_F1] = PV_KEYBOARD_KEY_F1;
			m_KeyMap[GLFW_KEY_F2] = PV_KEYBOARD_KEY_F2;
			m_KeyMap[GLFW_KEY_F3] = PV_KEYBOARD_KEY_F3;
			m_KeyMap[GLFW_KEY_F4] = PV_KEYBOARD_KEY_F4;
			m_KeyMap[GLFW_KEY_F5] = PV_KEYBOARD_KEY_F5;
			m_KeyMap[GLFW_KEY_F6] = PV_KEYBOARD_KEY_F6;
			m_KeyMap[GLFW_KEY_F7] = PV_KEYBOARD_KEY_F7;
			m_KeyMap[GLFW_KEY_F8] = PV_KEYBOARD_KEY_F8;
			m_KeyMap[GLFW_KEY_F9] = PV_KEYBOARD_KEY_F9;
			m_KeyMap[GLFW_KEY_F10] = PV_KEYBOARD_KEY_F10;
			m_KeyMap[GLFW_KEY_F11] = PV_KEYBOARD_KEY_F11;
			m_KeyMap[GLFW_KEY_F12] = PV_KEYBOARD_KEY_F12;
			m_KeyMap[GLFW_KEY_F13] = PV_KEYBOARD_KEY_F13;
			m_KeyMap[GLFW_KEY_F14] = PV_KEYBOARD_KEY_F14;
			m_KeyMap[GLFW_KEY_F15] = PV_KEYBOARD_KEY_F15;
			m_KeyMap[GLFW_KEY_F16] = PV_KEYBOARD_KEY_F16;
			m_KeyMap[GLFW_KEY_F17] = PV_KEYBOARD_KEY_F17;
			m_KeyMap[GLFW_KEY_F18] = PV_KEYBOARD_KEY_F18;
			m_KeyMap[GLFW_KEY_F19] = PV_KEYBOARD_KEY_F19;
			m_KeyMap[GLFW_KEY_F20] = PV_KEYBOARD_KEY_F20;
			m_KeyMap[GLFW_KEY_F21] = PV_KEYBOARD_KEY_F21;
			m_KeyMap[GLFW_KEY_F22] = PV_KEYBOARD_KEY_F22;
			m_KeyMap[GLFW_KEY_F23] = PV_KEYBOARD_KEY_F23;
			m_KeyMap[GLFW_KEY_F24] = PV_KEYBOARD_KEY_F24;
			m_KeyMap[GLFW_KEY_KP_0] = PV_KEYBOARD_KEY_NUMPAD_0;
			m_KeyMap[GLFW_KEY_KP_1] = PV_KEYBOARD_KEY_NUMPAD_1;
			m_KeyMap[GLFW_KEY_KP_2] = PV_KEYBOARD_KEY_NUMPAD_2;
			m_KeyMap[GLFW_KEY_KP_3] = PV_KEYBOARD_KEY_NUMPAD_3;
			m_KeyMap[GLFW_KEY_KP_4] = PV_KEYBOARD_KEY_NUMPAD_4;
			m_KeyMap[GLFW_KEY_KP_5] = PV_KEYBOARD_KEY_NUMPAD_5;
			m_KeyMap[GLFW_KEY_KP_6] = PV_KEYBOARD_KEY_NUMPAD_6;
			m_KeyMap[GLFW_KEY_KP_7] = PV_KEYBOARD_KEY_NUMPAD_7;
			m_KeyMap[GLFW_KEY_KP_8] = PV_KEYBOARD_KEY_NUMPAD_8;
			m_KeyMap[GLFW_KEY_KP_9] = PV_KEYBOARD_KEY_NUMPAD_9;
			m_KeyMap[GLFW_KEY_KP_DECIMAL] = PV_KEYBOARD_KEY_NUMPAD_DECIMAL;
			m_KeyMap[GLFW_KEY_KP_DIVIDE] = PV_KEYBOARD_KEY_NUMPAD_DIVIDE;
			m_KeyMap[GLFW_KEY_KP_MULTIPLY] = PV_KEYBOARD_KEY_NUMPAD_MULTIPLY;
			m_KeyMap[GLFW_KEY_KP_SUBTRACT] = PV_KEYBOARD_KEY_NUMPAD_SUBTRACT;
			m_KeyMap[GLFW_KEY_KP_ADD] = PV_KEYBOARD_KEY_NUMPAD_ADD;
			m_KeyMap[GLFW_KEY_KP_ENTER] = PV_KEYBOARD_KEY_ENTER;
			m_KeyMap[GLFW_KEY_MENU] = PV_KEYBOARD_KEY_MENU;
		}

		glfwSetWindowCloseCallback(m_Data.Window, [](GLFWwindow * window) -> void {
			WindowCloseEvent e;
			s_GlobalInstance->m_Data.CallbackFunction(e);
		});

		glfwSetWindowSizeCallback(m_Data.Window, [](GLFWwindow * window, int width, int height) -> void {
			WindowResizeEvent e(width, height);
			s_GlobalInstance->m_Data.CallbackFunction(e);
		});

		glfwSetWindowPosCallback(m_Data.Window, [](GLFWwindow * window, int x, int y) -> void {
			WindowMoveEvent e(x, y);
			s_GlobalInstance->m_Data.CallbackFunction(e);
		});

		glfwSetKeyCallback(m_Data.Window, [](GLFWwindow * window, int key, int scanCode, int action, int mods) ->void {
			if (action == GLFW_PRESS || action == GLFW_REPEAT) {
				KeyPressedEvent e(s_GlobalInstance->m_KeyMap[key], action == GLFW_REPEAT ? 1 : 0);
				s_GlobalInstance->m_Data.CallbackFunction(e);
				return;
			}

			KeyReleasedEvent e(s_GlobalInstance->m_KeyMap[key]);
			s_GlobalInstance->m_Data.CallbackFunction(e);
		});

		glfwSetScrollCallback(m_Data.Window, [](GLFWwindow * window, double xOffset, double yOffset) -> void {
			MouseScrolledEvent e((float)xOffset, (float)yOffset);
			s_GlobalInstance->m_Data.CallbackFunction(e);
		});

		glfwSetMouseButtonCallback(m_Data.Window, [](GLFWwindow * window, int button, int action, int mods) -> void {
			if (action == GLFW_PRESS) {
				MouseButtonPressedEvent e(button);
				s_GlobalInstance->m_Data.CallbackFunction(e);
				return;
			}

			MouseButtonReleasedEvent e(button);
			s_GlobalInstance->m_Data.CallbackFunction(e);
		});

		glfwSetCursorPosCallback(m_Data.Window, [](GLFWwindow * window, double x, double y) -> void {
			MouseMovedEvent e((float)x, (float)y);
			s_GlobalInstance->m_Data.CallbackFunction(e);
		});

		glfwSetCharCallback(m_Data.Window, [](GLFWwindow * window, unsigned int codepoint) -> void {
			CharacterEvent e((char)codepoint);
			s_GlobalInstance->m_Data.CallbackFunction(e);
		});

		m_Status = true;

		m_WindowAPI = WindowAPI::WINDOWING_API_GLFW;

		return;
	}

	GlfwWindow::~GlfwWindow() {
		glfwDestroyWindow(m_Data.Window);
		glfwTerminate();
		s_GlobalInstance = nullptr;
	}

	void GlfwWindow::Update() {
		glfwPollEvents();
		glfwSwapBuffers(m_Data.Window);
	}

	void GlfwWindow::SetEventCallbackFunc(std::function<void(Event & e)> func) {
		m_Data.CallbackFunction = func;
	}

	void * GlfwWindow::GetRawPointer() {
		return (void *)m_Data.Window;
	}

}

#endif