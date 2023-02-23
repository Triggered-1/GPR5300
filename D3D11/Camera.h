#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include "GameObject.h"

using namespace DirectX;
struct CameraData
{
	XMFLOAT3 camPos;
	FLOAT padding;
};
class Camera : public GameObject
{
public:
	INT init(ID3D11Device* pD3DDevice, UINT width, UINT height);
	void update(ID3D11DeviceContext* pD3DDeviceContext, FLOAT time);
	void deInit();
	void move(FLOAT time);
	void rotate(FLOAT time);


	const XMMATRIX& getViewMatrix() const { return _viewMatrix; }
	const XMMATRIX& getProjectionMatrix() const { return _projectionMatrix; }
	const XMMATRIX& getVPMatrix() const { return _VPMatrix; }

private:

	XMMATRIX _VPMatrix; // view projection Matrix
	XMMATRIX _viewMatrix; // view transformation matrix
	XMMATRIX _projectionMatrix; // projection matrix

	POINT cursorPos;
	LONG screenCenterX;
	LONG screenCenterY;

	const FLOAT movementSpeed = 5.0f;
	const FLOAT sensitivity = 2.0f;

	ID3D11Buffer* _pCameraBuffer = nullptr;
	INT initCameraBuffer(ID3D11Device* pD3DDevice);
	void SetCameraBuffer(ID3D11DeviceContext* pD3DDeviceContext);
};

