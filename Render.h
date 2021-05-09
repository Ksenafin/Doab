#pragma once

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SOIL.h>
// FreeType
#include <ft2build.h>
#include FT_FREETYPE_H

#include <map>
#include <string>

#include "Cell.h"
#include "Shader.h"
#include "Player.h"
#include "Sumbol.h"

#define HORIZONTAL		0
#define DESCENT_RIGHT	1
#define DESCENT_FRONT	2
#define CLIMB_RIGHT		3
#define CLIMB_FRONT		4
#define RIGHTSIDE_UP	5
#define RIGHTSIDE_DOWN	6
#define FRONTSIDE_UP	7
#define FRONTSIDE_DOWN	8
#define LEFTSIDE_UP		9
#define LEFTSIDE_DOWN	10
#define BACKSIDE_UP		11
#define BACKSIDE_DOWN	12

#define LOC_TRANSFORM_GRID		0
#define LOC_TRANSFORM_OBJECT	0
#define LOC_GRID				1
#define LOC_TEXTURE				2
#define LOC_FONT				3

#define VERTEX_GRID		0
#define VERTEX_FONT		1
#define VERTEX_PLAYER	2


class Render
{
private:
	GLuint uniformLocations[10];
	GLuint VAO[10];
	GLuint VBO[10];
	GLfloat* vertexGrid;
	GLfloat vertexPlayer[36];
	long vertexGridSize = 0;
	void createFace(GLfloat _x, GLfloat _y, GLfloat _z, GLfloat _type, int _rotate, GLfloat* arr);
	void createTriangle(GLfloat _x, GLfloat _y, GLfloat _z, GLfloat _type, int _rotate, GLfloat* arr);
	void createFont();

	std::map<GLchar, Sumbol> font;

public:
	Shader gridShader;
	Shader objectShader;
	Shader interfaceShader;
	Render();
	void createVertexGrid(Cell** cells, int xStart, int xEnd, int yStart, int yEnd);
	void bindGrid();
	void transformView(glm::mat4& transformMatrixGrid, glm::mat4& transformMatrixObject);
	void renderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);

	GLuint bufferImage(const char* filename, const char* nameUniform, Shader& shader, const int numberTexture);
	void bindPlayer(Player player);
};

