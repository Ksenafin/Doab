#include "callback.h"

void resizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	extern Camera camera;
	extern Player player;
	//action: GLFW_PRESS, GLFW_RELEASE or GLFW_REPEAT
	//mods - bits of press: 1 - shift, 2 - ctrl, 4 - alt, 8 - super, 16 - capslock, 32 - numlock
	switch (key) {
	case GLFW_KEY_ESCAPE:
		glfwSetWindowShouldClose(window, GL_TRUE);
		break;
	case GLFW_KEY_LEFT:
		if (mods & 2) {
			camera.rotate(0.0f, -0.01f);
		}
		else {
			player.move(-1, 0);
			camera.move(player.position);
		}
		break;
	case GLFW_KEY_RIGHT:
		if (mods & 2) {
			camera.rotate(0.0f, 0.01f);
		}
		else {
			player.move(1, 0);
			camera.move(player.position);
		}
		break;
	case GLFW_KEY_UP:
		if (mods & 2) {
			camera.rotate(0.01f, 0.0f);
		}
		else {
			player.move(0, 1);
			camera.move(player.position);
		}
		break;
	case GLFW_KEY_DOWN:
		if (mods & 2) {
			camera.rotate(-0.01f, 0.0f);
		}
		else {
			player.move(0, -1);
			camera.move(player.position);
		}
		break;
	}

}

void mouseCallback(GLFWwindow* window, double xMousePosition, double yMousePosition)
{
	extern Grid grid;
	xMousePosition = (xMousePosition / grid.getWidth() * 2 - 1);
	yMousePosition = (1 - yMousePosition / grid.getHeight() * 2);
	extern Camera camera;
	extern Player player;

	camera.course(xMousePosition, yMousePosition);

}
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	extern Camera camera;
	camera.scale(-yoffset);
}