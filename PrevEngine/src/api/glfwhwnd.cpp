#include "pch.h"
#include "glfwhwnd.h"

#ifdef PV_WINDOWING_API_GLFW

#define GLFW_EXPOSE_NATIVE_WIN32

#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

namespace prev {
	HWND GetHWNDFromGLFW(void * glfwWindow) {
		HWND hWnd = glfwGetWin32Window((GLFWwindow *)glfwWindow);
		return hWnd;
	}
}


#else

namespace prev {
	HWND GetHWNDFromGLFW(void * glfwWindow) {
		return nullptr;
	}
}

#endif