#include <Windows.h>
#include <random>
#include "Window.h"
#include "D3D.h"
#include "Mesh.h"
#include "Camera.h"
#include "Time.h"
#include "Material.h"
#include "Light.h"
#include "Utilities.h"
#include "Skybox.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int nCmdShow)
{
	UINT width = 800;
	UINT height = 600;
	BOOL isFullscreen = false;
	FLOAT red = 0.2f;
	FLOAT green = 0.2f;
	FLOAT blue = 0.2f;
	INT error = 0;

	// 1. create window
	Window window = {};
	error = window.init(hInstance, width, height);
	CheckError(error);

	// 2. create direct 3d connection
	D3D d3d = {};
	error = d3d.init(window.getWindowHandle(), width, height, isFullscreen);
	CheckError(error);

	// 3. create mesh
	Mesh mesh = {};
	error = mesh.init(d3d.getDevice(), "Obamium",L"Matte");
	CheckError(error);
	mesh.setTransparency(0.7f);
	// 3. create mesh
	Mesh mesh2 = {};
	error = mesh2.init(d3d.getDevice(), "sphere",L"Glossy");
	CheckError(error);
	mesh2.transform->position = Vector3(5.0f, 0.0f, 0.0f);
	//mesh2.transform.rotation = Vector3(0.0f,XM_PI,0.0f);


	// 4. create camera
	Camera camera = {};
	error = camera.init(d3d.getDevice(),width, height);
	CheckError(error);
	camera.transform->position = Vector3(0.0f, 0.0f, -10.0f);

	//Create Skybox
	Skybox skybox = {};
	error = skybox.init(d3d.getDevice(), "newskybox1", L"Texture");
	CheckError(error);
	skybox.setTrackingPos(camera.transform);

	// 5. create time
	Time time = {};
	error = time.init();
	CheckError(error);

	// 7. create light

	DirectionalLightData dirLightData = {};
	dirLightData.color = { 0.0f,0.0f,1.0f,1.0f };
	dirLightData.intensity = 1.0f;

	DirectionalLight* dirLight = new DirectionalLight;
	error = dirLight->init(dirLightData);
	CheckError(error);

	PointLightData pointLightData = {};
	pointLightData.color = { 1.0f,1.0f,1.0f,1.0f };
	pointLightData.intensity = 1.0f;
	pointLightData.constantAttenuation = 1.0f;
	pointLightData.linearAttenuation = 0.14f;
	pointLightData.quadraticAttenuation = 0.07f;
	pointLightData.maxDist = 15.0f;

	PointLight* pointLight = new PointLight;
	error = pointLight->init(pointLightData);
	CheckError(error);

	pointLight->transform->position = { 3.0f,0.0f,0.0f };

	Light light = {};
	error = light.init(d3d.getDevice());
	CheckError(error);
	light.SetLight(dirLight);
	light.SetLight(pointLight);
	FLOAT test = 0;
	// 8. run application
	while (window.run())
	{
		// 8.1. update objects
		time.update();
		test += time.getDeltaTime();
		// move Objects
		mesh.transform->rotation += Vector3(0.0f,time.getDeltaTime() * 3, 0.0f);
		mesh.transform->position = mesh.transform->right() * sinf(test) * 10;
		mesh.update();

		camera.update(d3d.getDeviceContext(), time.getDeltaTime());
		skybox.updateSkybox();
		pointLight->update();
		dirLight->update();
		// 8.2. render objects		
		d3d.beginScene(red, green, blue);

		// draw calls

		light.render(d3d.getDeviceContext());
		skybox.render(d3d.getDeviceContext(), camera.getVPMatrix());
		d3d.clearDepthStencil();

		mesh2.render(d3d.getDeviceContext(), camera.getVPMatrix());
		mesh.render(d3d.getDeviceContext(), camera.getVPMatrix());


		d3d.endScene();
	}

	// 9. tidy up
	light.deInit();
	time.deInit();
	camera.deInit();

	mesh.deInit();
	d3d.deInit();
	window.deInit();

	return 0;
}