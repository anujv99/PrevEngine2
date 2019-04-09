#pragma once

#include "engine/window.h"

namespace prev {

	struct GraphicsDesc {

		GraphicsDesc(unsigned int width, unsigned int height, bool isVsync = true, bool isFullscreen = false) :
			Width(width), Height(height), Vsync(isVsync), Fullscreen(isFullscreen) { }

		unsigned int Width;
		unsigned int Height;
		bool Vsync;
		bool Fullscreen;
	};

	class GraphicsApi {
	public:
		virtual void StartFrame() = 0;
		virtual void EndFrame() = 0;
	protected:
		GraphicsApi() { }
	public:
		// To get window raw pointer use GetRawPointer method in Window class
		static GraphicsApi * UseDirectX(void * windowRawPointer, WindowAPI windowApi, GraphicsDesc & graphicsDesc);
		static GraphicsApi * UseOpenGL(void * windowRawPointer, WindowAPI windowApi, GraphicsDesc & graphicsDesc);
	};

}
