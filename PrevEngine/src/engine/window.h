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
		friend class Application;
	public:
		virtual ~Window() { };
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
	private:
		static Window * Create(const WindowDesc & windowDesc, const WindowAPI & windowingAPI);
	};

}
