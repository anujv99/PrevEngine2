#pragma once

#include "engine/events/event.h"
#include "engine/events/applicationevent.h"
#include "engine/layer/layerstack.h"

namespace prev {

	class Application {
	public:
		Application();
		~Application();
	public:
		void Run();
		void EventCallbackFunc(Event & e);
		bool WindowCloseFunc(WindowCloseEvent & e);
	public:
		bool IsAppReady = true;
		bool IsAppRunning = true;
	private:
		LayerStack m_LayerStack;
	};

}
