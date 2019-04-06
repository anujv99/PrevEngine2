#include "pch.h"
#include "application.h"

#include "engine/window.h"

namespace prev {

	Window * s_Window = nullptr;

	Application::Application() {
		s_Window = Window::Create();
		if (s_Window == nullptr) {
			IsAppReady = false;
			return;
		}
		s_Window->SetEventCallbackFunc(BIND_EVENT_FN(Application::EventCallbackFunc));
		Timer::FPSCounter(true);
	}

	Application::~Application() {
		delete s_Window;
	}

	void Application::Run() {
		while (IsAppRunning) {
			Timer::Update();
			s_Window->Update();

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
