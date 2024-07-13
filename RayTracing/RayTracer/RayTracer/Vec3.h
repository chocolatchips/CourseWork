#pragma once

#include <cmath>
#include <iostream>
//#include "Vec4.h"

using namespace std;

class vec3
{
public:
	vec3();
	vec3(float, float, float);
	vec3(vector<float>);


	float x() { return v[0]; }
	float y() { return v[1]; }
	float z() { return v[2]; }

	float operator [](int i) { return v[i]; }
	vec3 operator-() { return vec3(-v[0], -v[1], -v[2]); }
	
	vec3 operator +=(vec3 u) {
		v[0] += u[0];
		v[1] += u[1];
		v[2] += u[2];
		return *this;
	};

	vec3 operator +=(float u) {
		v[0] += u;
		v[1] += u;
		v[2] += u;
		return *this;
	};

	vec3 operator *=(float u) {
		v[0] *= u;
		v[1] *= u;
		v[2] *= u;
		return *this;
	}

	vec3 operator -=(vec3 u) {
		v[0] -= u[0];
		v[1] -= u[1];
		v[2] -= u[2];
		return *this;
	}

	float mag() {
		return sqrt(mag_squared());
	}

	float mag_squared() {
		return v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
	}

	float dot(vec3 u) {
		return v[0] * u[0] + v[1] * u[1] + v[2] * u[2];
	}

	vec3 cross(vec3 u) {
		float i = v[1] * u[2] - v[2] * u[1];
		float j = v[0] * u[2] - v[2] * u[0];
		float k = v[0] * u[1] - v[1] * u[0];
		return vec3(i, j, k);
	}

	vec3 normalize() {
		float len = 1/mag();
		return vec3(v[0] * len, v[1] * len, v[2] * len);
	}

	void* begin() { return &v[0]; }
	void* end() { return &v[2]; }
private:
	float v[3];
};

vec3::vec3() : v{0,0,0}
{
}

vec3::vec3(float x, float y, float z) : v{ x,y,z }
{
}

vec3::vec3(vector<float> vec) : v{ vec[0], vec[1], vec[2] }{}

inline vec3 operator +(vec3 v, vec3 u) {
	return vec3(v[0] + u[0], v[1] + u[1], v[2] + u[2]);
}

inline vec3 operator -(vec3 v, vec3 u) {
	return vec3(v[0] - u[0], v[1] - u[1], v[2] - u[2]);
}

inline vec3 operator *(vec3 v, vec3 u) {
	return vec3(v[0] * u[0], v[1] * u[1], v[2] * u[2]);
}

inline vec3 operator *(vec3 v, float t) {
	return vec3(v[0] * t, v[1] * t, v[2] * t);
}

inline vec3 operator *(float t, vec3 v) {
	return v * t;
}
inline vec3 operator/(float t, vec3 v) {
	return vec3(t / v[0], t / v[1], t / v[2]);
}

inline vec3 operator /(vec3 v, float t) {
	return v * (1 / t);
}

inline float dot_prod(vec3 v, vec3 u) {
	return v[0] * u[0] + v[1] * u[1] + v[2] * u[2];
}

inline vec3 cross_prod(vec3 v, vec3 u) {
	float i = v[1] * u[2] - v[2] * u[1];
	float j = v[0] * u[2] - v[2] * u[0];
	float k = v[0] * u[1] - v[1] * u[0];
	return vec3(i, j, k);
}

inline vec3 normalize(vec3 v) {
	return v / v.mag();
}

