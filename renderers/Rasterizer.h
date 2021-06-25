#pragma once
#include "../utils/screen.h"
#include <glm/glm.hpp>

class Rasterizer
{
public:
	Rasterizer(Screen& s) : screen(s) {}
	void put_line(glm::mat4& trans, glm::vec3& v1, glm::vec3& v2)
	{
		glm::vec4 a = glm::vec4(v1, 1.0f);
		glm::vec4 b = glm::vec4(v2, 1.0f);

		glm::vec4 ndc_pos_a = trans * a;
		glm::vec4 ndc_pos_b = trans * b;

		int x0 = (ndc_pos_a)[0] / (ndc_pos_a)[3] * screen.XMAX + screen.XMAX / 2;
		int y0 = (ndc_pos_a)[1] / (ndc_pos_a)[3] * screen.YMAX + screen.YMAX / 2;

		int x1 = (ndc_pos_b)[0] / (ndc_pos_b)[3] * screen.XMAX + screen.XMAX / 2;
		int y1 = (ndc_pos_b)[1] / (ndc_pos_b)[3] * screen.YMAX + screen.YMAX / 2;

		if (ndc_pos_a.z <= ndc_pos_a.w && ndc_pos_a.z >= -ndc_pos_a.w && ndc_pos_b.z <= ndc_pos_b.w && ndc_pos_b.z >= -ndc_pos_b.w)
			screen.put_line(x0, y0, x1, y1);
	}
private:
	Screen& screen;
};