#include "pch.h"
#include "application.h"

#include "engine/window.h"
#include "engine/graphicsapi.h"
#include "engine/imgui/imguilayer.h"

namespace prev {

	Window * s_Window = nullptr;
	GraphicsAPI * s_GraphicsAPI = nullptr;

	Application::Application() {

		WindowDesc winDesc;
		s_Window = Window::Create(winDesc, WindowAPI::WINDOWING_API_WIN32);
		if (s_Window == nullptr) {
			IsAppReady = false;
			return;
		}

		GraphicsDesc graphicsDesc(winDesc.Width, winDesc.Height);
		graphicsDesc.Vsync = false;
		s_GraphicsAPI = GraphicsAPI::Create(s_Window->GetRawPointer(), s_Window->m_WindowAPI, graphicsDesc);
		if (s_GraphicsAPI == nullptr) {
			IsAppReady = false;
			return;
		}

		s_Window->SetEventCallbackFunc(BIND_EVENT_FN(Application::EventCallbackFunc));
		Timer::FPSCounter(true);

		m_LayerStack.PushOverlay(new ImGuiLayer(s_Window->m_WindowAPI, s_GraphicsAPI->m_RenderingAPI));
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
			
			m_LayerStack.OnUpdate();

			s_GraphicsAPI->EndFrame();
		}
	}

	void Application::EventCallbackFunc(Event & e) {

		m_LayerStack.OnEvent(e);

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::WindowCloseFunc));

		//PV_DEBUG_LOG(e.ToString().c_str());
	}

	bool Application::WindowCloseFunc(WindowCloseEvent & e) {
		IsAppRunning = false;
		return true;
	}

	void * Application::GetGraphicsAPI() {
		return s_GraphicsAPI;
	}

	void * Application::GetWindow() {
		return s_Window;
	}

}
