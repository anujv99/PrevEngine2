#pragma once

#if defined(PV_RENDERING_API_OPENGL) || defined(PV_RENDERING_API_BOTH)

#include "engine/graphicsapi.h"

namespace prev {

	class OpenGLAPI : public GraphicsAPI {
	public:
		OpenGLAPI(void * windowRawPointer, WindowAPI windowApi, GraphicsDesc & graphicsDesc);
		~OpenGLAPI();

		virtual void StartFrame() override;
		virtual void EndFrame() override;
	private:
		bool InitializeOpenGL();
		bool LoadGladOpenGL(void * (*glProc)(const char *));
		bool LoadwglGladOpenGL(HDC hdc, void * (*glProc)(const char *));
	public:
		struct OpenGLGraphicsData {
			unsigned int Width;
			unsigned int Height;
			bool Vsync;
			bool Fullscreen;
			std::string Description;
			HWND HWnd;
			PIXELFORMATDESCRIPTOR PFD;
			HDC HandleToDeviceContext;
			HGLRC OpenGLRenderingContext;
		};
		OpenGLGraphicsData m_Data;
	public:
		bool m_Status = false;
	};

}

#endif