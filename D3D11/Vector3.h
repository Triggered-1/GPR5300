#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <string>

using namespace DirectX;
class Vector3
{
public:
	FLOAT x;
	FLOAT y;
	FLOAT z;


	Vector3() { x = 0.0f, y = 0.0f, z = 0.0f; }
	Vector3(FLOAT x, FLOAT y, FLOAT z) { this->x = x, this->y = y, this->z = z; }

	XMVECTOR toXMVector() { return XMVectorSet(x, y, z, 0.0f); }
	XMFLOAT3 toXMFloat() { return XMFLOAT3(x, y, z); }

	void Normalize();
	FLOAT Magnitude();
	Vector3 operator *(FLOAT s);
	void operator *=(FLOAT s);

	Vector3 operator +(Vector3 vector);
	void operator +=(Vector3 vector);
	
};

