#pragma once
#include "BaseSystem.h"
#include "util/Particle.h"
#include "util/Spring.h"
#include <vector>
#include "util/vector.h"

class ParticleSystem : public BaseSystem
{
public:
	ParticleSystem(const std::string& name);
	void display(GLenum mode = GL_RENDER);
	int command(int argc, myCONST_SPEC char** argv);
	void getState(double* p);
	void setState(double* p);
	void reset(double time);
	Particle getParticle(int index);
	std::vector<Particle> getParticles();
	std::vector<Spring> getSprings();
	Spring getSpring(int index);
	void fixParticle(int index);
	void updatePartVelocity(int index, Vector v);
	void updatePartPosition(int index, Vector x);
	void populateParticles(int num);
	void addSpring(Spring spring);
	int numParticles;

protected:
	void addParticles(int index, double mass, double x, double y, double z, double vx, double vy, double vz);
	void setAllVelocities(double vx, double vy, double vz);
	std::vector<Particle> particles;
	std::vector<Spring> springs;
};
