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
	//== 1.���������� ������ ����� ==
	Shape worldObj;

	std::shared_ptr<Shape> cub = std::make_shared<Mesh>("res/cub.obj");

	cub->scale({ 10,10,10 });

	worldObj.addChild(cub);

	auto tp1 = std::chrono::system_clock::now();
	auto tp2 = std::chrono::system_clock::now();

	CameraAction action;

	Camera cam;

	while (1 && !Input::getExitAction())
	{
		/////////////////////////////////////////////////////////////////////////////////////////
		tp2 = std::chrono::system_clock::now();
		std::chrono::duration<float> elapsedTime = tp2 - tp1;
		tp1 = tp2;
		float deltaTime = elapsedTime.count();
		gScreen.deltaTime = deltaTime;
		gScreen.screen_clear();
		/////////////////////////////////////////////////////////////////////////////////////////

		action = Input::getCameraAction();
		cam.moveCamera(action, deltaTime);

		/////////////////////////////////////////////////////////////////////////////////////////
		
		cub->rotate(0.1, { 0.2,-1,0.6 });

		worldObj.drawChild(cam.getCameraViewMat());

		gScreen.screen_refresh();
		/////////////////////////////////////////////////////////////////////////////////////////
	}

	return 0;
}
