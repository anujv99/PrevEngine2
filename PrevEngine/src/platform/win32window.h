#pragma once

#ifdef PV_WINDOWING_API_WIN32

#include "engine/window.h"

namespace prev {

	class Win32Window : public Window {
		friend LRESULT CALLBACK WindowProc(_In_ HWND hWnd, _In_ UINT msg, _In_ WPARAM wParam, _In_ LPARAM lParam);
	public:
		Win32Window(const WindowDesc & windowDesc);
		~Win32Window();

		virtual void Update() override;
		virtual void SetEventCallbackFunc(std::function<void(Event &)> func) override;
		virtual void * GetRawPointer() override { return (void *)m_Data.HWnd; }
	private:
		bool RegisterWindowClass();
		bool CreateAndShowWindow();

		void DefaultEventCallbackFunction(Event & e) { }
	public:
		bool m_Status;
	private:
		struct WindowData {
			// Useful Info
			unsigned int Width;
			unsigned int Height;
			std::string Title;
			std::string WindowClassString;

			// Event Callback Function
			std::function<void(Event & e)> CallbackFunction;

			// WIN32 Stuff
			HINSTANCE HInstance;
			HWND HWnd;
			MSG Msg;
		};

		WindowData m_Data;
	};

}

#endif