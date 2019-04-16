#include "pch.h"
#include "openglapi.h"

#if defined(PV_RENDERING_API_OPENGL) || defined(PV_RENDERING_API_BOTH)

#include <glad/glad.h>
#include <glad/glad_wgl.h>

#include "platform/gethwnd.h"

namespace prev {

	GraphicsAPI * GraphicsAPI::UseOpenGL(void * windowRawPointer, WindowAPI windowApi, GraphicsDesc & graphicsDesc) {
		OpenGLAPI * api = new OpenGLAPI(windowRawPointer, windowApi, graphicsDesc);

		if (!api->m_Status) {
			PV_POST_ERROR("Unable to Use DirectX11");
			delete api;
			return nullptr;
		}

		return api;
	}

	OpenGLAPI::OpenGLAPI(void * windowRawPointer, WindowAPI windowApi, GraphicsDesc & graphicsDesc) {
		m_Data.Width = graphicsDesc.Width;
		m_Data.Height = graphicsDesc.Height;
		m_Data.Vsync = graphicsDesc.Vsync;
		m_Data.Fullscreen = graphicsDesc.Fullscreen;

		if (windowApi == WindowAPI::WINDOWING_API_WIN32) {
			m_Data.HWnd = (HWND)windowRawPointer;
		} else if (windowApi == WindowAPI::WINDOWING_API_GLFW) {
			m_Data.HWnd = GetHWND(windowRawPointer);
		}

		m_Status = InitializeOpenGL();

		if (!m_Status)
			PV_POST_ERROR("Unable to initialize glad");

		if (m_Data.Vsync) 
			wglSwapIntervalEXT(1);
		else
			wglSwapIntervalEXT(0);

		m_RenderingAPI = RenderingAPI::RENDERING_API_OPENGL;

		return;
	}

	OpenGLAPI::~OpenGLAPI() {
		wglDeleteContext(m_Data.OpenGLRenderingContext);
	}

	void OpenGLAPI::StartFrame() {
		glClearColor(1, 0, 1, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLAPI::EndFrame() {
		SwapBuffers(m_Data.HandleToDeviceContext);
	}

	bool OpenGLAPI::InitializeOpenGL() {
		m_Data.PFD = {
				sizeof(PIXELFORMATDESCRIPTOR),
				1,
				PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
				PFD_TYPE_RGBA,
				32,
				8, 0, 8, 8, 8, 16,
				8, 24,
				0,
				0, 0, 0, 0,
				24,
				8,
				0,
				PFD_MAIN_PLANE,
				0,
				0, 0, 0
		};

		m_Data.HandleToDeviceContext = GetDC(m_Data.HWnd);

		int letWindowsChooseThisPixelFormat;
		letWindowsChooseThisPixelFormat = ChoosePixelFormat(m_Data.HandleToDeviceContext, &m_Data.PFD);
		if (!SetPixelFormat(m_Data.HandleToDeviceContext, letWindowsChooseThisPixelFormat, &m_Data.PFD)) {
			PV_POST_ERROR("Unable to set pixel format opengl");
			return false;
		}

		m_Data.OpenGLRenderingContext = wglCreateContext(m_Data.HandleToDeviceContext);
		if (!wglMakeCurrent(m_Data.HandleToDeviceContext, m_Data.OpenGLRenderingContext)) {
			PV_POST_ERROR("Unable to create context opengl");
			return false;
		}

		bool isReady = false;

		isReady = LoadGladOpenGL([](const char * func)->void * {
			void * p = (void *)wglGetProcAddress(func);
			if (p == 0 ||
				(p == (void *)0x1) ||
				(p == (void *)0x2) ||
				(p == (void *)0x3) ||
				(p == (void *)-1)) {
				HMODULE module = LoadLibraryA("opengl32.dll");
				p = (void *)GetProcAddress(module, func);
			}
			return p;
		});

		if (!isReady)
			return false;

		isReady = LoadwglGladOpenGL(m_Data.HandleToDeviceContext, [](const char * func)->void * {
			void * p = (void *)wglGetProcAddress(func);
			return p;
		});

		if (!isReady)
			return false;

		m_Data.Description = (char *)glGetString(GL_VERSION);

		return true;
	}

	bool OpenGLAPI::LoadGladOpenGL(void * (*glProc)(const char *)) {
		if (!gladLoadGLLoader(glProc)) {
			PV_POST_ERROR("Unable to initialize glad");
			return false;
		}
		return true;
	}

	bool OpenGLAPI::LoadwglGladOpenGL(HDC hdc, void * (*glProc)(const char *)) {
		if (!gladLoadWGLLoader(glProc, hdc)) {
			PV_POST_ERROR("Unable to initialize wglGlad");
			return false;
		}
		return true;
	}

}

#endif