#include <time.h>
#include "utils/Screen.h"
#include "shapes/Shape.h"
#include "shapes/Mesh.h"
#include "utils/Input.h"
#include "utils/Camera.h"
#include "Shaders/ConcreteShaders/CubeShader.h"
#include <vector>
#include <chrono>


int main()
{
	Screen screen(235, 124);
	CubeShader cubShdr;
	Rasterizer rasterizer(screen,cubShdr);
	Mesh cubMesh("res/cub.obj");
	Shape worldObj;
	Camera cam(screen);

	std::shared_ptr<Shape> cub=std::shared_ptr<Shape>(new Shape(std::make_unique<MeshRenderer>(rasterizer,cubMesh)));
	std::shared_ptr<Shape> lightSource = std::make_shared<Shape>();

	cub->scale({ 10,10,10 });
	cub->rotate(90, { 1,0,0 });

	lightSource->translate({ 0,0,30 });

	worldObj.addChild(cub);
	worldObj.addChild(lightSource);

	auto tp1 = std::chrono::system_clock::now();
	auto tp2 = std::chrono::system_clock::now();

	float t = 0;
	while (1 && !Input::getExitAction())
	{
		/////////////////////////////////////////////////////////////////////////////////////////
		tp2 = std::chrono::system_clock::now();
		std::chrono::duration<float> elapsedTime = tp2 - tp1;
		tp1 = tp2;
		float deltaTime = elapsedTime.count();
		screen.deltaTime = deltaTime;
		screen.screen_clear();
		/////////////////////////////////////////////////////////////////////////////////////////

		Input::CameraAction action = Input::getCameraAction();
		cam.moveCamera(action, deltaTime);

		/////////////////////////////////////////////////////////////////////////////////////////
		t += elapsedTime.count() * 0.3;
		lightSource->setPos({30*cos(t),0,30*sin(t)});
		cubShdr.world_light_pos = lightSource->getPos();
		cub->rotate(0.1, { 0.2,-1,0.6 });

		worldObj.drawChild(cam.getCameraProjViewMat());

		screen.screen_refresh();
		/////////////////////////////////////////////////////////////////////////////////////////
	}

	return 0;
}
