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
		friend class Application;
	public:
		virtual ~GraphicsAPI() { }
		virtual void StartFrame() = 0;
		virtual void EndFrame() = 0;
		virtual void ChangeResolution(int index) = 0;
		virtual std::vector<std::pair<unsigned int, unsigned int>> GetSupportedResolution() = 0;

		virtual void OnEvent(Event & e) { };
		virtual void SetFullscreen(bool fullscreen) { };
	public:
		RenderingAPI m_RenderingAPI = RenderingAPI::RENDERING_API_UNINIT;
	protected:
		GraphicsAPI() { }
	protected:
		// To get window raw pointer use GetRawPointer method in Window class
		static GraphicsAPI * UseDirectX(void * windowRawPointer, WindowAPI windowApi, GraphicsDesc & graphicsDesc);
		static GraphicsAPI * UseOpenGL(void * windowRawPointer, WindowAPI windowApi, GraphicsDesc & graphicsDesc);
	private:
		static GraphicsAPI * GraphicsAPI::Create(void * windowRawPointer, WindowAPI windowApi, GraphicsDesc & graphicsDesc, RenderingAPI renderingAPI);
	};

}
