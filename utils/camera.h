#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "input.h"
#include "screen.h"

class Camera
{
public:
	void moveCamera(CameraAction act,float dt)
	{
		float tspeed = dt * speed;
		if (act == CameraAction::UP)
			cameraPos += tspeed * cameraUp;
		else if(act == CameraAction::DOWN)
			cameraPos -= tspeed * cameraUp;
		else if (act == CameraAction::LEFT)
			cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * tspeed;
		else if (act == CameraAction::RIGHT)
			cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * tspeed;
		else if (act == CameraAction::ZOOMOUT)
			cameraPos -= tspeed * cameraFront;
		else if (act == CameraAction::ZOOMIN)
			cameraPos += tspeed * cameraFront;
	}
	glm::mat4 getCameraViewMat()
	{
		glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		return proj* view;
	}
private:
	glm::mat4 proj = glm::perspective(glm::radians(90.0f), (float)gScreen.XMAX / (float)gScreen.YMAX, 0.1f, 200.0f);
	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 30.0f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	float speed=30;
};