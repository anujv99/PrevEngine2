#pragma once

#include "engine/layer/layer.h"
#include "engine/window.h"
#include "engine/graphicsapi.h"

namespace prev {

	class ImGuiLayer : public Layer {
	public:
		ImGuiLayer(WindowAPI windowAPI, RenderingAPI graphicsAPI);
		~ImGuiLayer();
	private:
		void ImGuiStuff();
	private:
		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate() override;
		virtual void OnEvent(Event & event) override;
	private:
		void ShowLogger();
	private:
		WindowAPI m_WindowAPI;
		RenderingAPI m_GraphicsAPI;
	private:
		bool MouseMoved(MouseMovedEvent & e);
		bool MouseButtonPressed(MouseButtonPressedEvent & e);
		bool MouseButtonReleased(MouseButtonReleasedEvent & e);
		bool MouseScrolled(MouseScrolledEvent & e);
		bool KeyPressed(KeyPressedEvent & e);
		bool KeyReleased(KeyReleasedEvent & e);
		bool CharacterInputEvent(CharacterEvent & e);
	};

}