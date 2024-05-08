#pragma once
#include "BaseSimulator.h"
#include "PersonSystem.h"
#include "Hermite.h"
#include "GlobalResourceManager.h"


class IKSimulator : public BaseSimulator
{
public:
	IKSimulator(const std::string& name, PersonSystem* m_object);
	~IKSimulator();
	int step(double time);
	int init(double time);
	int command(int argc, myCONST_SPEC char** argv);
	void reset();


protected:
	PersonSystem* m_object;
	std::string hermiteName = "hermite";
	Hermite* hermite;
	double path_current = 0.0;
};

