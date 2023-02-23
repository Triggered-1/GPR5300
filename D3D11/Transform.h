#pragma once
#include "Vector3.h"

class Transform
{
public:

	Vector3 position;
	Vector3 rotation;
	Vector3 scale;

	Transform();
	

	XMMATRIX GetTransformationMatrix();

	Vector3 forward();
	Vector3 back();
	Vector3 right();
	Vector3 left();
	Vector3 up();
	Vector3 down();
};

