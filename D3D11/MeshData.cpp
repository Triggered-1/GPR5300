#include "MeshData.h"
using namespace std;

INT MeshData::init()
{
	vertices = new vector<Vertex>;
	indices = new vector<USHORT>;
	vertexPositions = new vector<XMFLOAT3>;
	normals = new vector<XMFLOAT3>;
	texCoords = new vector<XMFLOAT2>;
	face_vertexindices = new vector<USHORT>;
	face_normalindices = new vector<USHORT>;
	face_uvIndices = new vector<USHORT>;
	return 0;
}

INT MeshData::deInit()
{
	//TO Do Clear Lists xDDDDD
	return 0;
}