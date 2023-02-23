#include "PointLight.h"

INT PointLight::init(PointLightData& lightData)
{
	PLData = lightData;
	return 0;
}

void PointLight::update()
{
	PLData.position = transform->position.toXMFloat();
}

void PointLight::deInit()
{
}
