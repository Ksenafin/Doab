#include "Place.h"

Place::Place(int _width, int _height) : width{ _width }, height{ _height } {
	cells = new Cell * [height];
	for (int i = 0; i < height; ++i) {
		cells[i] = new Cell[width];
	}
	build();
}

Place::~Place() {
	for (auto i = 0; i < width; ++i) {
		delete[] cells[i];
	}
	delete[] cells;
}

//Labyrinth builder
void Place::build() {
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
}

int Place::getWidth() {
	return width;
}
int Place::getHeight() {
	return height;
}