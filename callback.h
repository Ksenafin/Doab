#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Camera.h"
#include "Grid.h"
#include "Player.h"


//GameWindow gameWindow;
void resizeCallback(GLFWwindow* window, int width, int height);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouseCallback(GLFWwindow* window, double xpos, double ypos);
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

