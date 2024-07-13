#pragma once
#include <vector>
#include <string>
#include "Vec3.h"
#include "Ray.h"

using namespace std;

struct Plane
{
    int near;
    int left; int right; int top; int bottom;
};

struct Res
{
    int x; int y;
};

struct Ambient
{
    float Ir; float Ig; float Ib;
};

struct Back
{
    float r; float g; float b;
    vec3 colour() { return vec3(r, g, b); }
};

