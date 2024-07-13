#pragma once

#include <cmath>
#include <iostream>
#include <vector>
#include "Vec3.h"

using namespace std;

class vec4
{
public:
	vec4();
	vec4(float, float, float, float);
	vec4(vec3);
	vec4(vector<float>);

	vec3 to_vec3() { return vec3(v[0], v[1], v[2]);	}
	float x() { return v[0]; }
	float y() { return v[1]; }
	float z() { return v[2]; }
	float p() { return v[3]; }
	void mk_dir() { v[3] = 0.0; }

	float operator [](int i) { return v[i]; }
	vec4 operator-() { return vec4(-v[0], -v[1], -v[2], v[3]); }

	vec4 operator +=(vec4 u) {
		v[0] += u[0];
		v[1] += u[1];
		v[2] += u[2];
		return *this;
	};

	vec4 operator *=(float u) {
		v[0] *= u;
		v[1] *= u;
		v[2] *= u;
		return *this;
	}

	vec4 operator -=(vec4 u) {
		v[0] -= u[0];
		v[1] -= u[1];
		v[2] -= u[2];
		return *this;
	}

	float mag() {
		return sqrt(mag_squared());
	}

	float mag_squared() {
		return v[0] * v[0] + v[1] * v[1] + v[2] * v[2] + v[3] * v[3];
	}

	float dot(vec4 u) {
		return v[0] * u[0] + v[1] * u[1] + v[2] * u[2] + v[3] * u[3];
	}

	vec4 cross(vec4 u) {
		float i = v[1] * u[2] - v[2] * u[1];
		float j = v[0] * u[2] - v[2] * u[0];
		float k = v[0] * u[1] - v[1] * u[0];
		return vec4(i, j, k, v[3]);
	}

	vec4 normalize() {
		float len = 1 / mag();
		return vec4(v[0] * len, v[1] * len, v[2] * len, v[3] * len);
	}


public:
	float v[4];
};

vec4::vec4() : v{ 0.0f,0.0f,0.0f,0.0f }
{
}

vec4::vec4(float x, float y, float z, float p) : v{ x,y,z,p }
{
}

vec4::vec4(vec3 vec) : v{ vec[0], vec[1], vec[2], 1 }
{
}

vec4::vec4(vector<float> vec) {
	for (int i = 0; i < 4; i++) {
		if (vec.size() < i) {
			v[i] = 0.0f;
			continue;
		}
		v[i] = vec[i];
	}
}

inline vec4 operator +(vec4 v, vec4 u) {
	return vec4(v[0] + u[0], v[1] + u[1], v[2] + u[2], v[3]);
}

inline vec4 operator +(vec4 v, float u) {
	return vec4(v[0] + u, v[1] + u, v[2] + u, v[3]);
}

inline vec4 operator -(vec4 v, vec4 u) {
	return vec4(v[0] - u[0], v[1] - u[1], v[2] - u[2], v[3] - u[3]);
}

inline vec4 operator -(vec4 v, float u) {
	return vec4(v[0] - u, v[1] - u, v[2] - u, v[3]);
}

inline vec4 operator *(vec4 v, vec4 u) {
	return vec4(v[0] * u[0], v[1] * u[1], v[2] * u[2], v[3]);
}

inline vec4 operator *(vec4 v, float t) {
	return vec4(v[0] * t, v[1] * t, v[2] * t, v[3]);
}

inline vec4 operator *(float t, vec4 v) {
	return v * t;
}

inline vec4 operator /(vec4 v, float t) {
	return v * (1 / t);
}

inline float dot_prod(vec4 v, vec4 u) {
	return v[0] * u[0] + v[1] * u[1] + v[2] * u[2];
}

inline vec4 cross_prod(vec4 v, vec4 u) {
	float i = v[1] * u[2] - v[2] * u[1];
	float j = v[0] * u[2] - v[2] * u[0];
	float k = v[0] * u[1] - v[1] * u[0];
	return vec4(i, j, k, v[3]);
}

inline vec4 normalize(vec4 v) {
	float len = v.mag();
	vec4 res(v[0] / len, v[1] / len, v[2] / len, v[3] / len);
	return res;
}

