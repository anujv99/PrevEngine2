#include "pch.h"
#include "application.h"

#include "engine/window.h"
#include "engine/graphicsapi.h"

namespace prev {

	Window * s_Window = nullptr;
	GraphicsApi * s_GraphicsAPI = nullptr;

	Application::Application() {

		WindowDesc winDesc;
		s_Window = Window::Create();
		if (s_Window == nullptr) {
			IsAppReady = false;
			return;
		}

		GraphicsDesc graphicsDesc(winDesc.Width, winDesc.Height);
		graphicsDesc.Vsync = false;
		s_GraphicsAPI = GraphicsApi::UseOpenGL(s_Window->GetRawPointer(), s_Window->m_WindowAPI, graphicsDesc);
		if (s_GraphicsAPI == nullptr) {
			IsAppReady = false;
			return;
		}

		s_Window->SetEventCallbackFunc(BIND_EVENT_FN(Application::EventCallbackFunc));
		Timer::FPSCounter(true);
	}

	Application::~Application() {
		if (s_Window != nullptr) {
			delete s_Window;
			s_Window = nullptr;
		}
		if (s_GraphicsAPI != nullptr) {
			delete s_GraphicsAPI;
			s_GraphicsAPI = nullptr;
		}
		return;
	}

	void Application::Run() {
		while (IsAppRunning) {
			Timer::Update();
			s_Window->Update();
			s_GraphicsAPI->StartFrame();

			s_GraphicsAPI->EndFrame();
		}
	}

	void Application::EventCallbackFunc(Event & e) {

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::WindowCloseFunc));

		if (e.GetCategoryFlags() & EventCategoryMouse) {
			return;
		}

		PV_DEBUG_LOG(e.ToString().c_str());
	}

	bool Application::WindowCloseFunc(WindowCloseEvent & e) {
		IsAppRunning = false;
		return true;
	}

}
