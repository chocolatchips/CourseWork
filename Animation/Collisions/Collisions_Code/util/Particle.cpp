#include "Particle.h"

Particle::Particle() :
	x(0.0), y(0.0), z(0.0),
	vx(0.0), vy(0.0), vz(0.0),
	mass(0.0), fixed(false)
{
}

Particle::Particle(double x, double y, double z, double vx, double vy, double vz, double mass) :
	x(x), y(y), z(z),
	vx(vx), vy(vy), vz(vz),
	mass(mass), fixed(false)
{
}

bool Particle::isFixed()
{
	return fixed;
}

double* Particle::getPosition()
{
	double position[3] = { x,y,z };
	return position;
}

void Particle::getPosition(Vector v)
{
	setVector(v, x, y, z);
}

double* Particle::getVelocity()
{
	double velocity[3] = { vx,vy,vz };
	return velocity;
}

void Particle::getVelocity(Vector v)
{
	setVector(v, vx, vy, vz);
}

void Particle::setVelocity(double vx, double vy, double vz)
{
	this->vx = vx;
	this->vy = vy;
	this->vz = vz;
}

void Particle::setVelocity(Vector velocity)
{
	this->vx = velocity[0];
	this->vy = velocity[1];
	this->vz = velocity[2];
}

void Particle::setPosition(double x, double y, double z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

void Particle::setPosition(Vector position)
{
	this->x = position[0];
	this->y = position[1];
	this->z = position[2];
}

void Particle::setMass(double mass)
{
	this->mass = mass;
}

double Particle::getMass()
{
	return mass;
}

void Particle::toggleFix()
{
	fixed = !fixed;
}