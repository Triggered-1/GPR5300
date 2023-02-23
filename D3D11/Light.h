#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include "DirectionalLight.h"
#include "PointLight.h"

using namespace DirectX;

struct	LightData
{
	DirectionalLightData DLData;
	PointLightData PLData;
	XMFLOAT4 AmbientLight;
};

class Light
{
public:
	INT init(ID3D11Device* pD3DDevice);
	void render(ID3D11DeviceContext* pD3DDeviceContext);
	void deInit();

	void SetLight(DirectionalLight* pDirectionalLight);
	void SetLight(PointLight* pPointLight);

private:
	ID3D11Buffer* _pLightBuffer = nullptr; // light buffer object
	DirectionalLight* _pDirectionalLight;
	PointLight* _pPointLight;
	XMFLOAT4 ambientLight;
};

