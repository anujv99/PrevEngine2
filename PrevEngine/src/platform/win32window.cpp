#include "pch.h"
#include "win32window.h"

#include "engine/input/mousekeycodes.h"

namespace prev {

	Win32Window * s_GlobalInstance = nullptr;

	Window * Window::Create(const WindowDesc & windowDesc) {
		Win32Window * window = new Win32Window(windowDesc);
		if (!window->m_Status) {
			PV_POST_FATAL("Unable to create window");
			delete window;
			return nullptr;
		}
		return (Window *)window;
	}

	LRESULT CALLBACK WindowProc(_In_ HWND hWnd, _In_ UINT msg, _In_ WPARAM wParam, _In_ LPARAM lParam) {
		switch (msg) {
		case WM_MOUSEMOVE:
		{
			POINTS pt;
			pt = MAKEPOINTS(lParam);
			MouseMovedEvent e((float)pt.x, (float)pt.y);
			s_GlobalInstance->m_Data.CallbackFunction(e);
			break;
		}
		case WM_LBUTTONUP:
		{
			MouseButtonReleasedEvent e(PV_MOUSE_BUTTON_1);
			s_GlobalInstance->m_Data.CallbackFunction(e);
			break;
		}
		case WM_MBUTTONUP:
		{
			MouseButtonReleasedEvent e(PV_MOUSE_BUTTON_2);
			s_GlobalInstance->m_Data.CallbackFunction(e);
			break;
		}
		case WM_RBUTTONUP:
		{
			MouseButtonReleasedEvent e(PV_MOUSE_BUTTON_3);
			s_GlobalInstance->m_Data.CallbackFunction(e);
			break;
		}
		case WM_XBUTTONUP:
		{
			auto button = GET_XBUTTON_WPARAM(wParam);
			if (button == XBUTTON1) {
				MouseButtonReleasedEvent e(PV_MOUSE_BUTTON_4);
				s_GlobalInstance->m_Data.CallbackFunction(e);
				break;
			} else if (button == XBUTTON2) {
				MouseButtonReleasedEvent e(PV_MOUSE_BUTTON_5);
				s_GlobalInstance->m_Data.CallbackFunction(e);
				break;
			}
		}
		case WM_LBUTTONDOWN:
		{
			MouseButtonPressedEvent e(PV_MOUSE_BUTTON_1);
			s_GlobalInstance->m_Data.CallbackFunction(e);
			break;
		}
		case WM_MBUTTONDOWN:
		{
			MouseButtonPressedEvent e(PV_MOUSE_BUTTON_2);
			s_GlobalInstance->m_Data.CallbackFunction(e);
			break;
		}
		case WM_RBUTTONDOWN:
		{
			MouseButtonPressedEvent e(PV_MOUSE_BUTTON_3);
			s_GlobalInstance->m_Data.CallbackFunction(e);
			break;
		}
		case WM_XBUTTONDOWN:
		{
			auto button = GET_XBUTTON_WPARAM(wParam);
			if (button == XBUTTON1) {
				MouseButtonPressedEvent e(PV_MOUSE_BUTTON_4);
				s_GlobalInstance->m_Data.CallbackFunction(e);
				break;
			} else if (button == XBUTTON2) {
				MouseButtonPressedEvent e(PV_MOUSE_BUTTON_5);
				s_GlobalInstance->m_Data.CallbackFunction(e);
				break;
			}
		}
		case WM_MOUSEWHEEL:
		{
			MouseScrolledEvent e(0.0f, (float)GET_WHEEL_DELTA_WPARAM(wParam));
			s_GlobalInstance->m_Data.CallbackFunction(e);
			break;
		}
		case WM_KEYDOWN:
		{
			if (lParam & 0x40000000) {
				KeyPressedEvent e((int)wParam, true);
				s_GlobalInstance->m_Data.CallbackFunction(e);
				break;
			} else {
				KeyPressedEvent e((int)wParam, false);
				s_GlobalInstance->m_Data.CallbackFunction(e);
				break;
			}
		}
		case WM_KEYUP:
		{
			KeyReleasedEvent e((int)wParam);
			s_GlobalInstance->m_Data.CallbackFunction(e);
			break;
		}
		case WM_MOVE:
		{
			WindowMoveEvent e(unsigned int(LOWORD(lParam)), unsigned int(HIWORD(lParam)));
			s_GlobalInstance->m_Data.CallbackFunction(e);
			break;
		}
		case WM_SIZE:
		{
			WindowResizeEvent e(unsigned int(LOWORD(lParam)), unsigned int(HIWORD(lParam)));
			s_GlobalInstance->m_Data.CallbackFunction(e);
			break;
		}
		case WM_CLOSE:
		case WM_DESTROY:
		case WM_QUIT:
		{
			WindowCloseEvent e;
			s_GlobalInstance->m_Data.CallbackFunction(e);
			break;
		}
		default:
			return DefWindowProcA(hWnd, msg, wParam, lParam);
			break;
		}

		return 0;
	}

	Win32Window::Win32Window(const WindowDesc & windowDesc) {
		s_GlobalInstance = this;
		m_Data.Width = windowDesc.Width;
		m_Data.Height = windowDesc.Height;
		m_Data.Title = windowDesc.Title;
		m_Data.WindowClassString = "PREVENGINERESERVEDWINDOWCLASSNAME";
		m_Data.HInstance = GetModuleHandleA(nullptr);
		m_Data.CallbackFunction = BIND_EVENT_FN(Win32Window::DefaultEventCallbackFunction);

		m_Status = RegisterWindowClass();
		if (!m_Status) {
			PV_POST_FATAL("Unable to register window class");
			return;
		}

		m_Status = CreateAndShowWindow();
		if (!m_Status) {
			PV_POST_FATAL("Unable to create window from window class");
			return;
		}
	}

	Win32Window::~Win32Window() {
		DestroyWindow(m_Data.HWnd);
		m_Data.HWnd = nullptr;
		UnregisterClassA(m_Data.WindowClassString.c_str(), m_Data.HInstance);
		m_Data.HInstance = nullptr;
		s_GlobalInstance = nullptr;
		return;
	}

	void Win32Window::Update() {
		if (PeekMessage(&m_Data.Msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&m_Data.Msg);
			DispatchMessage(&m_Data.Msg);
		}
	}

	void Win32Window::SetEventCallbackFunc(std::function<void(Event &)> func) {
		m_Data.CallbackFunction = func;
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
		wd.lpszClassName	= m_Data.WindowClassString.c_str();
		wd.hIcon			= nullptr;
		wd.hCursor			= nullptr;
		wd.lpszMenuName		= nullptr;
		wd.hbrBackground	= nullptr;
		wd.hIconSm			= nullptr;

		ATOM status = RegisterClassExA(&wd);

		if (status == 0) {
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
			return false;
		}

		ShowWindow(m_Data.HWnd, SW_SHOWNORMAL);
		return true;
	}

}
