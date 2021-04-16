#pragma once
#include "../shape.h"
#include "rectangle.h"
#include "line.h"
#include "ellipse.h"
#include "hat.h"
#include "face.h"

class man : public shape {
private:
	std::shared_ptr<shape> f;
	std::shared_ptr<shape> cap;

public:
	man(float w, float h) : 
		f(std::make_shared<face>(w, h)), 
		cap(std::make_shared<hat>(w,h/2))
	{
		this->addChild(f);
		this->addChild(cap);

		f->translate({ 0, -h / 2 ,0});
		cap->translate({ 0, h / 4,0});
	}
};