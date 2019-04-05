#include "pch.h"
#include "application.h"

namespace prev {

	Application::Application() {
		m_Window = Window::Create();
	}

	Application::~Application() {
		delete m_Window;
	}

	void Application::Run() {
		while (m_Window->Update()) {

		}
	}

}
