#pragma once
#include<math.h>
#include <Eigen/Dense>
#include <iostream>

#define PI 3.14159265

class Jacobian
{
public:
	Jacobian();
	~Jacobian();

	void setJacobians(double* theta);
	Eigen::Vector4d getJacobianColumn(int column, double* theta);
	Eigen::Vector4d getEndPosition(double* theta);
	Eigen::Matrix4d rotateX(double theta);
	Eigen::Matrix4d rotateY(double theta);
	Eigen::Matrix4d rotateZ(double theta);
	Eigen::Matrix4d rotateXInv(double theta);
	Eigen::Matrix4d rotateYInv(double theta);
	Eigen::Matrix4d rotateZInv(double theta);
	Eigen::MatrixXd jac = Eigen::MatrixXd(3, 7);
	
	Eigen::Matrix4d translate(const Eigen::Vector3d& vec);

	EIGEN_MAKE_ALIGNED_OPERATOR_NEW

protected:
	int m = 4;
	int n = 7;

	Eigen::Vector4d origin;
	Eigen::Vector4d target;
	Eigen::Vector4d p_current;
	Eigen::Vector3d root_t;
	Eigen::Vector3d shoulder_t;
	Eigen::Vector3d elbow_t;
	Eigen::Vector3d wrist_t;
	Eigen::Vector3d hand_t;
};

