#include "pch.h"
#include "graphicsapi.h"

namespace prev {

#if defined(PV_RENDERING_API_OPENGL) || defined(PV_RENDERING_API_DIRECTX)
	GraphicsAPI * GraphicsAPI::Create(void * windowRawPointer, WindowAPI windowApi, GraphicsDesc & graphicsDesc, RenderingAPI renderingAPI) {
	#ifdef PV_RENDERING_API_OPENGL
		if (windowingAPI != WindowAPI::WINDOWING_API_WIN32) {
			PV_POST_ERROR("Cannot use DirectX when PV_RENDERING_API_DIRECTX is not defined!\nUsing OpenGL instead");
		}
		return UseOpenGL(windowRawPointer, windowApi, graphicsDesc);
	#else
		if (windowingAPI != WindowAPI::WINDOWING_API_WIN32) {
			PV_POST_ERROR("Cannot use OpenGL when PV_RENDERING_API_OPENGL is not defined!\nUsing DirectX instead");
		}
		return UseDirectX(windowRawPointer, windowApi, graphicsDesc);
	#endif
	}
#elif defined(PV_RENDERING_API_BOTH)
	GraphicsAPI * GraphicsAPI::Create(void * windowRawPointer, WindowAPI windowApi, GraphicsDesc & graphicsDesc, RenderingAPI renderingAPI) {
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
	GraphicsAPI * GraphicsAPI::Create(void * windowRawPointer, WindowAPI windowApi, GraphicsDesc & graphicsDesc, RenderingAPI renderingAPI) {
		PV_POST_FATAL("Please Define the rendering api symbols\n"
					  "For OpenGL use PV_RENDERING_API_OPENGL\n"
					  "For DirectX 11 use PV_RENDERING_API_DIRECTX\n"
					  "To Compile both use PV_RENDERING_API_BOTH");
	}
}
#endif

}