#pragma once

#include "engine/window.h"

namespace prev {

	class Win32Window : public Window {
	public:
		Win32Window(const WindowDesc & windowDesc);
		~Win32Window();

		virtual bool Update() override;
	private:
		bool RegisterWindowClass();
		bool CreateAndShowWindow();
	public:
		bool m_Status;
	private:
		struct WindowData {
			unsigned int Width;
			unsigned int Height;
			std::string Title;
			std::string WindowClassString;

			//WIN32 Stuff
			HINSTANCE HInstance;
			HWND HWnd;
			MSG Msg;
		};

		WindowData m_Data;
	};

}