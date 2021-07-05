#pragma once
#include"../utils/screen.h"
#include"../utils/MVP_mat.h"
#include"../utils/Vertex.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Rasterizer
{
	struct vrtx
	{
		glm::vec3 raster_pos;
		glm::vec3 view_pos;
		glm::vec3 view_norm;
	};
public:
	Rasterizer(Screen& s) : screen(s) {}
	void process_trngl(const MVP_mat& trans, const vertex& v0, const vertex& v1, const vertex& v2)
	{
		glm::vec4 clip_a = trans.proj * trans.view * trans.model * glm::vec4(v0.pos, 1.0f);
		glm::vec4 clip_b = trans.proj * trans.view * trans.model * glm::vec4(v1.pos, 1.0f);
		glm::vec4 clip_c = trans.proj * trans.view * trans.model * glm::vec4(v2.pos, 1.0f);

		vrtx a;
		vrtx b;
		vrtx c;

		//calculating raster positions
		a.raster_pos.x = (clip_a)[0] / (clip_a)[3] * screen.XMAX + screen.XMAX / 2;
		a.raster_pos.y = (clip_a)[1] / (clip_a)[3] * screen.YMAX + screen.YMAX / 2;
		a.raster_pos.z = clip_a.w;

		b.raster_pos.x = (clip_b)[0] / (clip_b)[3] * screen.XMAX + screen.XMAX / 2;
		b.raster_pos.y = (clip_b)[1] / (clip_b)[3] * screen.YMAX + screen.YMAX / 2;
		b.raster_pos.z = clip_b.w;

		c.raster_pos.x = (clip_c)[0] / (clip_c)[3] * screen.XMAX + screen.XMAX / 2;
		c.raster_pos.y = (clip_c)[1] / (clip_c)[3] * screen.YMAX + screen.YMAX / 2;
		c.raster_pos.z = clip_c.w;

		//calculating view positions
		a.view_pos = glm::vec3(trans.view * trans.model * glm::vec4(v0.pos, 1.0f));
		b.view_pos = glm::vec3(trans.view * trans.model * glm::vec4(v1.pos, 1.0f));
		c.view_pos = glm::vec3(trans.view * trans.model * glm::vec4(v2.pos, 1.0f));

		//calculation normals
		a.view_norm = glm::mat3(glm::transpose(glm::inverse(trans.view * trans.model))) * v0.norm;
		b.view_norm = glm::mat3(glm::transpose(glm::inverse(trans.view * trans.model))) * v1.norm;
		c.view_norm = glm::mat3(glm::transpose(glm::inverse(trans.view * trans.model))) * v2.norm;

		if (clip_a.z <= clip_a.w && clip_a.z >= -clip_a.w &&
			clip_b.z <= clip_b.w && clip_b.z >= -clip_b.w &&
			clip_c.z <= clip_c.w && clip_c.z >= -clip_c.w)
			put_triangle(a, b, c, glm::vec3(trans.view * glm::vec4(world_ligth_pos, 1.0f)));
	}
	glm::vec3 world_ligth_pos = glm::vec3(-60, 0, 0);
private:
	void put_triangle(const vrtx& v0, const vrtx& v1, const vrtx& v2, const glm::vec3& view_light_pos)
	{
		float xmin = min3(v0.raster_pos.x, v1.raster_pos.x, v2.raster_pos.x);
		float ymin = min3(v0.raster_pos.y, v1.raster_pos.y, v2.raster_pos.y);
		float xmax = max3(v0.raster_pos.x, v1.raster_pos.x, v2.raster_pos.x);
		float ymax = max3(v0.raster_pos.y, v1.raster_pos.y, v2.raster_pos.y);

		if (xmin < screen.XMAX - 1 && xmax > 0 && ymin < screen.YMAX - 1 && ymax > 0)
		{
			int x0 = std::max(0, (int)(std::floor(xmin)));
			int x1 = std::min(screen.XMAX - 1, (int)(std::floor(xmax)));
			int y0 = std::max(1, (int)(std::floor(ymin)));
			int y1 = std::min(screen.YMAX - 1, (int)(std::floor(ymax)));

			float area = edgeFunction(v0.raster_pos, v1.raster_pos, v2.raster_pos);
			for (int y = y0; y <= y1; ++y)
			{
				for (int x = x0; x <= x1; ++x)
				{
					glm::vec2 pixel = { x + 0.5,y + 0.5 };
					float w0 = edgeFunction(v1.raster_pos, v2.raster_pos, pixel);
					float w1 = edgeFunction(v2.raster_pos, v0.raster_pos, pixel);
					float w2 = edgeFunction(v0.raster_pos, v1.raster_pos, pixel);
					if (w0 >= 0 && w1 >= 0 && w2 >= 0)
					{
						w0 /= area;
						w1 /= area;
						w2 /= area;

						float corr = w0 / v0.raster_pos.z + w1 / v1.raster_pos.z + w2 / v2.raster_pos.z;

						w0 /= v0.raster_pos.z;
						w1 /= v1.raster_pos.z;
						w2 /= v2.raster_pos.z;
						
						w0 /= corr;
						w1 /= corr;
						w2 /= corr;

						glm::vec3 view_pixel_pos = w0 * v0.view_pos + w1 * v1.view_pos + w2 * v2.view_pos;
						glm::vec3 norm_pixel = glm::normalize(w0 * v0.view_norm + w1 * v1.view_norm + w2 * v2.view_norm);
						glm::vec3 lightDir = glm::normalize(view_light_pos - view_pixel_pos);

						glm::vec3 viewDir = normalize(-view_pixel_pos);
						glm::vec3 reflectDir = glm::normalize(glm::reflect(-lightDir, norm_pixel));

						float diff = glm::clamp(std::max(glm::dot(norm_pixel, lightDir), 0.0f), 0.0f, 1.0f);
						float spec = glm::clamp(std::pow(std::max(glm::dot(viewDir, reflectDir), 0.0f), 256), 0.0, 1.0);

						float color = ambient / 3 + diff * diffStrength / 3 + spec * specStrength / 3;

						screen.put_point(x, y, color);
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
	float edgeFunction(const glm::vec3& a, const  glm::vec3& b, const  glm::vec2& c)
	{
		return -((c.x - a.x) * (b.y - a.y) - (c.y - a.y) * (b.x - a.x));
	}
	float ambient = 0.5f;
	float diffStrength = 0.5f;
	float specStrength = 0.8f;
	Screen& screen;
};