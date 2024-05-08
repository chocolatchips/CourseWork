#include "IKSimulator.h"

IKSimulator::IKSimulator(const std::string& name, PersonSystem* m_object) :
	BaseSimulator(name), m_object(m_object)
{
}

IKSimulator::~IKSimulator()
{
}

/// <summary>
/// Function moves m_object end effector to target point
/// </summary>
/// <param name="time">Current time</param>
/// <returns>Step success</returns>
int IKSimulator::step(double time)
{
	double err = 0.0;
	double p_current[3] = { 0.0, 0.0, 0.0 };
	m_object->getState(p_current);
	double* target;

	if (m_object->targetPointSet()) {
		target = m_object->getTarget();
	}
	else {
		BaseSystem* sys = GlobalResourceManager::use()->getSystem(hermiteName);
		if (sys == nullptr)
			return TCL_OK;

		target = hermite->getIntermediatePoint(path_current).data();
	}
	
	err = sqrt(pow(target[0] - p_current[0], 2.0) + pow(target[1] - p_current[1], 2.0) + pow(target[2] - p_current[2], 2.0));

	if (!m_object->targetPointSet() && err < 0.1 && path_current <= 0.98 + 0.00001) {
		path_current += 0.005;
		target = hermite->getIntermediatePoint(path_current).data();
	}

	if (err > 0.1)
		m_object->setState(target);

	return TCL_OK;
}

int IKSimulator::init(double time)
{
	return TCL_OK;
}


/// <summary>
/// read spline.txt
/// </summary>
/// <param name="argc"></param>
/// <param name="argv"></param>
/// <returns></returns>
int IKSimulator::command(int argc, myCONST_SPEC char** argv)
{
	if (argc < 1) {
		animTcl::OutputMessage("system %s: wrong number of params.", m_name.c_str());
		return TCL_ERROR;
	}
	else if (strcmp(argv[0], "read") == 0) {
		if (argc != 2) {
			animTcl::OutputMessage("Incorrect usage: read <file_name>");
			return TCL_ERROR;
		}
		if (GlobalResourceManager::use()->getSystem(hermiteName) == NULL) {

			Hermite* herm = new Hermite(hermiteName);
			bool success = GlobalResourceManager::use()->addSystem(herm, true);
			assert(success);

			BaseSystem* sampleSystemRetrieval = GlobalResourceManager::use()->getSystem(hermiteName);
			assert(sampleSystemRetrieval);

			hermite = herm;
		}
		
		hermite->loadFromFile2D(argv[1]);
		m_object->reset(0.0);
	}
	return TCL_OK;
}

void IKSimulator::reset()
{
	path_current = 0.0;
}

