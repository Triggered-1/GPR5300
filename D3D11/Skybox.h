#pragma once
#include "Mesh.h"

class Skybox : public Mesh
{
public:
	void updateSkybox();
	void setTrackingPos(Transform* camPos);

private:
	Transform* trackingPos;

};

