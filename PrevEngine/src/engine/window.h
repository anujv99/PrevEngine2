#pragma once

#include <string>
#include <functional>

#include "engine/events/event.h"

namespace prev {

	enum class WindowAPI {
		WINDOWING_API_WIN32,
		WINDOWING_API_GLFW,
		WINDOWING_API_BOTH
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
	public:
		WindowAPI m_WindowAPI;
	protected:
		Window() {
			#if defined(PV_WINDOWING_API_WIN32)
				m_WindowAPI = WindowAPI::WINDOWING_API_WIN32;
			#elif defined(PV_WINDOWING_API_GLFW)
				m_WindowAPI = WindowAPI::WINDOWING_API_GLFW;
			#elif defined(PV_WINDOWING_API_BOTH)
				m_WindowAPI = WindowAPI::WINDOWING_API_BOTH;
			#endif
		};
	public:
		static Window * Create(const WindowDesc & windowDesc = WindowDesc());
	};

}
