#pragma once

#include "engine/window.h"

namespace prev {

	enum class RenderingAPI {
		RENDERING_API_DIRECTX,
		RENDERING_API_OPENGL,
		RENDERING_API_UNINIT // Uninitialized
	};

	struct GraphicsDesc {
		GraphicsDesc(unsigned int width, unsigned int height, bool isVsync = true, bool isFullscreen = false) :
			Width(width), Height(height), Vsync(isVsync), Fullscreen(isFullscreen) { }

		unsigned int Width;
		unsigned int Height;
		bool Vsync;
		bool Fullscreen;
	};

	class GraphicsAPI {
	public:
		virtual void StartFrame() = 0;
		virtual void EndFrame() = 0;
	public:
		RenderingAPI m_RenderingAPI = RenderingAPI::RENDERING_API_UNINIT;
	protected:
		GraphicsAPI() { }
	protected:
		// To get window raw pointer use GetRawPointer method in Window class
		static GraphicsAPI * UseDirectX(void * windowRawPointer, WindowAPI windowApi, GraphicsDesc & graphicsDesc);
		static GraphicsAPI * UseOpenGL(void * windowRawPointer, WindowAPI windowApi, GraphicsDesc & graphicsDesc);
	public:
#if defined(PV_RENDERING_API_OPENGL) || defined(PV_RENDERING_API_DIRECTX)
		static GraphicsAPI * Create(void * windowRawPointer, WindowAPI windowApi, GraphicsDesc & graphicsDesc) {
			#ifdef PV_RENDERING_API_OPENGL
				return UseOpenGL(windowRawPointer, windowApi, graphicsDesc);
			#else
				return UseDirectX(windowRawPointer, windowApi, graphicsDesc);
			#endif
		}
#elif defined(PV_RENDERING_API_BOTH)
		static GraphicsAPI * Create(void * windowRawPointer, WindowAPI windowApi, GraphicsDesc & graphicsDesc, RenderingAPI renderingAPI = RenderingAPI::RENDERING_API_DIRECTX) {
			if (renderingAPI == RenderingAPI::RENDERING_API_OPENGL) {
				return UseOpenGL(windowRawPointer, windowApi, graphicsDesc);
			} else if (renderingAPI == RenderingAPI::RENDERING_API_DIRECTX) {
				return UseDirectX(windowRawPointer, windowApi, graphicsDesc);
			} else {
				PV_POST_FATAL("Please Pass a valid rendering api\n"
							  "For OpenGL use RENDERING_API_OPENGL\n"
							  "For DirectX 11 use RENDERING_API_DIRECTX\n");
				return nullptr;
			}
		}
#else
		static GraphicsAPI * Create(void * windowRawPointer, WindowAPI windowApi, GraphicsDesc & graphicsDesc) {
			PV_POST_FATAL("Please Define the rendering api symbols\n"
						  "For OpenGL use PV_RENDERING_API_OPENGL\n"
						  "For DirectX 11 use PV_RENDERING_API_DIRECTX\n"
						  "To Compile both use PV_RENDERING_API_BOTH");
		}
	}
#endif
	};

}
