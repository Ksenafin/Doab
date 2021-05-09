#pragma once

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Cell.h"
#include "callback.h"
#include "Grid.h"
#include "Camera.h"
#include "Render.h"


class GameWindow
{
public:
	int screenWidth;
	int screenHeight;
	GLFWwindow* glfwWindow;

	//double mousePosition[2];	//0 - x; 1 -y
	//bool pressButton[2];		//0 - left mouse button; 1 - right mouse button

	GLfloat timeLastFrame;

	GameWindow(int width, int height);
	~GameWindow();

	void resize(int width, int height);

};

