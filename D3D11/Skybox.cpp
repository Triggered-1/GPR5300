#include "Skybox.h"

void Skybox::updateSkybox()
{
	transform->position = trackingPos->position;
	//XMStoreFloat4x4(&_worldMatrix, trackingPos->GetTransformationMatrix());
}

void Skybox::setTrackingPos(Transform* camPos)
{
	trackingPos = camPos;
}
