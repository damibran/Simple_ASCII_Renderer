#pragma once
#include "screen.h"
#include <vector>
#include <memory>
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>

//== 2. Библиотека фигур ==
class shape
{ // Виртуальный базовый класс "фигура"

public:
	shape(shape&) = delete;
	shape() = default;
	void addChild(std::shared_ptr<shape>& s)
	{
		childs.push_back(s);
	}
	virtual void drawChild(const glm::mat4& parent_trans)
	{
		glm::mat4 thisTrans = parent_trans * position * rotation * scaling;
		for (int i = 0; i < childs.size(); ++i)
		{
			childs[i]->drawChild(thisTrans);
		}
	}
	void translate(const glm::vec3& v)
	{
		position = glm::translate(position, v);
	}
	void rotate(float angle,glm::vec3 v)
	{
		rotation = glm::rotate(rotation, glm::radians(angle), v);
	}
	virtual void scale(const glm::vec3& factor)
	{
		scaling = glm::scale(scaling, factor);
	}
protected:
	std::vector<std::shared_ptr<shape>> childs;
	glm::mat4 position = glm::mat4(1.0f);
	glm::mat4 rotation = glm::mat4(1.0f);
	glm::mat4 scaling = glm::mat4(1.0f);

};

