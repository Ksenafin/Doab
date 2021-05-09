#include "GameWindow.h"


GameWindow::GameWindow(int _width = 640, int _height = 480) :
	screenWidth{ _width },
	screenHeight{ _height }
{
	if (!glfwInit()) {
		std::cerr << "GL initialization failed" << std::endl;
	}
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
	glfwWindowHint(GLFW_SAMPLES, 8);
	
	glfwWindow = glfwCreateWindow(screenWidth, screenHeight, "Doab", nullptr, nullptr);
	if (!glfwWindow) {
		std::cerr << "Window or GL initialization failed";
		glfwTerminate();
	}
	glfwMakeContextCurrent(glfwWindow);
	
	glfwSetWindowSizeCallback(glfwWindow, resizeCallback);
	glfwSetKeyCallback(glfwWindow, keyCallback);
	glfwSetCursorPosCallback(glfwWindow, mouseCallback);
	glfwSetScrollCallback(glfwWindow, scrollCallback);
	
	glewExperimental = GL_TRUE;
	glewInit();

	glfwGetFramebufferSize(glfwWindow, &screenWidth, &screenHeight);
	glViewport(0, 0, screenWidth, screenHeight);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

GameWindow::~GameWindow() {

}

void GameWindow::resize(int width, int height) {
	screenWidth = width;
	screenHeight = height;
	glViewport(0, 0, screenWidth, screenHeight);
	//render.resize();
}
