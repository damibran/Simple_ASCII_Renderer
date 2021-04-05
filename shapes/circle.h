#pragma once
#include "../shape.h"
#include "glm/gtc/matrix_access.hpp"

class circle : public shape
{
public:
	circle(float _r) :r(_r){}

	virtual void drawChild(const glm::mat4& parent_trans)override
	{
		glm::mat4 thisTrans = parent_trans * (position * rotation * scaling);

		glm::vec4 zero = glm::vec4(0,0,1,1);

		float temp = sqrt(pow(glm::length(thisTrans[0]),2)+ pow(glm::length(thisTrans[1]), 2));

		put_circle((thisTrans * zero)[0] / (thisTrans * zero)[3] * gScreen.XMAX, (thisTrans * zero)[1] / (thisTrans * zero)[3] * gScreen.YMAX, r *temp*gScreen.XMAX/2 / (thisTrans * zero)[3]);

		for (int i = 0; i < childs.size(); ++i)
		{
			childs[i]->drawChild(thisTrans);
		}
	}
	virtual void scale(const glm::vec3& factor)
	{
		glm::mat4 trans = glm::scale(glm::mat4(1.0f), factor);
		scaling = trans * scaling;

		r *= glm::length(scaling[0]);
	}
private:
	float r;
	void put_circle(int xc, int yc, int r)
	{
		int x = 0, y = r;
		int d = 3 - 2 * r;
		drawCircle(xc, yc, x, y);
		while (y >= x)
		{
			// for each pixel we will
			// draw all eight pixels

			x++;

			// check for decision parameter
			// and correspondingly 
			// update d, x, y
			if (d > 0)
			{
				y--;
				d = d + 4 * (x - y) + 10;
			}
			else
				d = d + 4 * x + 6;
			drawCircle(xc, yc, x, y);
		}
	}
	void drawCircle(int xc, int yc, int x, int y)
	{
		gScreen.put_point(xc + x, yc + y);
		gScreen.put_point(xc - x, yc + y);
		gScreen.put_point(xc + x, yc - y);
		gScreen.put_point(xc - x, yc - y);
		gScreen.put_point(xc + y, yc + x);
		gScreen.put_point(xc - y, yc + x);
		gScreen.put_point(xc + y, yc - x);
		gScreen.put_point(xc - y, yc - x);
	}

};