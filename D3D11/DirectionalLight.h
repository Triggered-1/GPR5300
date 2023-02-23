#pragma once
#include "GameObject.h"
struct DirectionalLightData 
{
	XMFLOAT3 direction;
	FLOAT intensity;
	XMFLOAT4 color;
};

class DirectionalLight: public GameObject
{
public:
	INT init(DirectionalLightData& lightData);
	void update();
	void deInit();
	DirectionalLightData GetData() { return DLData; }

private:
	DirectionalLightData DLData;
};

