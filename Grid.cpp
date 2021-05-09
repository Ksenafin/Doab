#include "Grid.h"

Grid::Grid(int _width, int _height) : width{ _width }, height{ _height } {
	cells = new Cell*[height];
	for (int i = 0; i < height; ++i) {
		cells[i] = new Cell[width];
	}
	build();
}

Grid::~Grid() {
	for (auto i = 0; i < height; ++i) {
		delete[] cells[i];
	}
	delete[] cells;
}


std::vector<int> Grid::targetCell(float x, float y, float dx, float dy) {
	int maxHeight = 10;
	int minHeight = -10;

	int z = maxHeight;

	while (	z >= minHeight &&
			((x < 0 || x >= width || y < 0 || y >= height) ||
			 cells[(int)y][(int)x].height != z)) {
		x -= dx;
		y -= dy;
		--z;
	}

	if (z >= minHeight) {
		float remainder = x - (int)x;
		if (remainder > 0.1f && remainder < 0.9f) {
			remainder = y - (int)y;
			if (remainder > 0.1f && remainder < 0.9f) {
				std::cout << "x: " << (int)x << std::endl;
				std::cout << "y: " << (int)y << std::endl;
				std::cout << "z: " << z << std::endl << std::endl;
				return std::vector<int>{(int)x, (int)y};
			}
		}
	}
	return std::vector<int>{-1, -1};

}



//Labyrinth builder
void Grid::build() {
	/*
	cells[0][3].height = 1;
	cells[0][4].height = 0;
	cells[0][5].height = 1;
	cells[2][0].height = 2;
	cells[2][1].height = 1;
	cells[2][2].height = 1;
	cells[3][0].height = -1;
	cells[3][1].height = -1;
	cells[3][2].height = 0;
	cells[4][0].height = -1;
	cells[4][1].height = -1;
	cells[4][6].height = 3;
	cells[5][2].height = -1;
	cells[5][3].height = -1;
	cells[6][0].height = -1;
	cells[6][1].height = -1;
	cells[6][2].height = -1;
	cells[6][3].height = -1;
	cells[6][4].height = -1;
	cells[6][5].height = -1;
	cells[6][6].height = -1;
	cells[6][7].height = -1;
	
	cells[4][6].type = CLIMB_FRONT;

	cells[0][4].type = DESCENT_FRONT;
	cells[2][1].type = DESCENT_RIGHT;
	cells[3][2].type = DESCENT_FRONT;
	cells[5][2].type = DESCENT_RIGHT;
	cells[5][3].type = CLIMB_RIGHT;
	cells[6][2].type = CLIMB_RIGHT;
	cells[6][3].type = DESCENT_RIGHT;
	cells[6][5].type = CLIMB_FRONT;
	cells[6][6].type = DESCENT_FRONT;
	cells[6][7].type = DESCENT_FRONT;
	*/


	auto passable = false;
	auto mult = 1;
	auto count = 0;
	for (auto x = 1; x < width - 1; ++x) {
		if (rand() % 100 < 35) {
			cells[1][x].type = mult;
		}
		else {
			++mult;
		}
	}
	++mult;
	for (auto y = 2; y < height - 1; ++y) {
		if (y < height - 2)
			for (auto x = 1; x < width - 1; ++x) {
				if (rand() % 100 < 30) {
					cells[y][x].type = mult;
				}
				else {
					++mult;
				}
			}
		passable = false;
		count = 0;
		mult = 0;
		for (auto x = 1; x < width - 1; ++x) {
			if (cells[y - 1][x].type == 0) {
				passable = false;
				count = 0;
				mult = 0;
			}
			else {
				++count;
				if (mult == 0) {
					mult = cells[y - 1][x].type;
				}
				if (cells[y][x].type > 0) {
					passable = true;
					cells[y][x].type = mult;
				}
				if (!passable && cells[y - 1][x + 1].type == 0) {
					cells[y][x - rand() % count].type = mult;
				}
			}
		}
		mult = 0;
		count = 0;
		for (auto x = 1; x < width - 1; ++x) {
			if (cells[y][x].type > 0) {
				++count;
				if (mult == 0 || mult > cells[y][x].type) {
					mult = cells[y][x].type;
				}
				if (cells[y][x + 1].type == 0) {
					for (auto _x = x - count + 1; _x <= x; ++_x) {
						cells[y][_x].type = mult;
					}
					count = 0;
					mult = 0;
				}
			}
		}
	}
	mult = 0;
	for (auto x = 1; x < width - 1; ++x) {
		if (cells[height - 2][x].type != 0) {
			if (mult == 0) {
				mult = cells[height - 2][x].type;
				count = 1;
			}
			else if (mult == cells[height - 2][x].type) {
				count = 1;
			}
			else {
				for (auto _x = x - count; _x < x; ++_x) {
					cells[height - 2][_x].type = mult;
				}
				for (auto _x = x + 1; _x < width - 2; ++_x) {
					if (cells[height - 2][_x].type == cells[height - 2][x].type)
						cells[height - 2][_x].type = mult;
				}
				cells[height - 2][x].type = mult;
			}
		}
		else {
			++count;
		}
	}

	for (auto y = 0; y < height; ++y) {
		for (auto x = 0; x < width; ++x) {
			if (cells[y][x].type == 0) {
				cells[y][x].height = -1;
			}
			else {
				cells[y][x].type = 0;
			}
		}
	}

}

int Grid::getWidth() {
	return width;
}
int Grid::getHeight() {
	return height;
}
