#include "Material.h"
#include "Utilities.h"
#include "WICTextureLoader.h"
#include <d3dcompiler.h>

#pragma comment(lib, "d3dcompiler.lib")

INT Material::init(ID3D11Device* pD3DDevice, LPCTSTR textureFilename)
{
	INT error = 0;

	error = createVertexShader(pD3DDevice);
	CheckError(error);

	error = createPixelShader(pD3DDevice);
	CheckError(error);

	error = createMatrixBuffer(pD3DDevice);
	CheckError(error);

	error = createTextureAndSamplerState(pD3DDevice, textureFilename);
	CheckError(error);

	//// material
	//_material.Ambient = { 1.0f, 1.0f, 1.0f, 1.0f };
	//_material.Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	//_material.Specular = { 1.0f, 1.0f, 1.0f, 1.0f };
	//_material.Power = 1024.0f;
	//_material.Emissive = { 0.0f, 0.0f, 0.0f, 1.0f };

	return 0;
}

void Material::render(ID3D11DeviceContext* pD3DDeviceContext)
{
	// set shader
	pD3DDeviceContext->VSSetShader(_pVertexShader, nullptr, 0);
	pD3DDeviceContext->PSSetShader(_pPixelShader, nullptr, 0);

	// set input layout
	pD3DDeviceContext->IASetInputLayout(_pInputLayout);

	// set matrix buffer
	pD3DDeviceContext->VSSetConstantBuffers(0, 1, &_pMatrixBuffer);

	// set texture and sampler state
	pD3DDeviceContext->PSSetShaderResources(0, 1, &_pTexture);
	pD3DDeviceContext->PSSetSamplers(0, 1, &_pSamplerState);

	//// set material
	//pD3DDevice->SetMaterial(&_material);
}

void Material::deInit()
{
	safeRelease(_pSamplerState);
	safeRelease(_pTexture);
	safeRelease(_pMatrixBuffer);
	safeRelease(_pInputLayout);
	safeRelease(_pPixelShader);
	safeRelease(_pVertexShader);
}

void Material::setMatrices(ID3D11DeviceContext* pD3DDeviceContext, const XMFLOAT4X4& worldMatrix, const XMFLOAT4X4& viewMatrix, const XMFLOAT4X4& projectionMatrix)
{
	// data
	XMMATRIX w = XMLoadFloat4x4(&worldMatrix);
	XMMATRIX v = XMLoadFloat4x4(&viewMatrix);
	XMMATRIX p = XMLoadFloat4x4(&projectionMatrix);

	XMMATRIX wvp = w * v * p;

	// map data to cpu memory
	D3D11_MAPPED_SUBRESOURCE data = {};
	HRESULT hr = pD3DDeviceContext->Map(_pMatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &data);
	if (FAILED(hr)) return;

	// write data
	MatrixBuffer* pMatrixBuffer = static_cast<MatrixBuffer*>(data.pData);
	XMStoreFloat4x4(&(pMatrixBuffer->WorldViewProjectionMatrix), XMMatrixTranspose(wvp));
	XMStoreFloat4x4(&(pMatrixBuffer->WorldMatrix), XMMatrixTranspose(w));

	// unmap data and write back to gpu memory
	pD3DDeviceContext->Unmap(_pMatrixBuffer, 0);
}

INT Material::createVertexShader(ID3D11Device* pD3DDevice)
{
	ID3DBlob* pCompiledShader = nullptr;
	HRESULT hr = {};

	hr = D3DReadFileToBlob(L"LightVertexShader.cso", &pCompiledShader);

	CheckFailed(hr, 60);

	// create vertex shader object
	hr = pD3DDevice->CreateVertexShader(
		pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), // compiled shader code & size
		nullptr, // pre-compiled shader stuff
		&_pVertexShader
	);
	CheckFailed(hr, 62);

	// create input layout
	INT error = createInputLayout(pD3DDevice, pCompiledShader);
	CheckError(error);

	safeRelease(pCompiledShader);

	return 0;
}

INT Material::createPixelShader(ID3D11Device* pD3DDevice)
{
	ID3DBlob* pCompiledShader = nullptr;
	//HRESULT hr = D3DReadFileToBlob(L"ColorPixelShader.cso", &pCompiledShader);
	//HRESULT hr = D3DReadFileToBlob(L"TexturePixelShader.cso", &pCompiledShader);
	HRESULT hr = D3DReadFileToBlob(L"LightPixelShader.cso", &pCompiledShader);
	CheckFailed(hr, 64);

	hr = pD3DDevice->CreatePixelShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), nullptr, &_pPixelShader);
	CheckFailed(hr, 66);

	safeRelease(pCompiledShader);

	return 0;
}

INT Material::createInputLayout(ID3D11Device* pD3DDevice, ID3DBlob* pCompiledShader)
{
	const int count = 4;
	D3D11_INPUT_ELEMENT_DESC elements[count] = {};

	// position
	elements[0].SemanticName = "POSITION";
	elements[0].SemanticIndex = 0;
	elements[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	elements[0].AlignedByteOffset = 0;

	// normal
	elements[1].SemanticName = "NORMAL";
	elements[1].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	elements[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;

	// uv
	elements[2].SemanticName = "TEXCOORD";
	elements[2].Format = DXGI_FORMAT_R32G32_FLOAT;
	elements[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;

	// color
	elements[3].SemanticName = "COLOR";
	elements[3].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	elements[3].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;

	HRESULT hr = pD3DDevice->CreateInputLayout(elements, count, pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), &_pInputLayout);
	CheckFailed(hr, 68);

	return 0;
}

INT Material::createMatrixBuffer(ID3D11Device* pD3DDevice)
{
	D3D11_BUFFER_DESC desc = {};
	desc.ByteWidth = sizeof(MatrixBuffer);
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	HRESULT hr = pD3DDevice->CreateBuffer(&desc, nullptr, &_pMatrixBuffer);
	CheckFailed(hr, 61);

	return 0;
}

INT Material::createTextureAndSamplerState(ID3D11Device* pD3DDevice, LPCTSTR textureFilename)
{
	// texture
	HRESULT hr = CreateWICTextureFromFile(pD3DDevice, textureFilename, nullptr, &_pTexture);
	CheckFailed(hr, 63);
	
	// sampler state
	D3D11_SAMPLER_DESC desc = {};
	desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	
	//pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	//pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
	//pD3DDevice->SetSamplerState(0, D3DSAMP_BORDERCOLOR, D3DCOLOR_XRGB(255, 0, 255));
	//pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	//pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	//pD3DDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	//pD3DDevice->SetSamplerState(0, D3DSAMP_MAXANISOTROPY, 16);
	//pD3DDevice->SetSamplerState(0, D3DSAMP_MAXMIPLEVEL, 0); // maximum used mip mapping level
	//pD3DDevice->SetSamplerState(0, D3DSAMP_MIPMAPLODBIAS, 0); 

	hr = pD3DDevice->CreateSamplerState(&desc, &_pSamplerState);
	CheckFailed(hr, 65);
	
	return 0;
}
