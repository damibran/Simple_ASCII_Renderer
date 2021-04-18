#include <time.h>
#include "screen.h"
#include "shape.h"
#include <vector>
#include "shapes/line.h"
#include "shapes/rectangle.h"
#include "shapes/ellipse.h"
#include "shapes/face.h"
#include "shapes/man.h"
#include "shapes/circle.h"
#include "shapes/cross.h"
#include "shapes/cube.h"
#include <chrono>
#include "input.h"
#include "camera.h"

int main()
{
	//== 1.���������� ������ ����� ==
	shape worldObj;
	std::shared_ptr<shape> x=std::make_shared<line>(500);
	std::shared_ptr<shape> y = std::make_shared<line>(500);
	y->rotate(90.0f, { 0,0,1 });
	std::shared_ptr<shape> cub = std::make_shared<cube>(20.0f);

	worldObj.addChild(x);
	worldObj.addChild(y);
	worldObj.addChild(cub);

	auto tp1 = std::chrono::system_clock::now();
	auto tp2 = std::chrono::system_clock::now();

	CameraAction action;

	camera cam;

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
		
		worldObj.drawChild(cam.getCameraViewMat());

		gScreen.screen_refresh();
		/////////////////////////////////////////////////////////////////////////////////////////
	}

	return 0;
}
