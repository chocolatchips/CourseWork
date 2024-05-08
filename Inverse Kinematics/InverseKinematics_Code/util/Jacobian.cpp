#include "Jacobian.h"

Jacobian::Jacobian()
{
	jac.setZero();
	origin << 0.0, 0.0, 0.0, 1.0;
	root_t << -0.5, -2.0, 4.0;
	shoulder_t << 0.5, 2.0, 0.0;
	elbow_t << 3.0, 0.0, 0.0;
	wrist_t << 3.0, 0.0, 0.0;
	hand_t << 1.0, 0.0, 0.0;
}

Jacobian::~Jacobian()
{
}

/// <summary>
/// Function sets the Jacobian matrix
/// </summary>
/// <param name="theta">Current theta values</param>
void Jacobian::setJacobians(double* theta)
{
	for (int i = 0; i < 7; i++) {
		Eigen::Vector4d vec = getJacobianColumn(i, theta);
		jac.col(i)(0) = vec(0);
		jac.col(i)(1) = vec(1);
		jac.col(i)(2) = vec(2);
	}
}

/// <summary>
/// Function returns column vector for Jacobian based on current
/// </summary>
/// <param name="column">Target column</param>
/// <param name="theta">Current theta values</param>
/// <returns></returns>
Eigen::Vector4d Jacobian::getJacobianColumn(int column, double* theta)
{
	Eigen::Vector4d vec;
	switch (column)
	{
	case 0:
		vec = translate(root_t) * translate(shoulder_t) * rotateZ(theta[2]) * rotateY(theta[1]) * rotateXInv(theta[0])
			* translate(elbow_t) * rotateZ(theta[4]) * rotateY(theta[3])
			* translate(wrist_t) * rotateY(theta[6]) * rotateX(theta[5]) * translate(hand_t) * origin;
		break;
	case 1:
		vec = translate(root_t) * translate(shoulder_t) * rotateZ(theta[2]) * rotateYInv(theta[1]) * rotateX(theta[0])
			* translate(elbow_t) * rotateZ(theta[4]) * rotateY(theta[3])
			* translate(wrist_t) * rotateY(theta[6]) * rotateX(theta[5]) * translate(hand_t) * origin;
		break;
	case 2:
		vec = translate(root_t) * translate(shoulder_t) * rotateZInv(theta[2]) * rotateY(theta[1]) * rotateX(theta[0])
			* translate(elbow_t) * rotateZ(theta[4]) * rotateY(theta[3])
			* translate(wrist_t) * rotateY(theta[6]) * rotateX(theta[5]) * translate(hand_t) * origin;
		break;
	case 3:
		vec = translate(root_t) * translate(shoulder_t) * rotateZ(theta[2]) * rotateY(theta[1]) * rotateX(theta[0])
			* translate(elbow_t) * rotateZ(theta[4]) * rotateYInv(theta[3])
			* translate(wrist_t) * rotateY(theta[6]) * rotateX(theta[5]) * translate(hand_t) * origin;
		break;
	case 4:
		vec = translate(root_t) * translate(shoulder_t) * rotateZ(theta[2]) * rotateY(theta[1]) * rotateX(theta[0])
			* translate(elbow_t) * rotateZInv(theta[4]) * rotateY(theta[3])
			* translate(wrist_t) * rotateY(theta[6]) * rotateX(theta[5]) * translate(hand_t) * origin;
		break;
	case 5:
		vec = translate(root_t) * translate(shoulder_t) * rotateZ(theta[2]) * rotateY(theta[1]) * rotateX(theta[0])
			* translate(elbow_t) * rotateZ(theta[4]) * rotateY(theta[3])
			* translate(wrist_t) * rotateY(theta[6]) * rotateXInv(theta[5]) * translate(hand_t) * origin;
		break;
	default:
		vec = translate(root_t) * translate(shoulder_t) * rotateZ(theta[2]) * rotateY(theta[1]) * rotateX(theta[0])
			* translate(elbow_t) * rotateZ(theta[4]) * rotateY(theta[3])
			* translate(wrist_t) * rotateYInv(theta[6]) * rotateX(theta[5]) * translate(hand_t) * origin;
		break;
	}
	return vec;
}

/// <summary>
/// Function returns current position of end effector based on provided thetas
/// </summary>
/// <param name="theta">Theta values of rotations</param>
/// <returns>Current position of end effector</returns>
Eigen::Vector4d Jacobian::getEndPosition(double* theta)
{
	Eigen::Vector4d vec = translate(root_t) * translate(shoulder_t) * rotateZ(theta[2]) * rotateY(theta[1]) * rotateX(theta[0])
		* translate(elbow_t) * rotateZ(theta[4]) * rotateY(theta[3])
		* translate(wrist_t) * rotateY(theta[6]) * rotateX(theta[5]) * translate(hand_t) * origin;
	return vec;
}

Eigen::Matrix4d Jacobian::rotateX(double theta)
{
	Eigen::Matrix4d rotMat;

	rotMat <<
		1.0, 0.0, 0.0, 0.0,
		0.0, cos(theta), -sin(theta), 0.0,
		0.0, sin(theta), cos(theta), 0.0,
		0.0, 0.0, 0.0, 1.0;

	return rotMat;
}

Eigen::Matrix4d Jacobian::rotateY(double theta)
{
	Eigen::Matrix4d rotMat;
	rotMat <<
		cos(theta), 0.0, sin(theta), 0.0,
		0.0, 1.0, 0.0, 0.0,
		-sin(theta), 0.0, cos(theta), 0.0,
		0.0, 0.0, 0.0, 1.0;
	return rotMat;
}

Eigen::Matrix4d Jacobian::rotateZ(double theta)
{
	Eigen::Matrix4d rotMat;
	rotMat <<
		cos(theta), -sin(theta), 0.0, 0.0,
		sin(theta), cos(theta), 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		0.0, 0.0, 0.0, 1.0;
	return rotMat;
}

Eigen::Matrix4d Jacobian::rotateXInv(double theta)
{
	Eigen::Matrix4d rotMat;
	rotMat <<
		0.0, 0.0, 0.0, 0.0,
		0.0, -sin(theta), -cos(theta), 0.0,
		0.0, cos(theta), -sin(theta), 0.0,
		0.0, 0.0, 0.0, 0.0;
	return rotMat;
}

Eigen::Matrix4d Jacobian::rotateYInv(double theta)
{
	Eigen::Matrix4d rotMat;
	rotMat <<
		-sin(theta), 0.0, cos(theta), 0.0,
		0.0, 0.0, 0.0, 0.0,
		-cos(theta), 0.0, -sin(theta), 0.0,
		0.0, 0.0, 0.0, 0.0;
	return rotMat;
}

Eigen::Matrix4d Jacobian::rotateZInv(double theta)
{
	Eigen::Matrix4d rotMat;
	rotMat <<
		-sin(theta), -cos(theta), 0.0, 0.0,
		cos(theta), -sin(theta), 0.0, 0.0,
		0.0, 0.0, 0.0, 0.0,
		0.0, 0.0, 0.0, 0.0;
	return rotMat;
}

Eigen::Matrix4d Jacobian::translate(const Eigen::Vector3d& vec)
{
	Eigen::Matrix4d transMat;
	transMat <<
		1.0, 0.0, 0.0, vec(0),
		0.0, 1.0, 0.0, vec(1),
		0.0, 0.0, 1.0, vec(2),
		0.0, 0.0, 0.0, 1.0;
	return transMat;
}
