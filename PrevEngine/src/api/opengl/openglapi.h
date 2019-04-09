#pragma once

#include "engine/graphicsapi.h"

namespace prev {

	class OpenGLApi : public GraphicsApi {
	public:
		OpenGLApi(void * windowRawPointer, WindowAPI windowApi, GraphicsDesc & graphicsDesc);
		~OpenGLApi();

		virtual void StartFrame() override;
		virtual void EndFrame() override;
	private:
		bool InitializeOpenGL();
		bool LoadGladOpenGL(void * (*glProc)(const char *));
		bool LoadwglGladOpenGL(HDC hdc, void * (*glProc)(const char *));
	private:
		struct GraphicsData {
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
		GraphicsData m_Data;
	public:
		bool m_Status = false;
	};

}
