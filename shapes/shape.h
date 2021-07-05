#pragma once
#include"../utils/MVP_mat.h"
#include "../utils/screen.h"
#include <vector>
#include <memory>
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>

//== 2. Библиотека фигур ==
class Shape
{ // Виртуальный базовый класс "фигура"

public:
	Shape(Shape&) = delete;
	Shape() = default;
	void addChild(std::shared_ptr<Shape>& s)
	{
		childs.push_back(s);
	}
	virtual void drawChild(const MVP_mat& parent_trans)
	{
		MVP_mat thisTrans(parent_trans);
		thisTrans.model = parent_trans.model * position * rotation * scaling;

		for (int i = 0; i < childs.size(); ++i)
		{
			childs[i]->drawChild(thisTrans);
		}
	}
	void translate(const glm::vec3& v)
	{
		position = glm::translate(position, v);
	}
	void setPos(const glm::vec3& v)
	{
		position= glm::translate(glm::mat4(1), v);
	}
	void rotate(float angle,glm::vec3 v)
	{
		rotation = glm::rotate(rotation, glm::radians(angle), v);
	}
	void scale(const glm::vec3& factor)
	{
		scaling = glm::scale(scaling, factor);
	}
	glm::vec3 getPos() 
	{
		return position * glm::vec4(1);
	}
protected:
	std::vector<std::shared_ptr<Shape>> childs;
	glm::mat4 position = glm::mat4(1.0f);
	glm::mat4 rotation = glm::mat4(1.0f);
	glm::mat4 scaling = glm::mat4(1.0f);

};

