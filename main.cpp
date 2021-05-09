#include <iostream>
#include <cstdlib>
#include <ctime>
#include <ratio>
#include <chrono>

#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include "Camera.h"
#include "Render.h"
#include "Sumbol.h"
#include "GameWindow.h"


GameWindow game(640, 640);	//pixels (width, height)
Render render;
Camera camera;
Grid grid(30, 30);			//cells (width, height)
Player player;

int main() {
	render.createVertexGrid(grid.cells, 0, grid.getWidth()-1, 0, grid.getHeight() -1);
	GLuint textureDescent = render.bufferImage("textures/descent.jpg", "textureDescent", render.gridShader, 1);
	GLuint textureFloor = render.bufferImage("textures/floor.jpg", "textureFloor", render.gridShader, 2);
	GLuint textureGrass = render.bufferImage("textures/grass.png", "textureGrass", render.gridShader, 3);
	GLuint textureUnder = render.bufferImage("textures/under.jpg", "textureUnder", render.gridShader, 4);
	GLuint textureBrick = render.bufferImage("textures/brick.jpg", "textureBrick", render.gridShader, 5);
	GLuint texturePlayer = render.bufferImage("textures/player.png", "texturePlayer", render.objectShader, 6);


	while (!glfwWindowShouldClose(game.glfwWindow))
	{
		glfwPollEvents();

		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		render.bindGrid();
		render.bindPlayer(player);
		//render.renderText("This is sample text", 25.0f, 25.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));

		glfwSwapBuffers(game.glfwWindow);
	}
	//glDeleteVertexArrays(1, &VAO);
	//glDeleteBuffers(1, &VAO);
	//glDeleteBuffers(1, &VBO);

	glfwTerminate();

	return 0;
}
