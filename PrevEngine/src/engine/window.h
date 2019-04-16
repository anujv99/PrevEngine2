#pragma once

#include <string>
#include <functional>

#include "engine/events/event.h"
#include "essentials/error.h"

namespace prev {

	enum class WindowAPI {
		WINDOWING_API_WIN32,
		WINDOWING_API_GLFW,
		WINDOWING_API_UNINIT // Uninitialized
	};

	struct WindowDesc {
		WindowDesc(unsigned int width = 1280, unsigned int height = 720, std::string title = "PrevEngineButtPlug") :
			Width(width), Height(height), Title(title) {
		}
		unsigned int Width;
		unsigned int Height;
		std::string Title;
	};

	class Window {
	public:
		~Window() { };
		virtual void Update() = 0;
		virtual void SetEventCallbackFunc(std::function<void(Event & e)>) = 0;
		virtual void * GetRawPointer() = 0;
		virtual std::pair<int, int> GetWindowSize() = 0;
	public:
		WindowAPI m_WindowAPI = WindowAPI::WINDOWING_API_UNINIT;
	protected:
		Window() {};
		static Window * CreateWin32Window(const WindowDesc & windowDesc = WindowDesc());
		static Window * CreateGLFWWindow(const WindowDesc & windowDesc = WindowDesc());
	public:
#if defined(PV_WINDOWING_API_WIN32) || defined(PV_WINDOWING_API_GLFW)
		static Window * Create(const WindowDesc & windowDesc = WindowDesc()) {
			#if defined(PV_WINDOWING_API_WIN32)
				return CreateWin32Window(windowDesc);
			#else
				return CreateGLFWWindow(windowDesc);
			#endif
		}
#elif defined(PV_WINDOWING_API_BOTH)
		static Window * Create(const WindowDesc & windowDesc = WindowDesc(), const WindowAPI & windowingApi = WindowAPI::WINDOWING_API_WIN32) {
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
		static Window * Create(const WindowDesc & windowDesc = WindowDesc()) {
			PV_POST_FATAL("Please Define the windowing api symbols\n"
						  "For Win32 use PV_WINDOWING_API_WIN32\n"
						  "For GLFW use PV_WINDOWING_API_GLFW\n"
						  "To Compile both use PV_WINDOWING_API_BOTH");
			return nullptr;
		}
#endif
	};

}
