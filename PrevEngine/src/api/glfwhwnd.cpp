#include "pch.h"
#include "glfwhwnd.h"

#if defined(PV_WINDOWING_API_GLFW) || defined(PV_WINDOWING_API_BOTH)

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