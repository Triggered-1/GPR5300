#include "D3D.h"
#include "Utilities.h"
#include <xutility>

INT D3D::init(HWND hWnd, UINT width, UINT height, BOOL isFullscreen)
{
	HRESULT hr = {};

	// 1. set up presentation parameters
	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
	swapChainDesc.OutputWindow = hWnd; // handle to target window
	swapChainDesc.Windowed = !isFullscreen; // windowed mode or exclusive fullscreen
	swapChainDesc.BufferCount = 1; // at least one
	swapChainDesc.BufferDesc.Width = width;
	swapChainDesc.BufferDesc.Height = height;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // red - green - blue - alpha, 8bit each channel
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // back buffer usage
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD; // what happens to the frontbuffer after switch
	swapChainDesc.SampleDesc.Count = 1; // at least one

	//d3dpp.EnableAutoDepthStencil = TRUE; // activate depth & stencil buffering
	//d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8; // pixel format of depth & stencil buffer

	// 2. create Direct3D 11 device, device context & swap chain
	D3D_FEATURE_LEVEL supportedFeatureLevels[] = {
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0
	};

	D3D_FEATURE_LEVEL chosenFeatureLevel = {};

	hr = D3D11CreateDeviceAndSwapChain(
		nullptr, // adapter/graphic card, here default adapter
		D3D_DRIVER_TYPE_HARDWARE, // which renderer should be used, here graphic card
		nullptr, // handle to software renderer module, only if D3D_DRIVER_TYPE_SOFTWARE is used
		0, // optional parameters
		supportedFeatureLevels, std::size(supportedFeatureLevels), // which direct3d version are supported
		D3D11_SDK_VERSION, // with which sdk version is this application compiled
		&swapChainDesc, &_pDXGISwapChain, &_pD3DDevice, &chosenFeatureLevel, &_pD3DDeviceContext
	);
	CheckFailed(hr, 20);

	// 3. create render target view (target is back buffer)
	ID3D11Texture2D* pBackBufferTexture = nullptr;
	hr = _pDXGISwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBufferTexture));
	CheckFailed(hr, 22);

	hr = _pD3DDevice->CreateRenderTargetView(pBackBufferTexture, nullptr, &_pRenderTargetView);
	CheckFailed(hr, 24);

	safeRelease(pBackBufferTexture);

	// 4. create depth stencil buffer
	ID3D11Texture2D* pDepthStencilBuffer = nullptr;
	D3D11_TEXTURE2D_DESC depthStencilBufferDesc = {};
	depthStencilBufferDesc.Width = width;
	depthStencilBufferDesc.Height = height;
	depthStencilBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilBufferDesc.ArraySize = 1; // at least 1
	depthStencilBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilBufferDesc.SampleDesc.Count = 1;

	hr = _pD3DDevice->CreateTexture2D(&depthStencilBufferDesc, nullptr, &pDepthStencilBuffer);
	CheckFailed(hr, 26);

	hr = _pD3DDevice->CreateDepthStencilView(pDepthStencilBuffer, nullptr, &_pDepthStencilView);
	CheckFailed(hr, 28);

	safeRelease(pDepthStencilBuffer);

	// 5. create rasterizer state
	D3D11_RASTERIZER_DESC rasterizerStateDesc = {};
	rasterizerStateDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerStateDesc.CullMode = D3D11_CULL_BACK;
	rasterizerStateDesc.FrontCounterClockwise = FALSE;

	hr = _pD3DDevice->CreateRasterizerState(&rasterizerStateDesc, &_pRasterizerState);
	CheckFailed(hr, 29);

	//Create Blend State
	D3D11_BLEND_DESC blendDesc = {};
	D3D11_RENDER_TARGET_BLEND_DESC targetBlendDesc;
	targetBlendDesc.BlendEnable = true;
	targetBlendDesc.SrcBlend = D3D11_BLEND_SRC_ALPHA;
	targetBlendDesc.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	targetBlendDesc.BlendOp = D3D11_BLEND_OP_ADD;
	targetBlendDesc.SrcBlendAlpha = D3D11_BLEND_ONE;
	targetBlendDesc.DestBlendAlpha = D3D11_BLEND_ZERO;
	targetBlendDesc.BlendOpAlpha = D3D11_BLEND_OP_ADD;
	targetBlendDesc.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	blendDesc.RenderTarget[0] = targetBlendDesc;

	hr = _pD3DDevice->CreateBlendState(&blendDesc, &_pBlendState);
	CheckFailed(hr, 31);

	// 6. create viewport
	D3D11_VIEWPORT viewport = {};
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	viewport.Width = width;
	viewport.Height = height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	// 7. set up render pipeline
	_pD3DDeviceContext->OMSetRenderTargets(1, &_pRenderTargetView, _pDepthStencilView);
	_pD3DDeviceContext->RSSetViewports(1, &viewport);
	_pD3DDeviceContext->RSSetState(_pRasterizerState);
	_pD3DDeviceContext->OMSetBlendState(_pBlendState,nullptr,0xffffffff);

	return 0;
}

void D3D::beginScene(FLOAT r, FLOAT g, FLOAT b)
{
	// clear buffer
	FLOAT color[] = { r, g, b, 1.0f };
	_pD3DDeviceContext->ClearRenderTargetView(_pRenderTargetView, color);
	// clear depth stencil buffer
	_pD3DDeviceContext->ClearDepthStencilView(_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0xff);
}

void D3D::endScene()
{
	// present frame
	_pDXGISwapChain->Present(
		0, // (de-)activate vsync, 0 - deactivate, 1;2;3;4 - sync every 1/2/3/4 frames, is only working in fullscreen & if refresh rate is set
		0 // optional flags
	);
}

void D3D::deInit()
{
	safeRelease(_pRasterizerState);
	safeRelease(_pDepthStencilView);
	safeRelease(_pRenderTargetView);
	safeRelease(_pDXGISwapChain);
	safeRelease(_pD3DDeviceContext);
	safeRelease(_pD3DDevice);
}

void D3D::clearDepthStencil()
{
	_pD3DDeviceContext->ClearDepthStencilView(_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0xff);
}
