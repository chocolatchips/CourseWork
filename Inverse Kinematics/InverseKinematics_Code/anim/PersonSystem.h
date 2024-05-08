#pragma once
#include "BaseSystem.h"
#include <Eigen/Dense>
#include "util/Jacobian.h"

class PersonSystem :public BaseSystem
{
public:
	PersonSystem(const std::string& name);
	~PersonSystem();
	void getState(double* p);
	void setState(double* p);
	void reset(double time);
	int command(int argc, myCONST_SPEC char** argv);
	void display(GLenum mode = GL_RENDER);
	int targetCommand(double x, double y, double z);
	bool targetPointSet();
	double* getTarget();

	EIGEN_MAKE_ALIGNED_OPERATOR_NEW

protected:
	Jacobian jac;
	
	double err = 0.0;
	double threshold = 0.05;
	bool onSpline = false;
	bool atTarget = false;
	double theta[7];

private:
	bool targetCommandSet = false;
	void drawCircle();
	void drawLeg(bool isLeft);
	void drawLeftArm();
	void drawRightArm();

	Eigen::Vector4d error;
	Eigen::Vector4d target;
	Eigen::Vector4d pInit;
	Eigen::Vector4d p_current;
	Eigen::Vector3d root_t;
	Eigen::Vector3d shoulder_t;
	Eigen::Vector3d elbow_t;
	Eigen::Vector3d wrist_t;
	Eigen::Vector3d hand_t;
};

