#pragma once
#include "../shape.h"
#include "line.h"
#include "glm/glm.hpp"

class cube : public shape {
private:
	std::shared_ptr<shape> ul;
	std::shared_ptr<shape> ur;
	std::shared_ptr<shape> uf;
	std::shared_ptr<shape> ub;

	std::shared_ptr<shape> dl;
	std::shared_ptr<shape> dr;
	std::shared_ptr<shape> df;
	std::shared_ptr<shape> db;

	std::shared_ptr<shape> lb;
	std::shared_ptr<shape> lf;
	std::shared_ptr<shape> rb;
	std::shared_ptr<shape> rf;

public:
	cube(float w) :
		ul(std::make_shared<line>(w)),
		ur(std::make_shared<line>(w)),
		uf(std::make_shared<line>(w)),
		ub(std::make_shared<line>(w)),
		dl(std::make_shared<line>(w)),
		dr(std::make_shared<line>(w)),
		df(std::make_shared<line>(w)),
		db(std::make_shared<line>(w)),
		lb(std::make_shared<line>(w)),
		lf(std::make_shared<line>(w)),
		rb(std::make_shared<line>(w)),
		rf(std::make_shared<line>(w))
	{
		ul->translate({ -w / 2,w / 2,0 });
		ur->translate({ w / 2,w / 2,0 });
		uf->translate({ 0,w / 2,w / 2 });
		ub->translate({ 0,w / 2,-w / 2 });

		dl->translate({ -w / 2,-w / 2,0 });
		dr->translate({ w / 2,-w / 2,0 });
		df->translate({ 0,-w / 2,w / 2 });
		db->translate({ 0,-w / 2,-w / 2 });

		ul->rotate(90, { 0,1,0 });
		ur->rotate(90, { 0,1,0 });
		dl->rotate(90, { 0,1,0 });
		dr->rotate(90, { 0,1,0 });


		lb->rotate(90, { 0,0,1 });
		lf->rotate(90, { 0,0,1 });
		rb->rotate(90, { 0,0,1 });
		rf->rotate(90, { 0,0,1 });

		lb->translate({ -w / 2,0, -w / 2 });
		lf->translate({ -w / 2,0,w / 2   });
		rb->translate({ w / 2 ,0,-w / 2  });
		rf->translate({ w / 2 ,0,w / 2   });


		this->addChild(ul);
		this->addChild(ur);
		this->addChild(uf);
		this->addChild(ub);
		this->addChild(dl);
		this->addChild(dr);
		this->addChild(df);
		this->addChild(db);
		this->addChild(lb);
		this->addChild(lf);
		this->addChild(rb);
		this->addChild(rf);
	}

};