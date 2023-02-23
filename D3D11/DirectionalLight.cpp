#include "DirectionalLight.h"

INT DirectionalLight::init(DirectionalLightData& lightData)
{
	DLData = lightData;
	return 0;
}

void DirectionalLight::update()
{
	DLData.direction = transform->forward().toXMFloat();
}

void DirectionalLight::deInit()
{

}
