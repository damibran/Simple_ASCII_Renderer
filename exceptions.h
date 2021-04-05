#pragma once
#include<string>
#include"glm/glm.hpp"

struct CantCreateShape : std::exception
{
	CantCreateShape(const std::string& s) : std::exception(s.c_str()) { }
};