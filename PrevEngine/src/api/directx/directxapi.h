#pragma once

#if defined(PV_RENDERING_API_DIRECTX) || defined(PV_RENDERING_API_BOTH)

#include "engine/graphicsapi.h"

namespace prev {

	class DirectXAPI : public GraphicsAPI {
	public:
		DirectXAPI(void * windowRawPointer, WindowAPI windowApi, GraphicsDesc & graphicsDesc);
		~DirectXAPI();

		virtual void StartFrame() override;
		virtual void EndFrame() override;
	private:
		bool CheckVideoAdapter();
		bool CreateDeviceAndSwapChain();
	public:
		struct DirectXGraphicsData {
			unsigned int Width;
			unsigned int Height;
			unsigned int DedicatedVideoMemory;
			struct {
				unsigned int Numerator;
				unsigned int Denominator;
			} RefreshRate;
			bool Vsync;
			bool Fullscreen;

			std::string AdapterDesc;
			std::string DirectXFeatureLevel;

			// WindowStuff
			HWND HWnd;

			//DirectXStuff
			Microsoft::WRL::ComPtr<IDXGISwapChain>				SwapChain;
			Microsoft::WRL::ComPtr<ID3D11Device>				Device;
			Microsoft::WRL::ComPtr<ID3D11DeviceContext>			DeviceContext;
			Microsoft::WRL::ComPtr<ID3D11RenderTargetView>		RenderTarget;
			Microsoft::WRL::ComPtr<ID3D11Texture2D>				DepthStencilBuffer;
			Microsoft::WRL::ComPtr<ID3D11DepthStencilState>		DepthStencilState;
			Microsoft::WRL::ComPtr<ID3D11DepthStencilView>		DepthStencilView;
			Microsoft::WRL::ComPtr<ID3D11RasterizerState>		RasterizerState;

			D3D_FEATURE_LEVEL FeatureLevel;
		};
		DirectXGraphicsData m_Data;
	public:
		bool m_Status = false;
	};

}

#endif