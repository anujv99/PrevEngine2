#pragma once

#include "engine/window.h"

namespace prev {

	class Application {
	public:
		Application();
		~Application();
	public:
		void Run();
	private:
		Window * m_Window = nullptr;
	};

}
