#pragma once
#include "../shape.h"
#include "line.h"

class cross : public shape {
private:
	std::shared_ptr<shape> l;
	std::shared_ptr<shape> u;
public:
	cross(float w, float h) :
		l(std::make_shared < line>(h)), 
		u(std::make_shared < line>(w))
	{
		l->rotate(glm::radians(45.0f), { 0,0,1 });
		u->rotate(glm::radians(-45.0f), { 0,0,1 });


		this->addChild(l);
		this->addChild(u);
	}

};