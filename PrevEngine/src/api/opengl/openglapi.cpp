#include "pch.h"
#include "openglapi.h"

#include <glad/glad.h>
#include <glad/glad_wgl.h>

#include "api/glfwhwnd.h"

namespace prev {

	GraphicsApi * GraphicsApi::UseOpenGL(void * windowRawPointer, WindowAPI windowApi, GraphicsDesc & graphicsDesc) {
		OpenGLApi * api = new OpenGLApi(windowRawPointer, windowApi, graphicsDesc);

		if (!api->m_Status) {
			PV_POST_ERROR("Unable to Use DirectX11");
			delete api;
			return nullptr;
		}

		return api;
	}

	OpenGLApi::OpenGLApi(void * windowRawPointer, WindowAPI windowApi, GraphicsDesc & graphicsDesc) {
		m_Data.Width = graphicsDesc.Width;
		m_Data.Height = graphicsDesc.Height;
		m_Data.Vsync = graphicsDesc.Vsync;
		m_Data.Fullscreen = graphicsDesc.Fullscreen;

		if (windowApi == WindowAPI::WINDOWING_API_WIN32) {
			m_Data.HWnd = (HWND)windowRawPointer;
		} else if (windowApi == WindowAPI::WINDOWING_API_GLFW) {
			m_Data.HWnd = GetHWNDFromGLFW(windowRawPointer);
		}

		m_Status = InitializeOpenGL();

		if (!m_Status)
			PV_POST_ERROR("Unable to initialize glad");

		if (m_Data.Vsync) 
			wglSwapIntervalEXT(1);
		else
			wglSwapIntervalEXT(0);

		return;
	}

	OpenGLApi::~OpenGLApi() {
		wglDeleteContext(m_Data.OpenGLRenderingContext);
	}

	void OpenGLApi::StartFrame() {
		SwapBuffers(m_Data.HandleToDeviceContext);
		glClearColor(1, 0, 1, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLApi::EndFrame() {
		
	}

	bool OpenGLApi::InitializeOpenGL() {
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

	bool OpenGLApi::LoadGladOpenGL(void * (*glProc)(const char *)) {
		if (!gladLoadGLLoader(glProc)) {
			PV_POST_ERROR("Unable to initialize glad");
			return false;
		}
		return true;
	}

	bool OpenGLApi::LoadwglGladOpenGL(HDC hdc, void * (*glProc)(const char *)) {
		if (!gladLoadWGLLoader(glProc, hdc)) {
			PV_POST_ERROR("Unable to initialize wglGlad");
			return false;
		}
		return true;
	}

}