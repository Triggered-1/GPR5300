#pragma once
#include <Vector>
#include <string>
#include <d3d11.h>
#include <DirectXMath.h>
#include "vertex.h"

using namespace std;
using namespace DirectX;

class MeshData
{
public:
    INT init();
    INT deInit();

    UINT vertexCount = 0;
    UINT indexCount = 0;

    vector<Vertex>* vertices;
    vector<USHORT>* indices;
    vector<XMFLOAT3>* vertexPositions;
    vector<XMFLOAT3>* normals;
    vector<XMFLOAT2>* texCoords;

    vector<USHORT>* face_vertexindices;
    vector<USHORT>* face_normalindices;
    vector<USHORT>* face_uvIndices;

    wstring texturePath;

};

