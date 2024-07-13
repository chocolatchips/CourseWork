#pragma once
#include "Vec3.h"
#include <string>

class light
{
public:
	light();
	light(string, float, float, float, float, float, float);
	vec4 centre() { return pos; }
	vec3 colour() { return col; }
	string Name() { return name; }
private:
	string name;
	vec4 pos; vec3 col;
};

light::light()
{
	name = "";
	pos = { 0.0, 0.0, 0.0, 1.0 };
	col = { 0.0, 0.0, 0.0 };
}

inline light::light(string name, float xPos, float yPos, float zPos, float Ir, float Ig, float Ib)
{
	this->name = name;
	this->pos = { xPos, yPos, zPos, 1.0 };
	this->col = { Ir, Ig, Ib };
}
