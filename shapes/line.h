#pragma once
#include "../shape.h"

class line : public shape {
public:
	line(float d) : a(glm::vec4(-d / 2, 0, 1, 1)), b(glm::vec4(d / 2, 0, 1, 1)){}
	virtual void drawChild(const glm::mat4& parent_trans)override
	{
		glm::mat4 thisTrans = parent_trans * position * rotation * scaling;
		int x0 = (thisTrans * a)[0]/(thisTrans * a)[3] * gScreen.XMAX+ gScreen.XMAX/2;
		int y0 = (thisTrans * a)[1] / (thisTrans * a)[3]*gScreen.YMAX + gScreen.YMAX / 2;

		int x1 = (thisTrans * b)[0] / (thisTrans * b)[3] * gScreen.XMAX + gScreen.XMAX / 2;
		int y1 = (thisTrans * b)[1] / (thisTrans * b)[3] * gScreen.YMAX + gScreen.YMAX / 2;

		put_line(x0, y0, x1, y1);
		for (int i = 0; i < childs.size(); ++i)
		{
			childs[i]->drawChild(thisTrans);
		}
	}
private:
	glm::vec4 a, b;
	void put_line(int x0, int y0, int x1, int y1)
		/* Алгоритм Брезенхэма для прямой:
		рисование отрезка прямой от (x0,y0) до (x1,y1).
		Уравнение прямой: b(x-x0) + a(y-y0) = 0.
		Минимизируется величина abs(eps), где eps = 2*(b(x-x0)) + a(y-y0).  */
	{
		int dx = 1;
		int a = x1 - x0;   if (a < 0) dx = -1, a = -a;
		int dy = 1;
		int b = y1 - y0;   if (b < 0) dy = -1, b = -b;
		int two_a = 2 * a;
		int two_b = 2 * b;
		int xcrit = -b + two_a;
		int eps = 0;
		for (;;) { //Формирование прямой линии по точкам
			gScreen.put_point(x0, y0);
			if (x0 == x1 && y0 == y1) break;
			if (eps <= xcrit) x0 += dx, eps += two_b;
			if (eps >= a || a < b) y0 += dy, eps -= two_a;
		}
	}

};