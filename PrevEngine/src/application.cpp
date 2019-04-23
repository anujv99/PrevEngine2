#include "pch.h"
#include "application.h"

#include "engine/window.h"
#include "engine/graphicsapi.h"

#include "engine/imgui/imguilogger.h"
#include "engine/imgui/imguiconsole.h"

namespace prev {

	Window * s_Window = nullptr;
	GraphicsAPI * s_GraphicsAPI = nullptr;

	Application::Application() {

		WindowDesc winDesc;
		s_Window = Window::Create(winDesc, WindowAPI::WINDOWING_API_GLFW);
		if (s_Window == nullptr) {
			IsAppReady = false;
			return;
		}

		GraphicsDesc graphicsDesc(winDesc.Width, winDesc.Height);
		graphicsDesc.Vsync = false;
		graphicsDesc.Fullscreen = true;
		s_GraphicsAPI = GraphicsAPI::Create(s_Window->GetRawPointer(), s_Window->m_WindowAPI, graphicsDesc, RenderingAPI::RENDERING_API_DIRECTX);
		if (s_GraphicsAPI == nullptr) {
			IsAppReady = false;
			return;
		}

		s_Window->SetEventCallbackFunc(BIND_EVENT_FN(Application::EventCallbackFunc));
		Timer::FPSCounter(false);

		IMGUI_CALL(m_ImGuiLayer = new ImGuiLayer(s_Window->m_WindowAPI, s_GraphicsAPI->m_RenderingAPI));
		IMGUI_CALL(m_LayerStack.PushOverlay(new ImGuiLogger()));
		IMGUI_CALL(

			auto imguiconsole = new ImGuiConsole(); 
			m_LayerStack.PushOverlay(imguiconsole);
			imguiconsole->AddConsoleCommand("exit", "Exit the program", [this](const std::vector<std::string> & cmdParam) -> void {
				IsAppRunning = false;
			});

			auto windowSizes = s_GraphicsAPI->GetSupportedResolution();
			std::stringstream ss;
			ss << "Set window size in real time\n";
			ss << "Supported Window Resolution\n";
			ss << "Use the index to set resolution\n";
			ss << "---------------------------\n";
			int i = 1;
			for (auto & resolution : windowSizes) {
				ss << i << ") " << resolution.first << ", " << resolution.second << "\n";
				i++;
			}

			imguiconsole->AddConsoleCommand("window_size", ss.str().c_str(), [this](const std::vector<std::string> & cmdParam) -> void {
				if (cmdParam.size() != 2) {
					return;
				}
				int index = std::atoi(cmdParam[1].c_str());

				s_GraphicsAPI->ChangeResolution(index);
			});
			imguiconsole->AddConsoleCommand("window_fullscreen",
											"Set window between fullscreen and windowed\n"
											"------------------------------------------\n"
											"For Fullscreen use : 1\n"
											"For Windowed use   : 0\n",
											[this](const std::vector<std::string> & cmdParam) -> void {
												if (cmdParam.size() != 2) {
													return;
												}
												bool fullscreen = std::atoi(cmdParam[1].c_str());
												s_GraphicsAPI->SetFullscreen(fullscreen);
											});
		);
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
		if (m_ImGuiLayer != nullptr) {
			delete m_ImGuiLayer;
			m_ImGuiLayer = nullptr;
		}
		return;
	}

	void Application::Run() {
		while (IsAppRunning) {
			Timer::Update();
			s_Window->Update();
			s_GraphicsAPI->StartFrame();

			m_LayerStack.OnUpdate();

			IMGUI_CALL (
				m_ImGuiLayer->StartFrame();
				m_LayerStack.OnImGuiUpdate();
				m_ImGuiLayer->EndFrame();
			);

			s_GraphicsAPI->EndFrame();
		}
	}

	void Application::EventCallbackFunc(Event & e) {

		m_LayerStack.OnEvent(e);
		m_ImGuiLayer->OnEvent(e);

		if (e.GetCategoryFlags() & EventCategoryApplication)
			s_GraphicsAPI->OnEvent(e);

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
