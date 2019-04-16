#include "pch.h"
#include "gethwnd.h"

#if defined(PV_WINDOWING_API_GLFW) || defined(PV_WINDOWING_API_BOTH)

#define GLFW_EXPOSE_NATIVE_WIN32

#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

namespace prev {
	HWND GetHWND(void * rawpointer) {
		HWND hWnd = glfwGetWin32Window((GLFWwindow *)rawpointer);
		return hWnd;
	}
}

#else

namespace prev {
	HWND GetHWND(void * rawpointer) {
		return (HWND)rawpointer;
	}
}

#endif