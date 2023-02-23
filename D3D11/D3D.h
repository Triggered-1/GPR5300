#pragma once
#include <d3d11.h>

#pragma comment(lib, "d3d11.lib")

class D3D
{
public:
	INT init(HWND hWnd, UINT width, UINT height, BOOL isFullscreen);
	void beginScene(FLOAT r, FLOAT g, FLOAT b); // prepare scene/frame 
	void endScene(); // present scene/frame
	void deInit();
	void clearDepthStencil();

	ID3D11Device* getDevice() const { return _pD3DDevice; }
	ID3D11DeviceContext* getDeviceContext() const { return _pD3DDeviceContext; }

private:
	ID3D11Device* _pD3DDevice = nullptr; // create pipeline objects, e.g. vertex and index buffer
	ID3D11DeviceContext* _pD3DDeviceContext = nullptr; // handle render pipeline, e.g. send input data to pipeline or do a draw call
	IDXGISwapChain* _pDXGISwapChain = nullptr; // holds references to front & back buffer and switch them
	ID3D11RenderTargetView* _pRenderTargetView = nullptr; // holds a reference to a texture we want to render on, here reference to back buffer
	ID3D11DepthStencilView* _pDepthStencilView = nullptr; // holds a reference to a texture for depth & stencil buffering
	ID3D11RasterizerState* _pRasterizerState = nullptr; // holds a reference to rasterizer stage settings
	ID3D11BlendState* _pBlendState = nullptr;
};

