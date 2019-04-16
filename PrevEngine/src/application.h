#pragma once

#include "engine/events/event.h"
#include "engine/events/applicationevent.h"
#include "engine/layer/layerstack.h"

namespace prev {

	class Application {
		friend class ImGuiLayer;
	public:
		Application();
		~Application();
	public:
		void Run();
		void EventCallbackFunc(Event & e);
		bool WindowCloseFunc(WindowCloseEvent & e);
	private:
		static void * GetGraphicsAPI();
		static void * GetWindow();
	public:
		bool IsAppReady = true;
		bool IsAppRunning = true;
	private:
		LayerStack m_LayerStack;
	};

}
