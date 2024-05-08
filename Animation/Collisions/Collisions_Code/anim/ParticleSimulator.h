#pragma once
#include "BaseSimulator.h"
#include "GlobalResourceManager.h"
#include "util/Particle.h"
#include "util/Spring.h"
#include "ParticleSystem.h"
#include <vector>
#include "util/vector.h"

class ParticleSimulator : public BaseSimulator
{
public:
	ParticleSimulator(const std::string& name, ParticleSystem* target);
	int step(double time);
	int init(double time);
	int command(int argc, myCONST_SPEC char** argv);

protected:
	void link(std::string system, int numSprings);
	void createSpring(int index1, int index2, double ks, double kd, double restlength);
	void fixParticle(int index);
	int setIntegration(std::string method, double timeStep);
	void setGround(double ground_ks, double ground_kd);
	void setGravity(double gravity);
	void setDrag(double drag);
	void sumForces(int index, double* result);
	void calcGravity(double mass, Vector forces);
	void calcDragForce(Particle part, Vector sum);
	void calcNormalForce(Particle part, Vector sum);
	void calcSpringForce(Spring spring, Particle p1, Particle p2, Vector sum);
	void calcDamperForce(Spring spring, Particle p1, Particle p2, Vector sum);
	void eulerIntegrate(int index);
	void symplecticIntegrate(int index);
	void verletIntegrate(int index);



	ParticleSystem* m_object;
	enum Integration
	{
		euler, symplectic, verlet
	};
	
	int num_springs;
	double timeStep;
	double gravity;
	double drag;
	double ground_ks;
	double ground_kd;
	Integration intType;
	std::vector<Spring> collisionSprings;
};

