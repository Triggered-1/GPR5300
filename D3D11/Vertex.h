#pragma once
#include <d3d11.h>
#include <DirectXMath.h>

using namespace DirectX;

struct Vertex
{
	XMFLOAT3 position;
	XMFLOAT2 uv;
	XMFLOAT3 normal;

	Vertex() : position(0.0f, 0.0f,0.0f), uv(0.0f, 0.0f), normal(0.0f, 0.0f, 0.0f) {}
	Vertex(FLOAT x, FLOAT y, FLOAT z) : position(x, y, z), uv(0.0f, 0.0f), normal(0.0f, 0.0f, 0.0f) {}
	Vertex(FLOAT x, FLOAT y, FLOAT z, BYTE r, BYTE g, BYTE b) : position(x, y, z), uv(0.0f, 0.0f), normal(0.0f, 0.0f, 0.0f) {}
	Vertex(FLOAT x, FLOAT y, FLOAT z, FLOAT u, FLOAT v) : position(x, y, z),  uv(u, v), normal(0.0f, 0.0f, 0.0f) {}
	Vertex(FLOAT x, FLOAT y, FLOAT z, FLOAT u, FLOAT v, FLOAT nx, FLOAT ny, FLOAT nz) : position(x, y, z), uv(u, v), normal(nx, ny, nz) {}
};
