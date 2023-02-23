#pragma once
#include "MeshData.h"
#include <fstream>
#include <sstream>

class MeshLoader
{
public:
	static MeshLoader* getInstance();

	MeshData* loadFromFile(string filename);
	MeshLoader();
	~MeshLoader();
	MeshData* data;

	static MeshLoader* _pInstance;
	static USHORT currentIndex;

	void parseLine(string line);
	void parseLineFaces(string line);
	void parseVertices(string line);
	Vertex* parseFace(string cv);
	void parseFaces(string line);
	void parseNormals(string line);
	void parseTexture(string line);
	void parseTextureCoordinates(string line);
	void calculateTangentAndBitangent();

};