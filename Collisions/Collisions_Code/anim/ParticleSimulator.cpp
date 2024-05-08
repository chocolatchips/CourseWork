#include "ParticleSimulator.h"

ParticleSimulator::ParticleSimulator(const std::string& name, ParticleSystem* target) :
	BaseSimulator(name), m_object(target), num_springs(0),
	timeStep(0.0), gravity(0.0), drag(0.0), intType(euler),
	ground_kd(0.0), ground_ks(0.0)
{
}

std::vector<double*> newForces;

/// <summary>
/// Function integrates particle movement over 0.01 seconds (Simulation time step)
/// </summary>
/// <param name="time">Current simulation time</param>
/// <returns>TCL_OK on success</returns>
int ParticleSimulator::step(double time)
{
	// 0.01 matches simulation time step
	for (double t = 0.0; t < 0.01; t += timeStep) {
		newForces.clear();
		for (int i = 0; i < m_object->getParticles().size(); i++) {
			Vector force;
			zeroVector(force);
			double* res = new double[3];
			sumForces(i, res);
			newForces.push_back(res);
			
		}

		for (int i = 0; i < m_object->getParticles().size(); i++) {
			if (m_object->getParticle(i).isFixed())
				continue;
			switch (intType) {
			case euler:
				eulerIntegrate(i);
				break;
			case symplectic:
				symplecticIntegrate(i);
				break;
			case verlet:
				verletIntegrate(i);
				break;
			default:
				break;
			}
		}
		
	}
	return TCL_OK;
}

int ParticleSimulator::init(double time)
{
	return TCL_OK;
}


/// <summary>
/// Function supports the following commands:
/// <sim_name> link <sys name> <Number of Springs>
/// <sim_name> spring <index1> <index2> <ks> <kd> <restlength>
/// <sim_name> fix <index>
/// <sim_name> integration <euler|symplectic|verlet> <time step>
/// <sim_name> ground <ks> <kd> 
/// <sim_name> gravity <g>
/// <sim_name> drag <kdrag>
/// </summary>
/// <param name="argc"></param>
/// <param name="argv"></param>
/// <returns></returns>
int ParticleSimulator::command(int argc, myCONST_SPEC char** argv)
{
	if (argc < 1) {
		animTcl::OutputMessage("system %s: wrong number of params.", m_name.c_str());
		return TCL_ERROR;
	}
	else if(strcmp(argv[0], "link") == 0) {
		if (argc != 3) {
			animTcl::OutputMessage("Incorrect usage: link <sys name> <Number of Springs>");
			return TCL_ERROR;
		}
		link(argv[1], std::stoi(argv[2]));
	}

	else if(strcmp(argv[0], "spring") == 0) {
		if (argc != 6) {
			animTcl::OutputMessage("Incorrect usage: spring <index1> <index2> <ks> <kd> <restlength>");
			return TCL_ERROR;
		}
		createSpring(std::stoi(argv[1]), std::stoi(argv[2]), std::stod(argv[3]), std::stod(argv[4]), std::stod(argv[5]));
	}

	else if(strcmp(argv[0], "fix") == 0) {
		if (argc != 2) {
			animTcl::OutputMessage("Incorrect usage: fix <index>");
			return TCL_ERROR;
		}
		fixParticle(std::stoi(argv[1]));
	}

	else if(strcmp(argv[0], "integration") == 0) {
		if (argc != 3) {
			animTcl::OutputMessage("Incorrect usage: integration <euler|symplectic|verlet> <time step>");
			return TCL_ERROR;
		}
		return setIntegration(argv[1], std::stod(argv[2]));
	}
	else if(strcmp(argv[0], "ground") == 0) {
		if (argc != 3) {
			animTcl::OutputMessage("Incorrect usage: ground <ks> <kd> ");
			return TCL_ERROR;
		}
		setGround(std::stod(argv[1]), std::stod(argv[2]));
	}

	else if(strcmp(argv[0], "gravity") == 0) {
		if (argc != 2) {
			animTcl::OutputMessage("Incorrect usage: gravity <g>");
			return TCL_ERROR;
		}
		setGravity(std::stod(argv[1]));
	}

	else if(strcmp(argv[0], "drag") == 0) {
		if (argc != 2) {
			animTcl::OutputMessage("Incorrect usage: drag <kdrag>");
			return TCL_ERROR;
		}
		setDrag(std::stod(argv[1]));
	}
	return TCL_OK;
}


/// <summary>
/// Function links this simulator to ParticleSystem matching given name.
/// If system not found, one is created.
/// </summary>
/// <param name="system">Name of ParticleSystem</param>
/// <param name="numSprings">Number of springs</param>
void ParticleSimulator::link(std::string system, int numSprings) {
	ParticleSystem* partSys = dynamic_cast<ParticleSystem*>(GlobalResourceManager::use()->getSystem(system));
	if (partSys == NULL) {
		bool success;
		partSys = new ParticleSystem(system);
		success = GlobalResourceManager::use()->addSystem(partSys, true);
		assert(success);
	}
	m_object = partSys;
	this->num_springs = numSprings;
}


/// <summary>
/// Function finds length between two particles
/// </summary>
/// <param name="p1">Particle one</param>
/// <param name="p2">Particle two</param>
/// <returns></returns>
double findLength(Particle p1, Particle p2) {
	double* pos1 = p1.getPosition();
	double* pos2 = p2.getPosition();

	return sqrt(pow(pos1[0] - pos2[0], 2) + pow(pos1[1] - pos2[1], 2) + pow(pos1[2] - pos2[2], 2));
}


/// <summary>
/// Function creates a spring attached to particles at provided indexes
/// with assigned k_s, k_d, and rest length values
/// </summary>
/// <param name="index1">Particle one</param>
/// <param name="index2">Particle two</param>
/// <param name="ks">Spring k_s</param>
/// <param name="kd">Spring k_d</param>
/// <param name="restlength">Spring rest length</param>
void ParticleSimulator::createSpring(int index1, int index2, double ks, double kd, double restlength)
{
	Particle p_one = m_object->getParticle(index1);
	Particle p_two = m_object->getParticle(index2);
	if (restlength < 0)
		restlength = findLength(p_one, p_two);
	m_object->addSpring(Spring(index1, index2, ks, kd, restlength));
}


/// <summary>
/// Fixes particle movement to current location
/// </summary>
/// <param name="index">Index of particle</param>
void ParticleSimulator::fixParticle(int index)
{
	m_object->fixParticle(index);
}


/// <summary>
/// Sets integration method and timestep
/// </summary>
/// <param name="method">Integration method</param>
/// <param name="timeStep">Timestep</param>
/// <returns></returns>
int ParticleSimulator::setIntegration(std::string method, double timeStep)
{
	if (method == "euler")
		this->intType = euler;
	else if (method == "symplectic")
		this->intType = symplectic;
	else if (method == "verlet")
		this->intType = verlet;
	else {
		animTcl::OutputMessage("Incorrect usage: integration must be euler | symplectic | verlet");
		return TCL_ERROR;
	}

	this->timeStep = timeStep;

	return 0;
}


/// <summary>
/// Sets values of ground_ks and ground_kd
/// </summary>
/// <param name="ground_ks"></param>
/// <param name="ground_kd">k_s value of round</param>
void ParticleSimulator::setGround(double ground_ks, double ground_kd)
{
	this->ground_ks = ground_ks;
	this->ground_kd = ground_kd;
}


/// <summary>
/// Sets gravity
/// </summary>
/// <param name="gravity">Gravity</param>
void ParticleSimulator::setGravity(double gravity)
{
	this->gravity = gravity;
}


/// <summary>
/// Function sets drage force 
/// </summary>
/// <param name="drag"></param>
void ParticleSimulator::setDrag(double drag)
{
	this->drag = drag;
}


/// <summary>
/// Function calculates the forces acting on a particle
/// </summary>
/// <param name="index">Index of particle</param>
/// <param name="result">Vector of forces</param>
void ParticleSimulator::sumForces(int index, double* result)
{
	Particle part = m_object->getParticle(index);
	if (part.isFixed()) {
		for (int i = 0; i < 3; i++) {
			result[i] = 0.0;
		}
		return;
	}
		
	Vector forces;
	zeroVector(forces);

	for each (Spring spring in m_object->getSprings()) {
		if (spring.getPOne() == index) {
			calcSpringForce(spring, part, m_object->getParticle(spring.getPTwo()), forces);
			calcDamperForce(spring, part, m_object->getParticle(spring.getPTwo()), forces);
		}if (spring.getPTwo() == index) {
			calcSpringForce(spring, part, m_object->getParticle(spring.getPOne()), forces);
			calcDamperForce(spring, part, m_object->getParticle(spring.getPOne()), forces);
		}
	}

	calcDragForce(part, forces);
	calcNormalForce(part, forces);
	calcGravity(part.getMass(), forces);

	for (int i = 0; i < 3; i++) {
		result[i] = forces[i];
	}
	VecNumDiv(result, result, part.getMass());
}


/// <summary>
/// Function calculates the gravity force on a particle
/// </summary>
/// <param name="mass">Mass of particle</param>
/// <param name="forces">Current sum of forces</param>
void ParticleSimulator::calcGravity(double mass, Vector forces)
{
	double Fg = mass * gravity;
	forces[1] += Fg;
}


/// <summary>
/// Function calculates the drag force applied to particle
/// </summary>
/// <param name="part">Particle</param>
/// <param name="sum">Vector of force sum</param>
void ParticleSimulator::calcDragForce(Particle part, Vector sum)
{
	Vector force;
	VecNumMul(force, part.getVelocity(), -drag);
	VecAdd(sum, sum, force);
}


/// <summary>
/// Function calculates the normal force applied when colliding with ground
/// </summary>
/// <param name="part">Particle colliding with ground</param>
/// <param name="sum">Vector of force sum</param>
void ParticleSimulator::calcNormalForce(Particle part, Vector sum)
{
	// Sets normal of ground along y-axis
	Vector ground_normal; setVector(ground_normal, 0.0, 1.0, 0.0);
	// x dot N
	double dot = VecDotProd(part.getPosition(), ground_normal);

	// Check for collision
	if (dot > 0)
		return;

	Vector force;

	// (x dot N) * N
	VecNumMul(force, ground_normal, dot);
	// -k_s * (x dot N) * N
	VecNumMul(force, force, -ground_ks);

	Vector current;
	// v dot N
	dot = VecDotProd(part.getVelocity(), ground_normal);
	// (v dot N) * N
	VecNumMul(current, ground_normal, dot);
	// k_d * (v dot N) * N
	VecNumMul(current, current, ground_kd);
	// F_N = -k_s * (x dot N) * N - k_d * (v dot N) * N
	VecSubtract(force, force, current);

	VecAdd(sum, sum, force);
}


/// <summary>
/// Function calculates the spring force towards particle p1
/// </summary>
/// <param name="spring">Spring connecting the particles</param>
/// <param name="p1">Particle force directed to</param>
/// <param name="p2">Particle connected to spring</param>
/// <param name="sum">Sum of other forces</param>
void ParticleSimulator::calcSpringForce(Spring spring, Particle p1, Particle p2, Vector sum)
{
	Vector force;
	Vector pos1; p1.getPosition(pos1);
	Vector pos2; p2.getPosition(pos2);
	// X_i - X_j
	Vector norm; VecSubtract(norm, pos1, pos2);
	// |X_i - X_j|
	double distance = VecLength(norm);

	// (X_i - X_j) / (|X_i - X_j|)
	VecNormalize(norm);

	// K_s * (L_ij - |X_i - X_j|)
	distance = spring.getKs() * (spring.getRestLength() - distance);

	VecNumMul(norm, norm, distance);
	// K_s * (L_ij - |X_i - X_j|) * (X_i - X_j) / (|X_i - X_j|)
	VecCopy(force, norm);
	
	VecAdd(sum, sum, force);
}


/// <summary>
/// Function calculates the damper force for the provided particles and spring
/// </summary>
/// <param name="spring">Spring connecting the two particles</param>
/// <param name="p1">Particle one</param>
/// <param name="p2">Particle two</param>
/// <param name="sum">Sum of other forces</param>
void ParticleSimulator::calcDamperForce(Spring spring, Particle p1, Particle p2, Vector sum)
{
	Vector force;
	Vector pos1; p1.getPosition(pos1);
	Vector pos2; p2.getPosition(pos2);
	// (X_i - X_j) / (| X_i - X_j | )
	Vector norm; VecSubtract(norm, pos1, pos2); VecNormalize(norm);
	
	Vector vel1; p1.getVelocity(vel1);
	Vector vel2; p2.getVelocity(vel2);

	// V_i - V_j
	Vector subVel; VecSubtract(subVel, vel1, vel2);
	
	// (V_i - V_j) (dot) ((X_i - X_j) / (| X_i - X_j | ))
	double dot = VecDotProd(subVel, norm);

	// -K_d * (V_i - V_j) (dot) ((X_i - X_j) / (| X_i - X_j | ))
	dot *= -spring.getKd();

	// -K_d * (V_i - V_j) (dot) ((X_i - X_j) / (| X_i - X_j | )) * ((X_i - X_j) / (| X_i - X_j | ))
	VecNumMul(force, norm, dot);

	VecAdd(sum, sum, force);
}


/// <summary>
/// Function performs Euler integration for particle with corresponding acceleration for time step (h)
/// X_i(t + h) = X_i(t) + h * V_i(t)
/// V_i(t + h) = V_i(t) + h * a
/// </summary>
/// <param name="index">Index of particle and acceleration</param>
void ParticleSimulator::eulerIntegrate(int index)
{
	Particle part = m_object->getParticle(index);
	Vector velocity;
	Vector acc;
	Vector position;

	// Set V_i
	part.getVelocity(velocity);
	// Set X_i
	part.getPosition(position);

	// h * V_i(t)
	VecNumMul(velocity, velocity, timeStep);

	// X_i(t) + h * V_i(t)
	VecAdd(position, position, velocity);

	// X_i(t + h) = X_i(t) + h * V_i(t + h)
	m_object->updatePartPosition(index, position);
	
	// h * a
	VecNumMul(acc, newForces.at(index), timeStep);
	// Set V_i
	part.getVelocity(velocity);
	// V_i(t) + h * a
	VecAdd(velocity, acc, velocity);
	
	// V_i(t + h) = V_i(t) + h * a
	m_object->updatePartVelocity(index, velocity);
}


/// <summary>
/// Function performs symplectic integration for particle with corresponding acceleration for time step (h)
/// V_i(t + h) = V_i(t) + h * a
/// X_i(t + h) = X_i(t) + h * V_i(t + h)
/// </summary>
/// <param name="index">Index of particle and acceleration</param>
void ParticleSimulator::symplecticIntegrate(int index)
{
	Particle part = m_object->getParticle(index);
	Vector velocity;
	Vector acc;
	Vector position;

	// Set V_i
	part.getVelocity(velocity);
	// Set X_i
	part.getPosition(position);

	// h * a
	VecNumMul(acc, newForces.at(index), timeStep);

	// V_i(t) + h * a
	VecAdd(velocity, acc, velocity);
	
	// V_i(t + h) = V_i(t) + h * a
	m_object->updatePartVelocity(index, velocity);

	// h * V_i(t + h)
	VecNumMul(velocity, velocity, timeStep);

	// X_i(t) + h * V_i(t + h)
	VecAdd(position, position, velocity);
	
	// X_i(t + h) = X_i(t) + h * V_i(t + h)
	m_object->updatePartPosition(index, position);
}


/// <summary>
/// Function performs verlet integration for particle with corresponding acceleration for time step (h)
/// X(t - h) = X(t) - V(t-h) * h
/// X(t + h) = 2 * X(t) - X(t - h) + a * h * h
/// V(t) = (X(t + h) - X(t - h)) / (2 * h)
/// </summary>
/// <param name="index">Index of particle and acceleration</param>
void ParticleSimulator::verletIntegrate(int index)
{
	Particle part = m_object->getParticle(index);
	Vector acc; VecCopy(acc, newForces.at(index));
	Vector pos; part.getPosition(pos);
	Vector posP; part.getPosition(posP); 
	Vector vel; part.getVelocity(vel);
	
	// a * h * h
	VecNumMul(acc, acc, timeStep * timeStep);

	// V(t) * h
	VecNumMul(vel, vel, timeStep);
	// X(t - h) = X(t) - V(t) * h
	VecSubtract(posP, posP, vel);

	// 2 * X(t)
	VecNumMul(pos, pos, 2.0);

	//2 * X(t) - X(t - h)
	VecSubtract(pos, pos, posP);
	// 2 * X(t) - X(t - h) + a * h * h
	VecAdd(pos, pos, acc);

	// X(t + h) = 2 * X(t) - X(t - h) + a * h * h
	m_object->updatePartPosition(index, pos);

	//X(t + h) - X(t - h))
	VecSubtract(pos, pos, posP);

	//(X(t + h) - X(t - h)) / (2 * h)
	VecNumDiv(pos, pos, 2.0 * timeStep);

	//V(t) = (X(t + h) - X(t - h)) / (2 * h)
	m_object->updatePartVelocity(index, pos);
}
