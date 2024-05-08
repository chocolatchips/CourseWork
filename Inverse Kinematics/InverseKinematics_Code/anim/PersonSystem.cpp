#include "PersonSystem.h"


PersonSystem::PersonSystem(const std::string& name) :
	BaseSystem(name)
{
	root_t << -0.5, -2.0, 4.0;
	shoulder_t << 0.5, 2.0, 0.0;
	elbow_t << 3.0, 0.0, 0.0;
	wrist_t << 3.0, 0.0, 0.0;
	hand_t << 1.0, 0.0, 0.0;
	target << 0.0, 0.0, 0.0, 1.0;

	theta[0] = 0.1;
	theta[1] = 0.1;
	theta[2] = 0.1;
	theta[3] = 0.1;
	theta[4] = 0.1;
	theta[5] = 0.1;
	theta[6] = 0.1;
	
	p_current = jac.getEndPosition(theta);
}

PersonSystem::~PersonSystem()
{
}


void printMatrixColMajor(const Eigen::MatrixXd matrix) {
	for (int i = 0; i < matrix.cols(); i++) {
		std::string output;
		for (int j = 0; j < matrix.rows(); j++) {
			output += std::to_string(matrix(j, i)) + " ";
		}
		output += '\n';
		char* out = new char[output.length()];
		strcpy(out, output.c_str());
		animTcl::OutputMessage(out);
	}
	animTcl::OutputMessage("\n");
}

void printMatrixRowMajor(const Eigen::MatrixXd matrix) {
	for (int i = 0; i < matrix.rows(); i++) {
		std::string output;
		for (int j = 0; j < matrix.cols(); j++) {
			output += std::to_string(matrix(i, j)) + " ";
		}
		output += '\n';
		char* out = new char[output.length()];
		strcpy(out, output.c_str());
		animTcl::OutputMessage(out);
	}
	animTcl::OutputMessage("\n");
}

void printThetas(double* theta) {
	std::string output;
	for (int i = 0; i < 7; i++) {
		output += std::to_string(theta[i]) + " ";
	}
	output += '\n';
	char* out = new char[output.length()];
	strcpy(out, output.c_str());
	animTcl::OutputMessage(out);
	animTcl::OutputMessage("\n");
}

void printAngles(double* theta) {
	std::string output;
	for (int i = 0; i < 7; i++) {
		output += std::to_string(theta[i] * 180.0 / PI) + " ";
	}
	output += '\n';
	char* out = new char[output.length()];
	strcpy(out, output.c_str());
	animTcl::OutputMessage(out);
	animTcl::OutputMessage("\n");
}

/// <summary>
/// Function returns current location of end effector
/// </summary>
/// <param name="p"></param>
void PersonSystem::getState(double* p)
{
	p_current = jac.getEndPosition(theta);
	p[0] = p_current(0);
	p[1] = p_current(1);
	p[2] = p_current(2);

}

/// <summary>
/// Function calculates right arm theta values for target point
/// </summary>
/// <param name="p">target point</param>
void PersonSystem::setState(double* p)
{
	if (!targetCommandSet)
		target << p[0], p[1], p[2], 1.0;
	
	error = (target - p_current);
	double len = error.norm();
	if (len > 0.1) {
		error = error.normalized();
		error = error * 0.1;
	}

	jac.setJacobians(theta);
	Eigen::MatrixXd J = jac.jac;
	Eigen::MatrixXd JT = J.transpose();
	Eigen::Vector3d X; X << error(0), error(1), error(2);
	Eigen::MatrixXd JJT = J * JT;
	Eigen::MatrixXd B = JJT.partialPivLu().solve(X);
	Eigen::MatrixXd O = JT * B;

	for (int i = 0; i < 7; i++) {
		theta[i] = theta[i] + O(i);
	}
}

/// <summary>
/// Function resets theta values for right arm and clears current target
/// </summary>
/// <param name="time">Time reset command given</param>
void PersonSystem::reset(double time)
{
	theta[0] = 0.1;
	theta[1] = 0.1;
	theta[2] = 0.1;
	theta[3] = 0.1;
	theta[4] = 0.1;
	theta[5] = 0.1;
	theta[6] = 0.1;

	target.setZero();
	targetCommandSet = false;
}

/// <summary>
/// Function accepts commands targeting system
/// </summary>
/// <param name="argc">Number of inputs</param>
/// <param name="argv">Command parameters</param>
/// <returns>Command success</returns>
int PersonSystem::command(int argc, myCONST_SPEC char** argv)
{
	if (argc < 1) {
		animTcl::OutputMessage("system %s: wrong number of params.", m_name.c_str());
		return TCL_ERROR;
	}
	else if (strcmp(argv[0], "position") == 0) {
		if (argc != 4) {
			animTcl::OutputMessage("Incorrect usage: position <x> <y> <z>");
			return TCL_ERROR;
		}
		return targetCommand(std::stod(argv[1]), std::stod(argv[2]), std::stod(argv[3]));
	}
	return TCL_OK;
}

/// <summary>
/// Function sets target to input values
/// </summary>
/// <param name="x">X coordinate</param>
/// <param name="y">Y coordinate</param>
/// <param name="z">Z coordinate</param>
/// <returns></returns>
int PersonSystem::targetCommand(double x, double y, double z)
{
	target << x, y, z, 1.0;
	targetCommandSet = true;
	return TCL_OK;
}

/// <summary>
/// Function returns if current target point is set by command
/// </summary>
/// <returns></returns>
bool PersonSystem::targetPointSet()
{
	return targetCommandSet;
}

/// <summary>
/// Function returns current target point
/// </summary>
/// <returns>Target point</returns>
double* PersonSystem::getTarget()
{
	double p_target[3] = { target(0), target(1), target(2) };
	return p_target;
}

/// <summary>
/// Function creates a person made of ovals
/// </summary>
/// <param name="mode"></param>
void PersonSystem::display(GLenum mode)
{
	set_colour(0.f, 1.f, 0.f);
	glPushMatrix();
	{
		// Draw root
		glTranslated(root_t(0), root_t(1), root_t(2));
		glPushMatrix();
		{
			glScaled(1.0, 2.5, 1.0);
			drawCircle();
		}
		glPopMatrix();

		// Draw legs
		drawLeg(true);
		drawLeg(false);

		// Draw head
		glPushMatrix();
		{
			glTranslated(0.0, 3.25, 0.0);
			glPushMatrix();
			{
				glScaled(0.75, 0.75, 1.0);
				drawCircle();
			}
			glPopMatrix();
		}
		glPopMatrix();

		// Draw left arm
		drawLeftArm();

		// Draw right arm
		drawRightArm();
	}
	glPopMatrix();
	
}

/// <summary>
/// Function draws a circle at origin of current coordinate system
/// </summary>
void PersonSystem::drawCircle()
{
		GLdrawCircle(1.0, 20);
}

/// <summary>
/// Function draws a leg
/// </summary>
/// <param name="isLeft"></param>
void PersonSystem::drawLeg(bool isLeft)
{
	double side = isLeft ? -1.0 : 1.0;
	set_colour(0.f, 1.f, 0.f);
	glPushMatrix();
	{
		glTranslated(side * 0.75, -3.75, 0.0);
		// Draw thigh
		glPushMatrix();
		{
			glScaled(0.5, 1.625, 1.0);
			drawCircle();
		}
		glPopMatrix();
		// Draw shin
		glTranslated(0.0, -3.125, 0.0);
		glPushMatrix();
		{
			glScaled(0.5, 1.5, 1.0);
			drawCircle();
		}
		glPopMatrix();
		glTranslated(0.0, -1.75, 0.0);
		// Draw foot
		glPushMatrix();
		{
			glScaled(0.5, 0.25, 1.0);
			drawCircle();
		}
		glPopMatrix();
	}
	glPopMatrix();
}

/// <summary>
/// Function draws the left arm fully extended
/// </summary>
void PersonSystem::drawLeftArm()
{
	glPushMatrix();
	{
		glTranslated(-2.0, 2.0, 0.0);
		glPushMatrix();
		{
			glScaled(1.5, 0.25, 1.0);
			drawCircle();
		}
		glPopMatrix();
		glTranslated(-3.0, 0.0, 0.0);
		glPushMatrix();
		{
			glScaled(1.5, 0.25, 1.0);
			drawCircle();
		}
		glPopMatrix();
		glTranslated(-2.0, 0.0, 0.0);
		glPushMatrix();
		{
			glScaled(0.5, 0.35, 1.0);
			drawCircle();
		}
		glPopMatrix();
	}
	glPopMatrix();
}

/// <summary>
/// Function draws the right arm with end effector at end of hand
/// </summary>
void PersonSystem::drawRightArm()
{
	glPushMatrix();
	{
		// Move to shoulder ->T_sh R_x R_y R_z
		glTranslated(shoulder_t(0), shoulder_t(1), shoulder_t(2));
		glRotated(theta[2] * 180.0 / PI, 0.0, 0.0, 1.0);
		glRotated(theta[1] * 180.0 / PI, 0.0, 1.0, 0.0);
		glRotated(theta[0] * 180.0 / PI, 1.0, 0.0, 0.0);

		glPushMatrix();
		{
			// Draw upper arm
			glTranslated(1.5, 0.0, 0.0);
			glScaled(1.5, 0.25, 1.0);
			drawCircle();
		}
		glPopMatrix();

		// Move to elbow -> T_el R_y R_z
		glTranslated(elbow_t(0), elbow_t(1), elbow_t(2));
		glRotated(theta[4] * 180.0 / PI, 0.0, 0.0, 1.0);
		glRotated(theta[3] * 180.0 / PI, 0.0, 1.0, 0.0);

		glPushMatrix();
		{
			// Draw forearm
			glTranslated(1.5, 0.0, 0.0);
			glScaled(1.5, 0.25, 1.0);
			drawCircle();
		}
		glPopMatrix();

		// Move to wrist -> T_wr R_x R_y
		glTranslated(wrist_t(0), wrist_t(1), wrist_t(2));
		glRotated(theta[6] * 180.0 / PI, 0.0, 1.0, 0.0);
		glRotated(theta[5] * 180.0 / PI, 1.0, 0.0, 0.0);

		glPushMatrix();
		{
			// Draw hand
			glTranslated(0.5, 0.0, 0.0);
			glScaled(0.5, 0.35, 1.0);
			drawCircle();
		}
		glPopMatrix();

		// Move to fingers
		glTranslated(hand_t(0), hand_t(1), hand_t(2));
		glPushMatrix();
		{
			// Draw p_current effector
			set_colour(1.0, 0.0, 0.0);
			glScaled(0.25, 0.25, 0.25);
			drawSphere();
		}
		glPopMatrix();
	}
	glPopMatrix();
	
}
