#include "RoomSystem.h"

RoomSystem::RoomSystem(const std::string& name) :
	BaseSystem(name)
{
	wall.push_back(new double[3] {-11.0, -11.0, -0.2});
	wall.push_back(new double[3] {11.0, -11.0, -0.2});
	wall.push_back(new double[3] {11.0, 11.0, -0.2});
	wall.push_back(new double[3] {-11.0, 11.0, -0.2});


	floor.push_back(new double[3] {-11.0, -11.0, -0.2});
	floor.push_back(new double[3] {-11.0, -11.0, 10.0});
	floor.push_back(new double[3] {11.0, -11.0, 10.0});
	floor.push_back(new double[3] {11.0, -11.0, -0.2});


	board.push_back(new double[3] {-6.0, -3.0, -0.1});
	board.push_back(new double[3] {6.0, -3.0, -0.1});
	board.push_back(new double[3] {6.0, 4.0, -0.1});
	board.push_back(new double[3] {-6.0, 4.0, -0.1});
}

RoomSystem::~RoomSystem()
{
}

void RoomSystem::getState(double* p)
{
}

void RoomSystem::setState(double* p)
{
}

void RoomSystem::reset(double time)
{
}

void RoomSystem::display(GLenum mode)
{
	set_colour(0.f, 0.f, 0.f);
	glBegin(GL_QUADS);
	for each (double* point in board) {
		glVertex3dv(point);
	}
	glEnd();

	set_colour(1.f, 1.f, 1.f);
	glBegin(GL_QUADS);
	for each (double* point in wall) {
		glVertex3dv(point);
	}
	glEnd();

	set_colour(0.57f, 0.42f, 0.23f);
	glBegin(GL_QUADS);
	for each (double* point in floor) {
		glVertex3dv(point);
	}
	glEnd();

}

int RoomSystem::command(int argc, myCONST_SPEC char** argv)
{
	return TCL_OK;
}
