#pragma once

#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Grid.h"
#include "Render.h"

class Camera
{
private:
	float pitch = 1.57079f;		//radian
	float yaw = 0.0f;			//radian
	float radius = 10.0f;
	glm::vec3 focus = glm::vec3(0.0f);

	const glm::vec3 vectorUp = glm::vec3(0.0f, 0.0f, 1.0f);

	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 viewFront = glm::mat4(1.0f);
	glm::mat4 projection = glm::infinitePerspective(1.5708f, 1.0f / 1.0f, 0.1f);

	void transform();
public:
	glm::vec3 position = glm::vec3(0.0f);
	glm::vec3 direction = glm::vec3(0.0f);
	glm::mat4 transformMatrixGrid = glm::mat4(1.0f);
	glm::mat4 transformMatrixObject = glm::mat4(1.0f);

	Camera();

	void rotate(float const& deltaPitch, float const& deltaYaw);
	void move(glm::vec3 const& focus);
	void scale(float const& deltaRadius);

	void perspective(float const& aspect, float const& fovy, float const& near);


	void course(float xMouse, float yMouse);
};

