#pragma once

#include "engine/layer/layer.h"
#include "engine/window.h"
#include "engine/graphicsapi.h"

// Use this macro for ImGui calls, so that you can easily disable them
#define IMGUI_CALL(...) __VA_ARGS__;

namespace prev {

	class ImGuiLayer {
		friend class Application;
	public:
		ImGuiLayer(WindowAPI windowAPI, RenderingAPI graphicsAPI);
		~ImGuiLayer();
	private:
		void StartFrame();
		void EndFrame();
		void OnEvent(Event & event);
	private:
		WindowAPI m_WindowAPI;
		RenderingAPI m_GraphicsAPI;
		unsigned int m_WinSizeX, m_WinSizeY;
	private:
		bool MouseMoved(MouseMovedEvent & e);
		bool KeyPressed(KeyPressedEvent & e);
		bool KeyReleased(KeyReleasedEvent & e);
		bool MouseScrolled(MouseScrolledEvent & e);
		bool CharacterInputEvent(CharacterEvent & e);
		bool MouseButtonPressed(MouseButtonPressedEvent & e);
		bool MouseButtonReleased(MouseButtonReleasedEvent & e);
		bool WindowSizeChanged(WindowResizeEvent & e);
	};

}