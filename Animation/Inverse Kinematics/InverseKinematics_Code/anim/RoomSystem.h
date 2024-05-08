#pragma once
#include "BaseSystem.h"
#include <vector>

class RoomSystem :public BaseSystem
{
public:
	RoomSystem(const std::string& name);
	~RoomSystem();
	void getState(double* p);
	void setState(double* p);
	void reset(double time);
	void display(GLenum mode = GL_RENDER);
	int command(int argc, myCONST_SPEC char** argv);


protected:
	std::vector<double*> wall;
	std::vector<double*> floor;
	std::vector < double*> board;

};

