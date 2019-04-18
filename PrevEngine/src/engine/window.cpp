#include "pch.h"
#include "window.h"

namespace prev {

#if defined(PV_WINDOWING_API_WIN32) || defined(PV_WINDOWING_API_GLFW)
	Window * Window::Create(const WindowDesc & windowDesc, const WindowAPI & windowingAPI) {
	#if defined(PV_WINDOWING_API_WIN32)
		if (windowingAPI != WindowAPI::WINDOWING_API_WIN32) {
			PV_POST_ERROR("Cannot use GLFW when PV_WINDOWING_API_GLFW is not defined!\nUsing Win32 instead");
		}
		return CreateWin32Window(windowDesc);
	#else
		if (windowingAPI != WindowAPI::WINDOWING_API_GLFW) {
			PV_POST_ERROR("Cannot use Win32 when PV_WINDOWING_API_WIN32 is not defined!\nUsing GLFW instead");
		}
		return CreateGLFWWindow(windowDesc);
	#endif
	}
#elif defined(PV_WINDOWING_API_BOTH)
	Window * Window::Create(const WindowDesc & windowDesc, const WindowAPI & windowingApi) {
		if (windowingApi == WindowAPI::WINDOWING_API_WIN32) {
			return CreateWin32Window(windowDesc);
		} else if (windowingApi == WindowAPI::WINDOWING_API_GLFW) {
			return CreateGLFWWindow(windowDesc);
		} else {
			PV_POST_FATAL("Please Pass a valid windowing api\n"
						  "For Win32 use WINDOWING_API_WIN32\n"
						  "For GLFW use WINDOWING_API_GLFW\n");
			return nullptr;
		}
	}
#else
	static Window * Window::Create(const WindowDesc & windowDesc, const WindowAPI & windowingApi) {
		PV_POST_FATAL("Please Define the windowing api symbols\n"
					  "For Win32 use PV_WINDOWING_API_WIN32\n"
					  "For GLFW use PV_WINDOWING_API_GLFW\n"
					  "To Compile both use PV_WINDOWING_API_BOTH");
		return nullptr;
	}
#endif

}