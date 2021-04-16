#pragma once
#include "../shape.h"
#include "rectangle.h"
#include "line.h"
#include "circle.h"

class hat : public shape {
private:
	std::shared_ptr<shape> rec;
	std::shared_ptr<shape> emblem;
	std::shared_ptr<shape> brim;

public:
	hat(float w, float h) : 
		rec(std::make_shared <rectangle>(w, h)), 
		emblem(std::make_shared <circle>((w > h ? h : w) / 2 - 1)), 
		brim(std::make_shared <line>(w + 12))
	{
		this->addChild(rec);
		this->addChild(emblem);
		this->addChild(brim);

		brim->translate({ 0, -h / 2 ,0});
	}
};