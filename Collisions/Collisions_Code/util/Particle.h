#pragma once
#include "GLModel/GLModel.h"
#include "util/vector.h"

class Particle
{
public:
	Particle();
	Particle(double x, double y, double z, double vx, double vy, double vz, double mass);
	bool isFixed();
	double* getPosition();
	void getPosition(Vector v);
	double* getVelocity();
	void getVelocity(Vector v);
	void setVelocity(double vx, double vy, double vz);
	void setVelocity(Vector velocity);
	void setPosition(double x, double y, double z);
	void setPosition(Vector position);
	void setMass(double mass);
	double getMass();
	void toggleFix();

protected:
	double x;
	double y;
	double z;
	double vx;
	double vy;
	double vz;
	double mass;
	bool fixed;
};

