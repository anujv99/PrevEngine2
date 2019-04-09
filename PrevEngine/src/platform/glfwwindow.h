#pragma once

#ifdef PV_WINDOWING_API_GLFW

#include "engine/window.h"

#include <GLFW/glfw3.h>

namespace prev {

	class GlfwWindow : public Window {
	public:
		GlfwWindow(const WindowDesc & windowDesc);
		~GlfwWindow();

		virtual void Update() override;
		virtual void SetEventCallbackFunc(std::function<void(Event & e) > func) override;
		virtual void * GetRawPointer() override;
	public:
		bool m_Status;
	private:
		void DefaultEventCallbackFunction(Event & e) { }
	private:
		struct WindowData {
			// Useful Info
			unsigned int Width;
			unsigned int Height;
			std::string Title;

			// Event Callback Function
			std::function<void(Event & e)> CallbackFunction;

			// GLFW Stuff
			GLFWwindow * Window = nullptr;
		};

		WindowData m_Data;
	};

}

#endif