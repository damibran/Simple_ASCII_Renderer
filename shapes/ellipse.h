#pragma once
#include "../shape.h"

class ellipse : public shape
{
public:
	ellipse(float _a, float _b) :a(_a), b(_b) {}

	virtual void drawChild(const glm::mat4& parent_trans)override
	{
		glm::mat4 thisTrans = parent_trans * (position * rotation * scaling);

		put_ellipse(a, b, thisTrans);

		for (int i = 0; i < childs.size(); ++i)
		{
			childs[i]->drawChild(thisTrans);
		}
	}
	virtual void scale(const glm::vec3& factor)
	{
		glm::mat4 trans = glm::scale(glm::mat4(1.0f), factor);
		scaling = trans * scaling;

		a *= glm::length(scaling[0]);
		b *= glm::length(scaling[1]);
	}
private:
	float a, b;
	void put_ellipse(int rx, int ry, const glm::mat4& TR)
	{
		float dx, dy, d1, d2, x, y;
		glm::vec4 t;
		x = 0;
		y = ry;

		// Initial decision parameter of region 1 
		d1 = (ry * ry) - (rx * rx * ry) +
			(0.25 * rx * rx);
		dx = 2 * ry * ry * x;
		dy = 2 * rx * rx * y;

		// For region 1 
		while (dx < dy)
		{

			// Print points based on 4-way symmetry 
			t = glm::vec4(x, y, 1, 1);
			gScreen.put_point((TR * t).x / (TR * t)[3] * gScreen.XMAX, (TR * t).y / (TR * t)[3] * gScreen.YMAX);

			t = glm::vec4(-x, y, 1, 1);
			gScreen.put_point((TR * t).x / (TR * t)[3] * gScreen.XMAX, (TR * t).y / (TR * t)[3] * gScreen.YMAX);

			t = glm::vec4(x, -y, 1, 1);
			gScreen.put_point((TR * t).x / (TR * t)[3] * gScreen.XMAX, (TR * t).y / (TR * t)[3] * gScreen.YMAX);

			t = glm::vec4(-x, -y, 1, 1);
			gScreen.put_point((TR * t).x / (TR * t)[3] * gScreen.XMAX, (TR * t).y / (TR * t)[3] * gScreen.YMAX);

			// Checking and updating value of 
			// decision parameter based on algorithm 
			if (d1 < 0)
			{
				x++;
				dx = dx + (2 * ry * ry);
				d1 = d1 + dx + (ry * ry);
			}
			else
			{
				x++;
				y--;
				dx = dx + (2 * ry * ry);
				dy = dy - (2 * rx * rx);
				d1 = d1 + dx - dy + (ry * ry);
			}
		}

		// Decision parameter of region 2 
		d2 = ((ry * ry) * ((x + 0.5) * (x + 0.5))) +
			((rx * rx) * ((y - 1) * (y - 1))) -
			(rx * rx * ry * ry);

		// Plotting points of region 2 
		while (y >= 0)
		{

			// Print points based on 4-way symmetry 
			t = glm::vec4(x, y, 1, 1);
			gScreen.put_point((TR * t).x / (TR * t)[3] * gScreen.XMAX, (TR * t).y / (TR * t)[3] * gScreen.YMAX);

			t = glm::vec4(-x, y, 1, 1);
			gScreen.put_point((TR * t).x / (TR * t)[3] * gScreen.XMAX, (TR * t).y / (TR * t)[3] * gScreen.YMAX);

			t = glm::vec4(x, -y, 1, 1);
			gScreen.put_point((TR * t).x / (TR * t)[3] * gScreen.XMAX, (TR * t).y / (TR * t)[3] * gScreen.YMAX);

			t = glm::vec4(-x, -y, 1, 1);
			gScreen.put_point((TR * t).x / (TR * t)[3] * gScreen.XMAX, (TR * t).y / (TR * t)[3] * gScreen.YMAX);

			// Checking and updating parameter 
			// value based on algorithm 
			if (d2 > 0)
			{
				y--;
				dy = dy - (2 * rx * rx);
				d2 = d2 + (rx * rx) - dy;
			}
			else
			{
				y--;
				x++;
				dx = dx + (2 * ry * ry);
				dy = dy - (2 * rx * rx);
				d2 = d2 + dx - dy + (rx * rx);
			}
		}
	}

};