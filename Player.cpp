#include "Player.h"

void Player::move(int _x, int _y) {
	int x = this->x + _x;
	int y = this->y + _y;
	extern Grid grid;
	if (x >= 0 && x < grid.getWidth() &&
		y >= 0 && y < grid.getHeight() &&
		(grid.cells[y][x].height == 0 || grid.cells[y][x].height >= grid.cells[this->y][this->x].height)) {
		this->x = x;
		this->y = y;
		position.x = x;
		position.y = y;
	}

}