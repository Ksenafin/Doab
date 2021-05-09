#pragma once
#include <glm/glm.hpp>

class Character
{
private:
	double hp = 1;
	double speed = 1;

public:
	int x = 0;
	int y = 0;
	glm::vec3 position = glm::vec3(0.0f);
};

