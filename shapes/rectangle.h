#pragma once
#include "../shape.h"
#include "line.h"

class rectangle : public shape {
private:
	std::shared_ptr<shape> l;
	std::shared_ptr<shape> u;
	std::shared_ptr<shape> r;
	std::shared_ptr<shape> d;
public:
	rectangle(float w, float h) :
		l(std::make_shared<line>(h)),
		u(std::make_shared<line>(w)),
		r(std::make_shared<line>(h)),
		d(std::make_shared<line>(w))
	{
		l->rotate(glm::radians(90.0f));
		r->rotate(glm::radians(90.0f));

		l->translate({ -w / 2, 0 });
		r->translate({ w / 2, 0 });
		u->translate({ 0, h / 2 });
		d->translate({ 0, -h / 2 });


		this->addChild(l);
		this->addChild(u);
		this->addChild(r);
		this->addChild(d);
	}

};