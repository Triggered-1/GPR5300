#pragma once
#include "GameObject.h"

struct PointLightData 
{
	XMFLOAT3 position;
	FLOAT intensity;
	XMFLOAT4 color;
	FLOAT constantAttenuation;
	FLOAT linearAttenuation;
	FLOAT quadraticAttenuation;
	FLOAT maxDist;
};

class PointLight : public GameObject
{
public:
	INT init(PointLightData& lightData);
	void update();
	void deInit();
	PointLightData GetData() { return PLData; }

private:
	PointLightData PLData;
};

