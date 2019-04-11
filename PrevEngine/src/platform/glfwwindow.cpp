#include "pch.h"
#include "glfwwindow.h"

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
				KeyPressedEvent e(key, action == GLFW_REPEAT ? 1 : 0);
				s_GlobalInstance->m_Data.CallbackFunction(e);
				return;
			}

			KeyReleasedEvent e(key);
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