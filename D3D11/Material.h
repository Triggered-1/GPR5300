#pragma once
#include <d3d11.h>
#include <DirectXMath.h>

using namespace DirectX;

class Material
{
public:
	INT init(ID3D11Device* pD3DDevice, LPCTSTR textureFilename);
	void render(ID3D11DeviceContext* pD3DDeviceContext);
	void deInit();

	void setMatrices(ID3D11DeviceContext* pD3DDeviceContext, const XMFLOAT4X4& worldMatrix, const XMFLOAT4X4& viewMatrix, const XMFLOAT4X4& projectionMatrix);

private:
	INT createVertexShader(ID3D11Device* pD3DDevice);
	INT createPixelShader(ID3D11Device* pD3DDevice);
	INT createInputLayout(ID3D11Device* pD3DDevice, ID3DBlob* pCompiledShader);

	ID3D11VertexShader* _pVertexShader = nullptr;
	ID3D11PixelShader* _pPixelShader = nullptr;
	ID3D11InputLayout* _pInputLayout = nullptr;

	// constant buffer
	INT createMatrixBuffer(ID3D11Device* pD3DDevice);
	ID3D11Buffer* _pMatrixBuffer = nullptr;
	struct MatrixBuffer
	{
		XMFLOAT4X4 WorldViewProjectionMatrix;
		XMFLOAT4X4 WorldMatrix;
	};

	// texture
	INT createTextureAndSamplerState(ID3D11Device* pD3DDevice, LPCTSTR textureFilename);
	ID3D11ShaderResourceView* _pTexture = nullptr; // main texture
	ID3D11SamplerState* _pSamplerState = nullptr; // main sampler state
	 
	
	//D3DMATERIAL9 _material = {}; // surface material, light reflection settings
};

