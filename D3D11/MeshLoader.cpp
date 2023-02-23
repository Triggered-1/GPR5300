#pragma once
#include "MeshLoader.h"

using namespace std;

MeshLoader* MeshLoader::_pInstance = nullptr;
USHORT MeshLoader::currentIndex = 0;

MeshLoader* MeshLoader::getInstance()
{
	if (MeshLoader::_pInstance == 0)
	{
		MeshLoader::_pInstance = new MeshLoader();
	}
	return MeshLoader::_pInstance;
}

/*<summary>
Reads a .obj file and creates a MeshData based on that.
</summary>
<param name="filename"></param>
<returns></returns>*/

MeshData* MeshLoader::loadFromFile(string filename)
{
	string path = "Models/Objects/" + filename + ".obj";
	currentIndex = 0;
	data = new MeshData{};
	data->init();

	string line;
	ifstream firstRead(path);
	ifstream secondRead(path);

	while (getline(firstRead, line))
	{
		parseLine(line);
	}

	while (getline(secondRead, line))
	{
		parseLineFaces(line);
	}

	calculateTangentAndBitangent();

	return data;
}

MeshLoader::MeshLoader()
{
}

MeshLoader::~MeshLoader()
{
}

void MeshLoader::parseLine(string line)
{
	if (line.substr(0, 2) == "v ") parseVertices(line);
	else if (line.substr(0, 2) == "vn") parseNormals(line);
	else if (line.substr(0, 2) == "vt") parseTextureCoordinates(line);
	else if (line.substr(0, 6) == "mtllib") parseTexture(line);
}

void MeshLoader::parseLineFaces(string line)
{
	if (line.substr(0, 1) == "f")
	{
		parseFaces(line);
	}
}

void MeshLoader::parseFaces(string line)
{
	string prefix, vertex1, vertex2, vertex3;

	istringstream s(line);
	s >> prefix >> vertex1 >> vertex2 >> vertex3;

	parseFace(vertex1);
	parseFace(vertex2);
	parseFace(vertex3);
}

Vertex* MeshLoader::parseFace(string currentVertex)
{
	Vertex* vertex = new Vertex{};

	UINT firstSlash = (UINT)currentVertex.find_first_of("/");
	UINT secondSlash = (UINT)currentVertex.find_last_of("/");

	string vertexIndexString = currentVertex.substr(0, firstSlash);
	UINT vertexIndex = atoi(vertexIndexString.c_str()) - 1;
	data->face_vertexindices->push_back(vertexIndex);
	data->indexCount++;

	string uvIndexString = currentVertex.substr(firstSlash + 1, secondSlash - firstSlash + 1);
	UINT uvIndex = atoi(uvIndexString.c_str()) - 1;
	data->face_uvIndices->push_back(uvIndex);

	string normalIndexString = currentVertex.substr(secondSlash + 1);
	UINT normalIndex = atoi(normalIndexString.c_str()) - 1;
	data->face_normalindices->push_back(normalIndex);


	vertex->position = data->vertexPositions->at(vertexIndex);
	vertex->normal = data->normals->at(normalIndex);
	vertex->uv = data->texCoords->at(uvIndex);
	data->vertices->push_back(*vertex);
	data->indices->push_back(currentIndex++);

	return vertex;
}

void MeshLoader::parseVertices(string line)
{
	string prefix;
	float x, y, z;
	istringstream s(line);
	s >> prefix >> x >> y >> z;

	XMFLOAT3* v = new XMFLOAT3(x, y, z);
	data->vertexPositions->push_back(*v);
	data->vertexCount++;
}

void MeshLoader::parseNormals(string line)
{
	string prefix;
	float x, y, z;
	istringstream s(line);
	s >> prefix >> x >> y >> z;

	XMFLOAT3* normal = new XMFLOAT3(x, y, z);
	data->normals->push_back(*normal);
}

void MeshLoader::parseTextureCoordinates(string line)
{
	string prefix;
	float u, v;
	istringstream s(line);
	s >> prefix >> u >> v;

	XMFLOAT2* texCoord = new XMFLOAT2(u, 1.0f - v);
	data->texCoords->push_back(*texCoord);
}

void MeshLoader::parseTexture(string line)
{
	string prefix, materialName, textureName;
	istringstream s(line);

	s >> prefix >> materialName;

	string materialPath = "Models/Materials/" + materialName;
	string reading;

	ifstream materialStream(materialPath);

	while (getline(materialStream, reading))
	{
		if ((reading.substr(0, 6) == "map_Kd")) break;
	}

	istringstream ts(reading);
	ts >> prefix >> textureName;

	string texturePath;
	if (textureName == "") {
		texturePath = "Models/Textures/white.png";
	}
	else{
		texturePath = "Models/Textures/" + textureName;
	}
	data->texturePath = wstring(texturePath.begin(), texturePath.end());
}

void MeshLoader::calculateTangentAndBitangent()
{

}