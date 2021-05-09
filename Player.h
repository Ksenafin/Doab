#pragma once
#include "Character.h"
#include "grid.h"
class Player : public Character
{
public:
	void move(int x, int y);
};

