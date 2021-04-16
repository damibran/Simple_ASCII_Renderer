#pragma once
#include "../shape.h"
#include "rectangle.h"
#include "line.h"
#include "circle.h"

class face : public shape {
private:
	std::shared_ptr<shape>  rec;
	float bD;
	std::shared_ptr<shape>     l_eye;
	std::shared_ptr<shape>     r_eye;
	std::shared_ptr<shape>     nose;
	std::shared_ptr<shape>     mouth;

public:
	face(float w, float h) : 
		rec(std::make_shared <rectangle>(w, h)),
		bD(w > h ? h : w),
		l_eye(std::make_shared <circle>(bD / 8)),
		r_eye(std::make_shared <circle>(bD / 8)),
		nose(std::make_shared <circle>(bD / 16)),
		mouth(std::make_shared <line>(w * 2 / 3))
	{
		this->addChild(rec);
		this->addChild(l_eye);
		this->addChild(r_eye);
		this->addChild(nose);
		this->addChild(mouth);

		l_eye->translate({ -w / 4,h / 4,0 });
		r_eye->translate({ w / 4,h / 4 ,0 });

		mouth->translate({ 0, -h / 4 ,0});
	}

};