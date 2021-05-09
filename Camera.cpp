#include "Camera.h"

Camera::Camera()
{
	rotate(0.0f, 0.0f);
}

void Camera::rotate(float const& deltaPitch, float const& deltaYaw) {
	pitch += deltaPitch;
	if (pitch > 1.3f)		//~74.5 degree
		pitch = 1.3f;
	else if (pitch < 0.2f)	//~11.5 degree
		pitch = 0.2f;
	yaw += deltaYaw;

	if (yaw > 6.283185f)
		yaw -= 6.283185f;
	else if (yaw < 0.0f)
		yaw += 6.283185f;

	direction.x = static_cast<float>(std::cos(pitch) * std::sin(yaw)) * radius;
	direction.y = static_cast<float>(-std::cos(pitch) * std::cos(yaw)) * radius;
	direction.z = static_cast<float>(std::sin(pitch)) * radius;
	transform();
}

void Camera::move(glm::vec3 const& focus = glm::vec3(0.0f)) {
	this->focus = focus;
	transform();
}

void Camera::scale(float const& deltaRadius) {
	radius += deltaRadius;
	if (radius > 100.0f)
		radius = 100.0f;
	else if (radius < 1.0f)
		radius = 1.0f;
	direction.x = static_cast<float>(std::cos(pitch) * std::sin(yaw)) * radius;
	direction.y = static_cast<float>(-std::cos(pitch) * std::cos(yaw)) * radius;
	direction.z = static_cast<float>(std::sin(pitch)) * radius;
	transform();
}

void Camera::transform() {
	position = direction + focus;
	view = glm::lookAtRH(position, focus, vectorUp);

	viewFront = glm::lookAtRH(
		glm::vec3(
			static_cast<float>(std::cos(pitch)) * radius + focus.x,
			0 + focus.y,
			static_cast<float>(std::sin(pitch)) * radius + focus.z
		),
		focus,
		vectorUp);

	transformMatrixGrid = projection * view;
	transformMatrixObject = projection * viewFront;
	extern Render render;
	render.transformView(transformMatrixGrid, transformMatrixObject);
}

void Camera::perspective(float const& aspect, float const& fovy = 1.5708f,  float const& near = 0.1f) {
	projection = glm::infinitePerspective(fovy, aspect, near);
	transform();
}

void Camera::course(float xMouse, float yMouse) {
	extern Grid grid;

	xMouse *= radius;
	yMouse *= radius;

	float x = xMouse * cos(yaw) - yMouse * sin(yaw) * sin(pitch) + focus.x;
	float y = xMouse * sin(yaw) + yMouse * cos(yaw) * sin(pitch) + focus.y;
	float z = yMouse * cos(pitch);

	float dx = (x - position.x) / (z - position.z);
	float dy = (y - position.y) / (z - position.z);

	z = 10;
	x = position.x + (z - position.z) * dx;
	y = position.y + (z - position.z) * dy;

	grid.targetCell(x, y, dx, dy);

}