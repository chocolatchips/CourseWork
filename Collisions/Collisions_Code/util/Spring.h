#pragma once
#include "Particle.h"
#include "GLModel/GLModel.h"

class Spring
{
public:
	Spring();
	Spring(int p_one, int p_two, double ks, double kd, double rest_length);
	double getKd() { return kd; }
	double getKs() { return ks; }
	int getPOne() { return p_one; }
	int getPTwo() { return p_two; }
	double getRestLength() { return rest_length; }

protected:
	double rest_length;
	double ks;
	double kd;
	int p_one;
	int p_two;

};

