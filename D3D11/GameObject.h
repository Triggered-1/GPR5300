#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <string>
#include "Transform.h"

class GameObject
{
public:
	virtual void update();
	virtual void deInit();

	Transform* transform = new Transform;
};

