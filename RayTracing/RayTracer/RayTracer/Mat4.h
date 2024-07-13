#pragma once

#include <vector>
#include <cmath>
#include "Vec4.h"

#define SMALL_NUMBER    1.e-8

using namespace std;

class mat4
{
public:
	mat4();
	mat4(float[4][4]);
	vec4 translate(vec4, vec3);
	vec4 scale(vec4, vec3);


	float &val(int i, int j) {
		return m[i][j];
	}

	vec4 dot(vec4 v) {
		vector<float> res;
		for (int i = 0; i < 4; i++) {
			float val = 0;
			for (int j = 0; j < 4; j++) {
				val += m[i][j] * v[j];;
			}
			res.push_back(val);
		}
		return vec4(res);
	}

private:
	float m[4][4];
};

mat4::mat4()
{
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (i == j)
				this->m[i][j] = 1;
			else
				this->m[i][j] = 0;
		}
	}
}

mat4::mat4(float m[4][4])
{
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			this->m[i][j] = m[i][j];
		}
	}
}

vec4 mat4::translate(vec4 in, vec3 trans) {
	mat4 m;
	m.val(0, 3) = trans[0];
	m.val(1, 3) = trans[1];
	m.val(2, 3) = trans[2];
	return m.dot(in);
}

vec4 mat4::scale(vec4 in, vec3 scl) {
	mat4 m;
	m.val(0, 0) = scl[0];
	m.val(1, 1) = scl[1];
	m.val(2, 2) = scl[2];
	return m.dot(in);
}

inline vec4 dot(mat4 m, vec4 v) {
	vector<float> res;
	for (int i = 0; i < 4; i++) {
		float val = 0;
		for (int j = 0; j < 4; j++) {
			val += m.val(i, j) * v[j];
		}
		res.push_back(val);
	}
	return vec4(res);
}

