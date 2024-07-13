#pragma once

#include "Vec4.h"

class ray
{
public:
	ray();
	ray(vec4, vec4);

	vec4 origin() { return org; }
	vec4 direct() { return dir; }

	vec4 pos(float t) { return org + t * dir; }

private:
	vec4 org;
	vec4 dir;
};

ray::ray() {

}

ray::ray(vec4 origin, vec4 direction) {
	org = origin;
	dir = direction;
	dir.mk_dir();
}
