#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "input.h"
#include "screen.h"

class camera
{
public:
	void moveCamera(CameraAction act,float dt)
	{
		float tspeed = dt * speed;
		if (act == CameraAction::UP)
			position = glm::translate(position, { 0,-tspeed , 0});
		else if(act == CameraAction::DOWN)
			position = glm::translate(position, { 0,tspeed , 0 });
		else if (act == CameraAction::LEFT)
			position = glm::translate(position, { tspeed,0 , 0 });
		else if (act == CameraAction::RIGHT)
			position = glm::translate(position, { -tspeed,0 , 0 });
		else if (act == CameraAction::ZOOMOUT)
		{
			position = glm::translate(position, { 0 ,0,-tspeed });
		}
		else if (act == CameraAction::ZOOMIN)
		{
			position = glm::translate(position, { 0 ,0,tspeed });
		}
	}
	glm::mat4 getCameraViewMat()
	{
		return proj*position;
	}
private:
	glm::mat4 proj = glm::perspective(glm::radians(90.0f), (float)gScreen.XMAX / (float)gScreen.YMAX, 0.1f, 100.0f);
	float speed=30;
	glm::mat4 position = glm::translate(glm::mat4(1.0f), { 0,0, -30 });
};