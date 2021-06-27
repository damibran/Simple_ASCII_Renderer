#pragma once
#include"../utils/screen.h"
#include"../utils/MVP_mat.h"
#include <glm/glm.hpp>

class Rasterizer
{
public:
	Rasterizer(Screen& s) : screen(s) {}
	void process_trngl(const MVP_mat& trans, const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2)
	{
		glm::vec4 a = glm::vec4(v0, 1.0f);
		glm::vec4 b = glm::vec4(v1, 1.0f);
		glm::vec4 c = glm::vec4(v2, 1.0f);

		glm::vec4 clip_a = trans.proj * trans.view * trans.model * a;
		glm::vec4 clip_b = trans.proj * trans.view * trans.model * b;
		glm::vec4 clip_c = trans.proj * trans.view * trans.model * c;

		glm::vec2 raster_a;
		glm::vec2 raster_b;
		glm::vec2 raster_c;

		raster_a.x = (clip_a)[0] / (clip_a)[3] * screen.XMAX + screen.XMAX / 2;
		raster_a.y = (clip_a)[1] / (clip_a)[3] * screen.YMAX + screen.YMAX / 2;

		raster_b.x = (clip_b)[0] / (clip_b)[3] * screen.XMAX + screen.XMAX / 2;
		raster_b.y = (clip_b)[1] / (clip_b)[3] * screen.YMAX + screen.YMAX / 2;

		raster_c.x = (clip_c)[0] / (clip_c)[3] * screen.XMAX + screen.XMAX / 2;
		raster_c.y = (clip_c)[1] / (clip_c)[3] * screen.YMAX + screen.YMAX / 2;

		if (clip_a.z <= clip_a.w && clip_a.z >= -clip_a.w &&
			clip_b.z <= clip_b.w && clip_b.z >= -clip_b.w &&
			clip_c.z <= clip_c.w && clip_c.z >= -clip_c.w)
			put_triangle(raster_a, raster_b, raster_c);
	}
private:
	void put_triangle(const glm::vec2& v0, const glm::vec2& v1, const glm::vec2& v2)
	{
		float xmin = min3(v0.x, v1.x, v2.x);
		float ymin = min3(v0.y, v1.y, v2.y);
		float xmax = max3(v0.x, v1.x, v2.x);
		float ymax = max3(v0.y, v1.y, v2.y);

		if (xmin < screen.XMAX - 1 && xmax > 0 && ymin < screen.YMAX - 1 && ymax > 0)
		{
			int x0 = std::max(0, (int)(std::floor(xmin)));
			int x1 = std::min(screen.XMAX - 1, (int)(std::floor(xmax)));
			int y0 = std::max(0, (int)(std::floor(ymin)));
			int y1 = std::min(screen.YMAX - 1, (int)(std::floor(ymax)));

			for (int y = y0; y <= y1; ++y)
			{
				for (int x = x0; x <= x1; ++x)
				{
					glm::vec2 pixel = { x + 0.5,y + 0.5 };
					if (edgeFunction(v0, v1, pixel) &&
						edgeFunction(v1, v2, pixel) &&
						edgeFunction(v2, v0, pixel))
					{
						screen.put_point(x, y);
					}
				}
			}
		}
	}
	float min3(const float& a, const float& b, const float& c)
	{
		return std::min(a, std::min(b, c));
	}
	float max3(const float& a, const float& b, const float& c)
	{
		return std::max(a, std::max(b, c));
	}
	float edgeFunction(const glm::vec2& a, const  glm::vec2& b, const  glm::vec2& c)
	{
		return ((c.x - a.x) * (b.y - a.y) - (c.y - a.y) * (b.x - a.x) >= 0);
	}
	Screen& screen;
};