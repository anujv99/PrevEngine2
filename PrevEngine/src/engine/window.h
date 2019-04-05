#pragma once

#include <string>

namespace prev {

	struct WindowDesc {
		WindowDesc(unsigned int width = 1280, unsigned int height = 720, std::string title = "PrevEngineButtPlug") :
			Width(width), Height(height), Title(title) {
		}
		unsigned int Width;
		unsigned int Height;
		std::string Title;
	};

	class Window {
	public:
		~Window() { };
		virtual bool Update() = 0;
	protected:
		Window() { };
	public:
		static Window * Create(const WindowDesc & windowDesc = WindowDesc());
	};

}
