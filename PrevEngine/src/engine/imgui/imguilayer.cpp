#include "pch.h"
#include "imguilayer.h"

#include <imgui.h>

#include "application.h"
#include "engine/input/keyboardkeycodes.h"
#include "engine/essentials/log.h"

#if defined(PV_RENDERING_API_DIRECTX) || defined(PV_RENDERING_API_BOTH)
#include <examples/imgui_impl_dx11.h>
#include <examples/imgui_impl_dx11.cpp>
#include "api/directx/directxapi.h"
#endif

#if defined(PV_RENDERING_API_OPENGL) || defined(PV_RENDERING_API_BOTH)
#define IMGUI_IMPL_OPENGL_LOADER_GLAD
#include <examples/imgui_impl_opengl3.h>
#include <examples/imgui_impl_opengl3.cpp>
#include "api/opengl/openglapi.h"
#endif

namespace prev {

	ImGuiLayer::ImGuiLayer(WindowAPI windowAPI, RenderingAPI graphicsAPI) {

		m_WindowAPI = windowAPI;
		m_GraphicsAPI = graphicsAPI;

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO & io = ImGui::GetIO(); (void)io;
		io.ConfigFlags		|= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags		|= ImGuiConfigFlags_DockingEnable;
		io.BackendFlags		|= ImGuiBackendFlags_HasMouseCursors;
		io.BackendFlags		|= ImGuiBackendFlags_HasSetMousePos;

		ImGui::StyleColorsDark();

		ImGuiStyle & style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		#if defined(PV_RENDERING_API_DIRECTX) || defined(PV_RENDERING_API_BOTH)
			if (m_GraphicsAPI == RenderingAPI::RENDERING_API_DIRECTX) {
				auto gapi = (GraphicsAPI *)Application::GetGraphicsAPI();
				DirectXAPI * dapi = (DirectXAPI *)gapi;
				ImGui_ImplDX11_Init(dapi->m_Data.Device.Get(), dapi->m_Data.DeviceContext.Get());
			}
		#endif

		#if defined(PV_RENDERING_API_OPENGL) || defined(PV_RENDERING_API_BOTH)
			if (m_GraphicsAPI == RenderingAPI::RENDERING_API_OPENGL) {
				auto gapi = (GraphicsAPI *)Application::GetGraphicsAPI();
				OpenGLAPI * oapi = (OpenGLAPI *)gapi;
				ImGui_ImplOpenGL3_Init("#version 330 core");
			}
		#endif

		{
			io.KeyMap[ImGuiKey_Tab]			= PV_KEYBOARD_KEY_TAB;
			io.KeyMap[ImGuiKey_LeftArrow]	= PV_KEYBOARD_KEY_LEFT;
			io.KeyMap[ImGuiKey_RightArrow]	= PV_KEYBOARD_KEY_RIGHT;
			io.KeyMap[ImGuiKey_UpArrow]		= PV_KEYBOARD_KEY_UP;
			io.KeyMap[ImGuiKey_DownArrow]	= PV_KEYBOARD_KEY_DOWN;
			io.KeyMap[ImGuiKey_PageUp]		= PV_KEYBOARD_KEY_PAGEUP;
			io.KeyMap[ImGuiKey_PageDown]	= PV_KEYBOARD_KEY_PAGEDOWN;
			io.KeyMap[ImGuiKey_Home]		= PV_KEYBOARD_KEY_HOME;
			io.KeyMap[ImGuiKey_End]			= PV_KEYBOARD_KEY_END;
			io.KeyMap[ImGuiKey_Insert]		= PV_KEYBOARD_KEY_INSERT;
			io.KeyMap[ImGuiKey_Delete]		= PV_KEYBOARD_KEY_DELETE;
			io.KeyMap[ImGuiKey_Backspace]	= PV_KEYBOARD_KEY_BACKSPACE;
			io.KeyMap[ImGuiKey_Space]		= PV_KEYBOARD_KEY_SPACE;
			io.KeyMap[ImGuiKey_Enter]		= PV_KEYBOARD_KEY_ENTER;
			io.KeyMap[ImGuiKey_Escape]		= PV_KEYBOARD_KEY_ESCAPE;
			io.KeyMap[ImGuiKey_A]			= PV_KEYBOARD_KEY_A;
			io.KeyMap[ImGuiKey_C]			= PV_KEYBOARD_KEY_C;
			io.KeyMap[ImGuiKey_V]			= PV_KEYBOARD_KEY_V;
			io.KeyMap[ImGuiKey_X]			= PV_KEYBOARD_KEY_X;
			io.KeyMap[ImGuiKey_Y]			= PV_KEYBOARD_KEY_Y;
			io.KeyMap[ImGuiKey_Z]			= PV_KEYBOARD_KEY_Z;
		}

		Window * win = (Window *)Application::GetWindow();
		auto [x, y] = win->GetWindowSize();

		m_WinSizeX = x;
		m_WinSizeY = y;
	}

	ImGuiLayer::~ImGuiLayer() {
		#if defined(PV_RENDERING_API_DIRECTX) || defined(PV_RENDERING_API_BOTH)
			if (m_GraphicsAPI == RenderingAPI::RENDERING_API_DIRECTX) {
				ImGui_ImplDX11_Shutdown();
			}
		#endif

		#if defined(PV_RENDERING_API_OPENGL) || defined(PV_RENDERING_API_BOTH)
			if (m_GraphicsAPI == RenderingAPI::RENDERING_API_OPENGL) {
				ImGui_ImplOpenGL3_Shutdown();
			}
		#endif
		ImGui::DestroyContext();
	}

	void ImGuiLayer::EndFrame() {
		ImGui::EndFrame();
		ImGui::Render();
		#if defined(PV_RENDERING_API_DIRECTX) || defined(PV_RENDERING_API_BOTH)
			if (m_GraphicsAPI == RenderingAPI::RENDERING_API_DIRECTX) {
				ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
			}
		#endif

		#if defined(PV_RENDERING_API_OPENGL) || defined(PV_RENDERING_API_BOTH)
			if (m_GraphicsAPI == RenderingAPI::RENDERING_API_OPENGL) {
				ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
			}
		#endif
	}

	void ImGuiLayer::StartFrame() {
		ImGuiIO & io = ImGui::GetIO();
		io.DeltaTime = Timer::GetDeltaTime();
		io.DisplaySize.x = (float)m_WinSizeX;
		io.DisplaySize.y = (float)m_WinSizeY;

		#if defined(PV_RENDERING_API_DIRECTX) || defined(PV_RENDERING_API_BOTH)
			if (m_GraphicsAPI == RenderingAPI::RENDERING_API_DIRECTX) {
				ImGui_ImplDX11_NewFrame();
			}
		#endif

		#if defined(PV_RENDERING_API_OPENGL) || defined(PV_RENDERING_API_BOTH)
			if (m_GraphicsAPI == RenderingAPI::RENDERING_API_OPENGL) {
				ImGui_ImplOpenGL3_NewFrame();
			}
		#endif

		ImGui::NewFrame();
	}

	void ImGuiLayer::OnEvent(Event & event) {
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<MouseMovedEvent>(BIND_EVENT_FN(ImGuiLayer::MouseMoved));
		dispatcher.Dispatch<MouseButtonPressedEvent>(BIND_EVENT_FN(ImGuiLayer::MouseButtonPressed));
		dispatcher.Dispatch<MouseButtonReleasedEvent>(BIND_EVENT_FN(ImGuiLayer::MouseButtonReleased));
		dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT_FN(ImGuiLayer::MouseScrolled));
		dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(ImGuiLayer::KeyPressed));
		dispatcher.Dispatch<KeyReleasedEvent>(BIND_EVENT_FN(ImGuiLayer::KeyReleased));
		dispatcher.Dispatch<CharacterEvent>(BIND_EVENT_FN(ImGuiLayer::CharacterInputEvent));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(ImGuiLayer::WindowSizeChanged));
	}

	bool ImGuiLayer::MouseMoved(MouseMovedEvent & e) {
		ImGuiIO & io = ImGui::GetIO();
		io.MousePos = ImVec2(e.GetX(), e.GetY());
		return false;
	}
	bool ImGuiLayer::MouseButtonPressed(MouseButtonPressedEvent & e) {
		ImGuiIO & io = ImGui::GetIO();
		io.MouseDown[e.GetMouseButton()] = true;
		return false;
	}
	bool ImGuiLayer::MouseButtonReleased(MouseButtonReleasedEvent & e) {
		ImGuiIO & io = ImGui::GetIO();
		io.MouseDown[e.GetMouseButton()] = false;
		return false;
	}
	bool ImGuiLayer::WindowSizeChanged(WindowResizeEvent & e) {
		m_WinSizeX = e.GetWidth();
		m_WinSizeY = e.GetHeight();
		return false;
	}
	bool ImGuiLayer::MouseScrolled(MouseScrolledEvent & e) {
		ImGuiIO & io = ImGui::GetIO();
		io.MouseWheelH = e.GetYOffset();
		io.MouseWheel = e.GetXOffset();
		return false;
	}
	bool ImGuiLayer::KeyPressed(KeyPressedEvent & e) {
		ImGuiIO & io = ImGui::GetIO();
		if (!e.IsRepeating()) {
			io.KeysDown[e.GetKeyCode()] = true;
		}
		return false;
	}
	bool ImGuiLayer::KeyReleased(KeyReleasedEvent & e) {
		ImGuiIO & io = ImGui::GetIO();
		io.KeysDown[e.GetKeyCode()] = false;
		return false;
	}

	bool ImGuiLayer::CharacterInputEvent(CharacterEvent & e) {
		ImGuiIO & io = ImGui::GetIO();
		io.AddInputCharacter(e.GetPressedChar());
		return false;
	}

}