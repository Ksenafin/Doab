#pragma once
#include <iostream>
#include <cassert>
#include <cstdlib>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>

#include "Cell.h"

#define HORIZONTAL		0
#define DESCENT_RIGHT	1
#define DESCENT_FRONT	2
#define CLIMB_RIGHT		3
#define CLIMB_FRONT		4


class Grid
{
	int width;	//cells of grid
	int height;	//cells of grid

public:
	Cell** cells;

	Grid(int _width, int _height);
	~Grid();
	void build();
	int getWidth();
	int getHeight();
	std::vector<int> targetCell(float x, float y, float dx, float dy);
};
