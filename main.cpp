#include <time.h>
#include "utils/screen.h"
#include "shapes/shape.h"
#include "shapes/Mesh.h"
#include "utils/input.h"
#include "utils/camera.h"
#include <vector>
#include <chrono>


int main()
{
	Screen screen(235, 124);
	Rasterizer rasterizer(screen);
	Shape worldObj;
	Camera cam(screen);


	std::shared_ptr<Shape> cub = std::make_shared<Mesh>(rasterizer, "res/cub.obj");

	cub->scale({ 10,10,10 });

	worldObj.addChild(cub);

	auto tp1 = std::chrono::system_clock::now();
	auto tp2 = std::chrono::system_clock::now();


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
		
		cub->rotate(0.1, { 0.2,-1,0.6 });

		worldObj.drawChild(cam.getCameraViewMat());

		screen.screen_refresh();
		/////////////////////////////////////////////////////////////////////////////////////////
	}

	return 0;
}