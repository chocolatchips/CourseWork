#pragma once

#include "Vec4.h"


class sphere
{
public:
	sphere();
    sphere(string, float, float, float,
        float, float, float, float,
        float, float, float, float,
        float, float, int);
    vec4 inverse(vec4);
    vec4 transform(vec4);
    vec4 translate(vec4);
    vec4 translate_inv(vec4);
    vec4 scale(vec4);
    vec4 scale_inv(vec4);
    vec4 normal(vec4);
    string Name() { return name; }
    vec4 centre() { return vec4(pos[0], pos[1], pos[2], 1.0); }
    vec3 colour() { return this->col; }
    float ka() { return Ka; }
    float kd() { return Kd; }
    float ks() { return Ks; }
    float kr() { return Kr; }
    int N() { return n; }
private:
    string name;
    vec3 pos;
    vec3 scl;
    vec3 col;
    float Ka; float Kd; float Ks; float Kr;
    int n;
};

sphere::sphere()
{
    name = "";
    pos = vec3(); scl = vec3(); col = vec3();
    Ka = 0; Kd = 0; Ks = 0; Kr = 0;
    n = 0;
}

sphere::sphere(string name, float xPos, float yPos, float zPos,
    float xScl, float yScl, float zScl, float r, float g, float b, float ka, float kd, float ks, float kr, int n) {
    this->name = name;
    this->pos = vec3(xPos, yPos, zPos);
    this->scl = vec3(xScl, yScl, zScl);
    this->col = vec3(r, g, b);
    this->Ka = ka;
    this->Kd = kd;
    this->Ks = ks;
    this->Kr = kr;
    this->n = n;
}

vec4 sphere::inverse(vec4 v) {
    vec4 res(scale_inv(translate_inv(v)));
    return res;
}

vec4 sphere::transform(vec4 v) {
    return scale(translate(v));
}

vec4 sphere::translate(vec4 v)
{
    vec4 res(v.x() + v.p() * pos.x(), v.y() + v.p() * pos.y(), v.z() + v.p() * pos.z(), v.p());
    return res;
}

vec4 sphere::translate_inv(vec4 v)
{
    vec4 res(v.x() + v.p() * -pos.x(), v.y() + v.p() * -pos.y(), v.z() + v.p() * -pos.z(), v.p());
    return res;
}

vec4 sphere::scale(vec4 v)
{
    vec4 res(v.x() * scl.x(), v.y() * scl.y(), v.z() * scl.z(), v.p());
    return res;
}

vec4 sphere::scale_inv(vec4 v)
{
    vec4 res(v.x() / scl.x(), v.y() / scl.y(), v.z() / scl.z(), v.p());
    return res;
}

vec4 sphere::normal(vec4 P) {
    vec4 v (P - pos);
    vec4 res(v.x() / (scl.x() * scl.x()), v.y() / (scl.y() * scl.y()), v.z() / (scl.z() * scl.z()), v.p());
    return res;
}
