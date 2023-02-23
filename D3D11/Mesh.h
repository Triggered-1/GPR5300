#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <string>
#include "MeshData.h"
#include "GameObject.h"

using namespace DirectX;
using namespace std;

struct MatrixBuffer {
	XMFLOAT4X4 WVPMatrix;
	XMFLOAT4X4 worldMatrix;
};

struct TransparencyData 
{
	FLOAT transparency;
	XMFLOAT3  padding;
};

class Mesh : public GameObject
{
public:
	INT init(ID3D11Device* pD3DDevice, string path, wstring shader);
	void update();
	void render(ID3D11DeviceContext* pD3DDeviceContext, const XMMATRIX& pVMMatrix);
	void deInit();

	void setPosition(FLOAT x, FLOAT y, FLOAT z) { _position = { x, y, z }; }
	void setTransparency(FLOAT transparency);
	const XMFLOAT4X4& getWorldMatrix() const { return _worldMatrix; }

protected:
	XMFLOAT4X4 _worldMatrix = {}; // world transformation matrix

private:

	void SetMatrixBuffer(ID3D11DeviceContext* pD3DDeviceContext, const XMMATRIX& pVMMatrix);
	void SetTransparencyBuffer(ID3D11DeviceContext* pD3DDeviceContext);

	INT createVertexBuffer(ID3D11Device* pD3DDevice);
	INT createIndexBuffer(ID3D11Device* pD3DDevice);
	INT createVertexShader(ID3D11Device* pD3DDevice, wstring shader);
	INT createPixelShader(ID3D11Device* pD3DDevice, wstring shader);
	INT createMatrixBuffer(ID3D11Device* pD3DDevice);
	INT createTransparentcyBuffer(ID3D11Device* pD3DDevice);
	INT createTextureAndSamplerState(ID3D11Device* pD3DDevice);
	INT createInputLayout(ID3D11Device* pD3DDevice, ID3DBlob* pCompiledShaderCode);

	FLOAT _transparency = 1.0f;
	ID3D11VertexShader* _pVertexShader = nullptr;
	ID3D11PixelShader* _pPixelShader = nullptr;


	ID3D11Buffer* _pMatrixBuffer = nullptr;
	ID3D11Buffer* _pVertexBuffer = nullptr;
	ID3D11Buffer* _pIndexBuffer = nullptr;
	ID3D11Buffer* _pTransparencyBuffer = nullptr;

	ID3D11ShaderResourceView* _pTexture = nullptr;
	ID3D11SamplerState* _pSamplerState = nullptr;
	ID3D11InputLayout* _pInputLayout = nullptr;

	UINT _vertexCount = 0; // amount of vertices
	UINT _vertexStride = 0; // byte length of one vertex
	UINT _indexCount = 0; // amount of indices

	XMFLOAT3 _position = {}; // world position

	MeshData* m_meshData;
};

