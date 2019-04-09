#include "pch.h"
#include "directxapi.h"

#include "api/glfwhwnd.h"

#define CHECK_AND_POST_ERROR(hr, string, ...) { if (FAILED(hr)) { PV_POST_ERROR(string); __VA_ARGS__; return false; }}

namespace prev {

	GraphicsApi * GraphicsApi::UseDirectX(void * windowRawPointer, WindowAPI windowApi, GraphicsDesc & graphicsDesc) {
		DirectXAPI * api = new DirectXAPI(windowRawPointer, windowApi, graphicsDesc);

		if (!api->m_Status) {
			PV_POST_ERROR("Unable to Use DirectX11");
			delete api;
			return nullptr;
		}

		return api;
	}

	DirectXAPI::DirectXAPI(void * windowRawPointer, WindowAPI windowApi, GraphicsDesc & graphicsDesc) {
		m_Data.Width		= graphicsDesc.Width;
		m_Data.Height		= graphicsDesc.Height;
		m_Data.Vsync		= graphicsDesc.Vsync;
		m_Data.Fullscreen	= graphicsDesc.Fullscreen;
		
		if (windowApi == WindowAPI::WINDOWING_API_WIN32) {
			m_Data.HWnd = (HWND)windowRawPointer;
		} else if (windowApi == WindowAPI::WINDOWING_API_GLFW) {
			m_Data.HWnd = GetHWNDFromGLFW(windowRawPointer);
		}

		m_Status = CheckVideoAdapter();
		if (!m_Status) {
			PV_POST_ERROR("Unable to get video card props");
			return;
		}

		m_Status = CreateDeviceAndSwapChain();
		if (!m_Status) {
			PV_POST_ERROR("Unable to Initialize DirectX 11");
			return;
		}

		return;

	}

	DirectXAPI::~DirectXAPI() {

	}

	void DirectXAPI::StartFrame() {
		static float color[] = { 0, 0, 1, 1 };
		m_Data.DeviceContext->ClearRenderTargetView(m_Data.RenderTarget.Get(), color);
		m_Data.DeviceContext->ClearDepthStencilView(m_Data.DepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);

		return;
	}

	void DirectXAPI::EndFrame() {
		if (m_Data.Vsync)
			m_Data.SwapChain->Present(1, 0);
		else
			m_Data.SwapChain->Present(0, 0);

		return;
	}

	bool DirectXAPI::CheckVideoAdapter() {
		Microsoft::WRL::ComPtr<IDXGIFactory> d3dFactory;
		Microsoft::WRL::ComPtr<IDXGIAdapter> videoAdapter;
		Microsoft::WRL::ComPtr<IDXGIOutput> videoOutput;

		UINT numModes = 0;
		DXGI_MODE_DESC * outputModes = nullptr;
		DXGI_ADAPTER_DESC adapterDesc;

		HRESULT hr;
		hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void **)d3dFactory.GetAddressOf());
		CHECK_AND_POST_ERROR(hr, "Unable to get DXGIFactory");

		hr = d3dFactory->EnumAdapters(0, videoAdapter.GetAddressOf());
		CHECK_AND_POST_ERROR(hr, "Unable to get primary adapter");

		hr = videoAdapter->GetDesc(&adapterDesc);
		CHECK_AND_POST_ERROR(hr, "Unable to get Video adapter description");

		hr = videoAdapter->EnumOutputs(0, videoOutput.GetAddressOf());
		CHECK_AND_POST_ERROR(hr, "Unable to get primary adapter output");

		hr = videoOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, 0, &numModes, NULL);
		CHECK_AND_POST_ERROR(hr, "Unable to get primary output display mode list");

		outputModes = new DXGI_MODE_DESC[numModes];

		hr = videoOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, 0, &numModes, outputModes);
		CHECK_AND_POST_ERROR(hr, "Unable to get primary output display mode list", delete[] outputModes);

		bool found = false;

		for (UINT i = 0; i < numModes; i++) {
			if (outputModes[i].Width == m_Data.Width) {
				if (outputModes[i].Height == m_Data.Height) {
					m_Data.RefreshRate.Numerator = outputModes[i].RefreshRate.Numerator;
					m_Data.RefreshRate.Denominator = outputModes[i].RefreshRate.Denominator;
					found = true;
				}
			}
		}

		delete[] outputModes;

		if (!found) {
			PV_POST_ERROR("Unable to find display resolution");
			return false;
		}

		m_Data.AdapterDesc = std::string(_bstr_t(adapterDesc.Description));
		m_Data.DedicatedVideoMemory = adapterDesc.DedicatedVideoMemory / (1024 * 1024);

		return true;
	}

	bool DirectXAPI::CreateDeviceAndSwapChain() {
		DXGI_SWAP_CHAIN_DESC swapChainDesc;
		ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

		swapChainDesc.BufferCount = 2;
		swapChainDesc.BufferDesc.Width = m_Data.Width;
		swapChainDesc.BufferDesc.Height = m_Data.Height;
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

		if (m_Data.Vsync) {
			swapChainDesc.BufferDesc.RefreshRate.Numerator = m_Data.RefreshRate.Numerator;
			swapChainDesc.BufferDesc.RefreshRate.Denominator = m_Data.RefreshRate.Denominator;
		} else {
			swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
			swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
		}

		swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
		swapChainDesc.OutputWindow = m_Data.HWnd;
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

		if (m_Data.Fullscreen)
			swapChainDesc.Windowed = FALSE;
		else
			swapChainDesc.Windowed = TRUE;

		D3D_FEATURE_LEVEL featureLevels[] = {
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0,
		};

		D3D_FEATURE_LEVEL selectedFeatureLevel;

		HRESULT hr;

		hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL,
										   D3D11_CREATE_DEVICE_DEBUG, featureLevels, std::size(featureLevels), D3D11_SDK_VERSION,
										   &swapChainDesc, m_Data.SwapChain.GetAddressOf(), m_Data.Device.GetAddressOf(), &selectedFeatureLevel,
										   m_Data.DeviceContext.GetAddressOf());
		CHECK_AND_POST_ERROR(hr, "Unable to create D3D Device and swap chain");

		switch (selectedFeatureLevel) {



		case D3D_FEATURE_LEVEL_11_1:
			m_Data.DirectXFeatureLevel = "D3D_FEATURE_LEVEL_11_1";
			break;
		case D3D_FEATURE_LEVEL_11_0:
			m_Data.DirectXFeatureLevel = "D3D_FEATURE_LEVEL_11_0";
			break;
		case D3D_FEATURE_LEVEL_10_1:
			m_Data.DirectXFeatureLevel = "D3D_FEATURE_LEVEL_10_1";
			break;
		case D3D_FEATURE_LEVEL_10_0:
			m_Data.DirectXFeatureLevel = "D3D_FEATURE_LEVEL_10_0";
			break;
		case D3D_FEATURE_LEVEL_9_3:
			m_Data.DirectXFeatureLevel = "D3D_FEATURE_LEVEL_9_3";
			break;
		case D3D_FEATURE_LEVEL_9_2:
			m_Data.DirectXFeatureLevel = "D3D_FEATURE_LEVEL_9_2";
			break;
		case D3D_FEATURE_LEVEL_9_1:
			m_Data.DirectXFeatureLevel = "D3D_FEATURE_LEVEL_9_1";
			break;
		default:
			PV_POST_ERROR("DirectX 11 Not supported");
			return false;
			break;
		}

		m_Data.FeatureLevel = selectedFeatureLevel;

		Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;

		hr = m_Data.SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void **)backBuffer.GetAddressOf());
		CHECK_AND_POST_ERROR(hr, "Unable to get back buffer");

		hr = m_Data.Device->CreateRenderTargetView(backBuffer.Get(), NULL, m_Data.RenderTarget.GetAddressOf());
		CHECK_AND_POST_ERROR(hr, "Unable to create render target view");

		D3D11_TEXTURE2D_DESC depthBufferDesc;
		ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

		depthBufferDesc.ArraySize = 1;
		depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthBufferDesc.CPUAccessFlags = 0;
		depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthBufferDesc.Width = m_Data.Width;
		depthBufferDesc.Height = m_Data.Height;
		depthBufferDesc.MipLevels = 1;
		depthBufferDesc.MiscFlags = 0;
		depthBufferDesc.SampleDesc.Count = 1;
		depthBufferDesc.SampleDesc.Quality = 0;
		depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;

		hr = m_Data.Device->CreateTexture2D(&depthBufferDesc, NULL, m_Data.DepthStencilBuffer.GetAddressOf());
		CHECK_AND_POST_ERROR(hr, "Unable to create render depth stencil buffer");

		D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
		ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

		depthStencilDesc.DepthEnable = true;
		depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
		depthStencilDesc.StencilEnable = true;
		depthStencilDesc.StencilReadMask = 0xFF;
		depthStencilDesc.StencilWriteMask = 0xFF;
		depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		hr = m_Data.Device->CreateDepthStencilState(&depthStencilDesc, m_Data.DepthStencilState.GetAddressOf());
		CHECK_AND_POST_ERROR(hr, "Unable to create depth stencil state");

		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
		ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

		depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice = 0;

		hr = m_Data.Device->CreateDepthStencilView(m_Data.DepthStencilBuffer.Get(), 
												  &depthStencilViewDesc, m_Data.DepthStencilView.GetAddressOf());
		CHECK_AND_POST_ERROR(hr, "Unable to create depth stencil view");

		m_Data.DeviceContext->OMSetRenderTargets(1, m_Data.RenderTarget.GetAddressOf(), m_Data.DepthStencilView.Get());

		D3D11_RASTERIZER_DESC rasterDesc;
		ZeroMemory(&rasterDesc, sizeof(rasterDesc));

		rasterDesc.AntialiasedLineEnable = false;
		rasterDesc.CullMode = D3D11_CULL_BACK;
		rasterDesc.DepthBias = 0;
		rasterDesc.DepthBiasClamp = 0.0f;
		rasterDesc.DepthClipEnable = true;
		rasterDesc.FillMode = D3D11_FILL_SOLID;
		rasterDesc.FrontCounterClockwise = false;
		rasterDesc.MultisampleEnable = false;
		rasterDesc.ScissorEnable = false;
		rasterDesc.SlopeScaledDepthBias = 0.0f;

		hr = m_Data.Device->CreateRasterizerState(&rasterDesc, m_Data.RasterizerState.GetAddressOf());
		CHECK_AND_POST_ERROR(hr, "Unable to create rasterizer state");

		m_Data.DeviceContext->RSSetState(m_Data.RasterizerState.Get());

		D3D11_VIEWPORT viewport;
		viewport.Width = (float)m_Data.Width;
		viewport.Height = (float)m_Data.Height;
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		viewport.TopLeftX = 0.0f;
		viewport.TopLeftY = 0.0f;

		m_Data.DeviceContext->RSSetViewports(1, &viewport);

		return true;
	}

}
