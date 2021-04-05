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
#include <chrono>
#include "input.h"
#include "camera.h"
#include "exceptions.h"

int main()
{
	//== 1.���������� ������ ����� ==
	shape worldObj;
	std::shared_ptr<shape> x=std::make_shared<line>(500);
	std::shared_ptr<shape> y = std::make_shared<line>(500);
	y->rotate(glm::radians(90.0f));
	std::shared_ptr<shape> e = std::make_shared<man>(30, 40);
	std::shared_ptr<shape> e2 = std::make_shared<man>(30, 40);
	std::shared_ptr<shape> e3 = std::make_shared<man>(30, 40);
	
	e2->translate({ -60,0 });
	e3->translate({ 60,0 });
	
	worldObj.addChild(x);
	worldObj.addChild(y);
	worldObj.addChild(e);
	worldObj.addChild(e2);
	worldObj.addChild(e3);
	
	e->translate({ 0,80});
	e->scale({ 5,5,1 });

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
		e->rotate(2 * deltaTime);
		e2->rotate(2 * deltaTime);
		e3->rotate(2 * deltaTime);

		worldObj.drawChild(cam.getCameraViewMat());

		gScreen.screen_refresh();
		/////////////////////////////////////////////////////////////////////////////////////////
	}

	return 0;
}
