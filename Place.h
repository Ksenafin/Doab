#pragma once

#include <cassert>
#include <cstdlib>
#include <GLFW/glfw3.h>
#include "Cell.h"

class Place
{
	int width;	//in cells of grid
	int height;	//in cells of grid
	Cell** cells;

public:
	Place(int _width, int _height);
	~Place();
	void build();
	void populate();
	int getWidth();
	int getHeight();
};

