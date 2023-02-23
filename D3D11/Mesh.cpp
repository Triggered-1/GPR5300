#include <xutility>
#include <d3dcompiler.h>
#include "Mesh.h"
#include "Utilities.h"
#include "Vertex.h"
#include "MeshLoader.h"
#include "WICTextureLoader.h"


INT Mesh::init(ID3D11Device* pD3DDevice, string path, wstring shader)
{
	INT error = 0;
	m_meshData = MeshLoader::getInstance()->loadFromFile(path);

	error = createVertexBuffer(pD3DDevice);
	CheckError(error);

	error = createIndexBuffer(pD3DDevice);
	CheckError(error);

	error = createVertexShader(pD3DDevice,shader);
	CheckError(error);

	error = createPixelShader(pD3DDevice,shader);
	CheckError(error);

	error = createMatrixBuffer(pD3DDevice);
	CheckError(error);

	error = createTransparentcyBuffer(pD3DDevice);
	CheckError(error);

	error = createTextureAndSamplerState(pD3DDevice);
	CheckError(error);
	return 0;
}

void Mesh::update()
{
	XMStoreFloat4x4(&_worldMatrix, transform->GetTransformationMatrix());

}

void Mesh::render(ID3D11DeviceContext* pD3DDeviceContext, const XMMATRIX& pVMMatrix)
{
	pD3DDeviceContext->IASetInputLayout(_pInputLayout);
	pD3DDeviceContext->VSSetShader(_pVertexShader, nullptr, 0);
	pD3DDeviceContext->PSSetShader(_pPixelShader, nullptr, 0);

	SetMatrixBuffer(pD3DDeviceContext, pVMMatrix);
	SetTransparencyBuffer(pD3DDeviceContext);
	pD3DDeviceContext->PSSetShaderResources(0, 1, &_pTexture);
	pD3DDeviceContext->PSSetSamplers(0, 1, &_pSamplerState);


	// set vertex buffer
	static UINT offset = 0;
	pD3DDeviceContext->IASetVertexBuffers(0, 1, &_pVertexBuffer, &_vertexStride, &offset);

	// set index buffer
	pD3DDeviceContext->IASetIndexBuffer(_pIndexBuffer, DXGI_FORMAT_R16_UINT, offset);

	// set primitive topology
	pD3DDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// draw with index buffer
	pD3DDeviceContext->DrawIndexed(_indexCount, offset, offset);
}

void Mesh::deInit()
{
	safeRelease(_pIndexBuffer);
	safeRelease(_pVertexBuffer);
}

void Mesh::setTransparency(FLOAT transparency)
{
	_transparency = transparency;
}

void Mesh::SetMatrixBuffer(ID3D11DeviceContext* pD3DDeviceContext, const  XMMATRIX& pVMMatrix)
{
	// data
	XMMATRIX w = transform->GetTransformationMatrix();
	XMMATRIX vp = pVMMatrix;

	XMMATRIX wvp = w * vp;

	// map data to cpu memory
	D3D11_MAPPED_SUBRESOURCE data = {};
	HRESULT hr = pD3DDeviceContext->Map(_pMatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &data);
	if (FAILED(hr)) return;

	// write data
	MatrixBuffer* pMatrixBuffer = static_cast<MatrixBuffer*>(data.pData);
	XMStoreFloat4x4(&(pMatrixBuffer->WVPMatrix), XMMatrixTranspose(wvp));
	XMStoreFloat4x4(&(pMatrixBuffer->worldMatrix), XMMatrixTranspose(w));

	// unmap data and write back to gpu memory
	pD3DDeviceContext->Unmap(_pMatrixBuffer, 0);
	pD3DDeviceContext->VSSetConstantBuffers(0, 1, &_pMatrixBuffer);

}

void Mesh::SetTransparencyBuffer(ID3D11DeviceContext* pD3DDeviceContext)
{
	// map data to cpu memory
	D3D11_MAPPED_SUBRESOURCE data = {};
	HRESULT hr = pD3DDeviceContext->Map(_pTransparencyBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &data);
	if (FAILED(hr)) return;

	// write data
	TransparencyData* pTransparencyData = static_cast<TransparencyData*>(data.pData);

	pTransparencyData->transparency = _transparency;
	// unmap data and write back to gpu memory
	pD3DDeviceContext->Unmap(_pTransparencyBuffer, 0);
	pD3DDeviceContext->PSSetConstantBuffers(1, 1, &_pTransparencyBuffer);
}

INT Mesh::createVertexBuffer(ID3D11Device* pD3DDevice)
{


	// define vertex buffer size
	_vertexCount = m_meshData->vertices->size();
	_vertexStride = sizeof(Vertex);

	// create vertex buffer
	D3D11_BUFFER_DESC desc = {};
	desc.ByteWidth = _vertexCount * _vertexStride; // byte length of vertex buffer
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER; // buffer type
	desc.Usage = D3D11_USAGE_IMMUTABLE; // buffer access

	D3D11_SUBRESOURCE_DATA initialData = {};
	initialData.pSysMem = m_meshData->vertices->data();

	HRESULT hr = pD3DDevice->CreateBuffer(&desc, &initialData, &_pVertexBuffer);
	CheckFailed(hr, 30);

	return 0;
}

INT Mesh::createIndexBuffer(ID3D11Device* pD3DDevice)
{

	_indexCount = m_meshData->indices->size();

	D3D11_BUFFER_DESC desc = {};
	desc.ByteWidth = _indexCount * sizeof(USHORT);
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	desc.Usage = D3D11_USAGE_IMMUTABLE;

	D3D11_SUBRESOURCE_DATA initialData = {};
	initialData.pSysMem = m_meshData->indices->data();

	HRESULT hr = pD3DDevice->CreateBuffer(&desc, &initialData, &_pIndexBuffer);
	CheckFailed(hr, 32);

	return 0;
}

INT Mesh::createVertexShader(ID3D11Device* pD3DDevice, wstring shader)
{
	wstring shaderPath = shader + L"VertexShader.cso";
	ID3DBlob* pCompiledShader = nullptr;
	HRESULT hr = {};

	hr = D3DReadFileToBlob(shaderPath.c_str(), &pCompiledShader);

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

INT Mesh::createPixelShader(ID3D11Device* pD3DDevice, wstring shader)
{
	wstring shaderPath = shader + L"PixelShader.cso";
	ID3DBlob* pCompiledShader = nullptr;
	HRESULT hr = D3DReadFileToBlob(shaderPath.c_str(), &pCompiledShader);
	CheckFailed(hr, 64);

	hr = pD3DDevice->CreatePixelShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), nullptr, &_pPixelShader);
	CheckFailed(hr, 66);

	safeRelease(pCompiledShader);

	return 0;
}

INT Mesh::createMatrixBuffer(ID3D11Device* pD3DDevice)
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

INT Mesh::createTransparentcyBuffer(ID3D11Device* pD3DDevice)
{
	D3D11_BUFFER_DESC desc = {};
	desc.ByteWidth = sizeof(TransparencyData);
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	HRESULT hr = pD3DDevice->CreateBuffer(&desc, nullptr, &_pTransparencyBuffer);
	CheckFailed(hr, 61);

	return 0;
}

INT Mesh::createTextureAndSamplerState(ID3D11Device* pD3DDevice)
{

	HRESULT hr;
	// texture
	if (m_meshData->texturePath != L"")
	{
		hr = CreateWICTextureFromFile(pD3DDevice, m_meshData->texturePath.c_str(), nullptr, &_pTexture);
		CheckFailed(hr, 63);
	}

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

INT Mesh::createInputLayout(ID3D11Device* pD3DDevice, ID3DBlob* pCompiledShaderCode)
{
	const int count = 3;
	D3D11_INPUT_ELEMENT_DESC elements[count] = {};

	// position
	elements[0].SemanticName = "POSITION";
	elements[0].SemanticIndex = 0;
	elements[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	elements[0].AlignedByteOffset = 0;
	// uv
	elements[1].SemanticName = "TEXCOORD";
	elements[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	elements[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;

	// normal
	elements[2].SemanticName = "NORMAL";
	elements[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	elements[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;


	HRESULT hr = pD3DDevice->CreateInputLayout(elements, count, pCompiledShaderCode->GetBufferPointer(), pCompiledShaderCode->GetBufferSize(), &_pInputLayout);
	CheckFailed(hr, 68);

	return 0;
}
