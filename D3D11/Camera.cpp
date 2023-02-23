#include "Camera.h"
#include "Utilities.h"

INT Camera::init(ID3D11Device* pD3DDevice,UINT width, UINT height)
{
	INT error = 0;
	error = initCameraBuffer(pD3DDevice);
	CheckError(error);

	screenCenterX = width / 2;
	screenCenterY = height / 2;
	SetCursorPos(screenCenterX, screenCenterY);
	// projection matrix
	_projectionMatrix = XMMatrixPerspectiveFovLH(
		XM_PI * 0.3333333f, // field of view in radians, here 60 degrees
		static_cast<FLOAT>(width) / static_cast<FLOAT>(height), // aspect ratio
		0.3f, 1000.0f // near & far clipping planes
	);

	return 0;
}

void Camera::update(ID3D11DeviceContext* pD3DDeviceContext,FLOAT time)
{
	move(time);
	rotate(time);
	SetCameraBuffer(pD3DDeviceContext);
	XMVECTOR forward = transform->forward().toXMVector();
	XMVECTOR up = transform->up().toXMVector();

	_viewMatrix = XMMatrixLookToLH(transform->position.toXMVector(), forward, up);

	_VPMatrix = _viewMatrix * _projectionMatrix;
}

void Camera::deInit()
{
}

void Camera::move(FLOAT time)
{
	Vector3 movement;
	if (GetAsyncKeyState('W') & 0x8000) movement.z += 1.0f;
	if (GetAsyncKeyState('S') & 0x8000) movement.z += -1.0f;
	if (GetAsyncKeyState('D') & 0x8000) movement.x += 1.0f;
	if (GetAsyncKeyState('A') & 0x8000) movement.x += -1.0f;
	if (GetAsyncKeyState('E') & 0x8000) movement.y += 1.0f;
	if (GetAsyncKeyState('Q') & 0x8000) movement.y += -1.0f;

	movement.Normalize();
	movement *= time * movementSpeed;


	transform->position += transform->right() * movement.x;
	transform->position += transform->up() * movement.y;
	transform->position += transform->forward() * movement.z;
}

void Camera::rotate(FLOAT time)
{

	GetCursorPos(&cursorPos);
	Vector3 mouseDelta;
	mouseDelta.x = FLOAT(cursorPos.x - screenCenterX);
	mouseDelta.y = FLOAT(cursorPos.y - screenCenterY);
	mouseDelta.z = 0.0f;

	mouseDelta *= time * sensitivity;

	transform->rotation.y += mouseDelta.x;
	transform->rotation.x += mouseDelta.y;

	SetCursorPos(screenCenterX, screenCenterY);
}

void Camera::SetCameraBuffer(ID3D11DeviceContext* pD3DDeviceContext)
{
	// map data to cpu memory
	D3D11_MAPPED_SUBRESOURCE data = {};
	HRESULT hr = pD3DDeviceContext->Map(_pCameraBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &data);
	if (FAILED(hr)) return;

	// write data
	CameraData* pCameraBuffer = static_cast<CameraData*>(data.pData);
	
	pCameraBuffer->camPos = transform->position.toXMFloat();
	// unmap data and write back to gpu memory
	pD3DDeviceContext->Unmap(_pCameraBuffer, 0);
	pD3DDeviceContext->PSSetConstantBuffers(2, 1, &_pCameraBuffer);
}

INT Camera::initCameraBuffer(ID3D11Device* pD3DDevice)
{
	D3D11_BUFFER_DESC _desc = {};
	_desc.ByteWidth = sizeof(CameraData);
	_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	_desc.Usage = D3D11_USAGE_DYNAMIC;
	_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	HRESULT hr = pD3DDevice->CreateBuffer(&_desc,nullptr,&_pCameraBuffer);
	CheckFailed(hr, 12);
	return 0;
}
