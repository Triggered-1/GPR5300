#include "Light.h"
#include "Utilities.h"

INT Light::init(ID3D11Device* pD3DDevice)
{
	// create light buffer
	D3D11_BUFFER_DESC desc = {};
	desc.ByteWidth = sizeof(LightData);
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;



	HRESULT hr = pD3DDevice->CreateBuffer(&desc, nullptr, &_pLightBuffer);
	CheckFailed(hr, 70);
	ambientLight = { 0.3f,0.3f,0.3f,1.0f };
	return 0;
}

void Light::render(ID3D11DeviceContext* pD3DDeviceContext)
{
	D3D11_MAPPED_SUBRESOURCE data;
	HRESULT hr = pD3DDeviceContext->Map(_pLightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &data);
	LightData* lightBuffer = static_cast <LightData*> (data.pData);
	lightBuffer->AmbientLight = ambientLight;
	lightBuffer->DLData = _pDirectionalLight->GetData();
	lightBuffer->PLData = _pPointLight->GetData();
	pD3DDeviceContext->Unmap(_pLightBuffer, 0);
	pD3DDeviceContext->PSSetConstantBuffers(0, 1, &_pLightBuffer);
}

void Light::deInit()
{
	safeRelease(_pLightBuffer);
}

void Light::SetLight(DirectionalLight* pDirectionalLight)
{
	_pDirectionalLight = pDirectionalLight;
}

void Light::SetLight(PointLight* pPointLight)
{
	_pPointLight = pPointLight;
}
