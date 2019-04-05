#include "pch.h"
#include "win32window.h"

namespace prev {

	Window * Window::Create(const WindowDesc & windowDesc) {
		return new Win32Window(windowDesc);
	}

	LRESULT CALLBACK WindowProc(_In_ HWND hWnd, _In_ UINT msg, _In_ WPARAM wParam, _In_ LPARAM lParam) {
		switch (msg) {
		case WM_DESTROY:
			PostQuitMessage(69);
			break;
		default:
			return DefWindowProcA(hWnd, msg, wParam, lParam);
		}

		return 0;
	}

	Win32Window::Win32Window(const WindowDesc & windowDesc) {
		m_Data.Width = windowDesc.Width;
		m_Data.Height = windowDesc.Height;
		m_Data.Title = windowDesc.Title;
		m_Data.WindowClassString = "PREVENGINERESERVEDWINDOWCLASSNAME";
		m_Data.HInstance = GetModuleHandleA(nullptr);

		m_Status = RegisterWindowClass();
		if (!m_Status)
			return;

		m_Status = CreateAndShowWindow();
		if (!m_Status)
			return;
	}

	Win32Window::~Win32Window() {
		DestroyWindow(m_Data.HWnd);
		m_Data.HWnd = nullptr;
		UnregisterClassA(m_Data.WindowClassString.c_str(), m_Data.HInstance);
		m_Data.HInstance = nullptr;
		return;
	}

	bool Win32Window::Update() {
		if (PeekMessage(&m_Data.Msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&m_Data.Msg);
			DispatchMessage(&m_Data.Msg);
		}
		if (m_Data.Msg.message == WM_QUIT) {
			return false;
		}
		return true;
	}

	bool Win32Window::RegisterWindowClass() {
		WNDCLASSEXA wd;
		ZeroMemory(&wd, sizeof(wd));

		wd.cbSize			= sizeof(wd);
		wd.style			= CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
		wd.lpfnWndProc		= WindowProc;
		wd.cbClsExtra		= 0;
		wd.cbWndExtra		= 0;
		wd.hInstance		= m_Data.HInstance;
		wd.lpszClassName	=  m_Data.WindowClassString.c_str();
		wd.hIcon			= nullptr;
		wd.hCursor			= nullptr;
		wd.lpszMenuName		= nullptr;
		wd.hbrBackground	= nullptr;
		wd.hIconSm			= nullptr;

		ATOM status = RegisterClassExA(&wd);

		if (status == 0) {
			throw std::exception("Implement Error Handling");
			return false;
		}

		return true;
	}

	bool Win32Window::CreateAndShowWindow() {
		RECT rc;
		rc.left = 0;
		rc.top = 0;
		rc.right = rc.left + m_Data.Width;
		rc.bottom = rc.top + m_Data.Height;

		AdjustWindowRectEx(&rc, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
						   NULL, WS_EX_APPWINDOW);

		m_Data.HWnd = CreateWindowExA(
			WS_EX_APPWINDOW,
			m_Data.WindowClassString.c_str(),
			m_Data.Title.c_str(),
			WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
			CW_USEDEFAULT, CW_USEDEFAULT,
			rc.right - rc.left, rc.bottom - rc.top,
			NULL,
			NULL,
			m_Data.HInstance,
			NULL
		);

		if (!m_Data.HWnd) {
			throw std::exception("Implement Error Handling");
			return false;
		}

		ShowWindow(m_Data.HWnd, SW_SHOWNORMAL);
		return true;
	}

}
