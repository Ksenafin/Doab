#include "Render.h"
#include <iostream>

Render::Render()
{
	interfaceShader.load("shaders/interface.vs", "shaders/interface.frag");
	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(640), 0.0f, static_cast<GLfloat>(640));
	glUniformMatrix4fv(glGetUniformLocation(interfaceShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	createFont();

	objectShader.load("shaders/object.vs", "shaders/object.frag");
	objectShader.Use();
	uniformLocations[LOC_TRANSFORM_OBJECT] = glGetUniformLocation(objectShader.Program, "_transform");

	gridShader.load("shaders/grid.vs", "shaders/grid.frag");
	gridShader.Use();

	uniformLocations[LOC_TRANSFORM_GRID] = glGetUniformLocation(gridShader.Program, "_transform");
	/*
	objectShader.load("shaders/object.vs", "shaders/object.frag");
	objectShader.Use();

	uniformLocations[LOC_TRANSFORM] = glGetUniformLocation(objectShader.Program, "_transform");
	*/
	glGenVertexArrays(1, &VAO[VERTEX_PLAYER]);
	glGenBuffers(1, &VBO[VERTEX_PLAYER]);
}

void Render::bindGrid()
{
	gridShader.Use();
	glBindVertexArray(VAO[VERTEX_GRID]);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawArrays(GL_TRIANGLES, 0, vertexGridSize);
	glBindVertexArray(0);
}

void Render::transformView(glm::mat4& transformMatrixGrid, glm::mat4& transformMatrixObject)
{
	gridShader.Use();
	glUniformMatrix4fv(uniformLocations[LOC_TRANSFORM_GRID], 1, GL_FALSE, glm::value_ptr(transformMatrixGrid));
	objectShader.Use();
	glUniformMatrix4fv(uniformLocations[LOC_TRANSFORM_OBJECT], 1, GL_FALSE, glm::value_ptr(transformMatrixObject));
}

void Render::createVertexGrid(Cell** cells, int xStart, int xEnd, int yStart, int yEnd) {
	auto xNum = xEnd - xStart + 1;
	auto yNum = yEnd - yStart + 1;
	auto xCenter = xNum / 2;
	auto yCenter = yNum / 2;
	xCenter = 0;
	yCenter = 0;

	//Counting the amount of memory for vertices
	{
		auto triangleCount = xNum * yNum * 2;
		auto deltaHeight = 0;
		for (auto y = yStart; y < yEnd; ++y) {
			for (auto x = xStart; x < xEnd; ++x) {


				deltaHeight = (cells[y][x + 1].height - cells[y][x].height) * 2;

				if (cells[y][x].type == DESCENT_FRONT || cells[y][x].type == CLIMB_FRONT) {
					deltaHeight -= 1;
				}
				else if (cells[y][x].type == CLIMB_RIGHT) {
					deltaHeight -= 2;
				}

				if (cells[y][x + 1].type == DESCENT_FRONT || cells[y][x + 1].type == CLIMB_FRONT) {
					deltaHeight += 1;
				}
				else if (cells[y][x + 1].type == DESCENT_RIGHT) {
					deltaHeight += 2;
				}
				if (deltaHeight == 0 && (cells[y][x].type == DESCENT_FRONT && cells[y][x + 1].type == CLIMB_FRONT || cells[y][x].type == CLIMB_FRONT && cells[y][x + 1].type == DESCENT_FRONT))
					deltaHeight = 2;
				triangleCount += abs(deltaHeight);


				deltaHeight = (cells[y + 1][x].height - cells[y][x].height) * 2;

				if (cells[y][x].type == DESCENT_RIGHT || cells[y][x].type == CLIMB_RIGHT) {
					deltaHeight -= 1;
				}
				else if (cells[y][x].type == CLIMB_FRONT) {
					deltaHeight -= 2;
				}

				if (cells[y + 1][x].type == DESCENT_RIGHT || cells[y + 1][x].type == CLIMB_RIGHT) {
					deltaHeight += 1;
				}
				else if (cells[y + 1][x].type == DESCENT_FRONT) {
					deltaHeight += 2;
				}
				if (deltaHeight == 0 && (cells[y][x].type == DESCENT_RIGHT && cells[y + 1][x].type == CLIMB_RIGHT || cells[y][x].type == CLIMB_RIGHT && cells[y + 1][x].type == DESCENT_RIGHT))
					deltaHeight = 2;
				triangleCount += abs(deltaHeight);

			}
		}
		for (auto x = xStart; x < xEnd; ++x) {

			deltaHeight = (cells[yEnd][x + 1].height - cells[yEnd][x].height) * 2;

			if (cells[yEnd][x].type == DESCENT_FRONT || cells[yEnd][x].type == CLIMB_FRONT) {
				deltaHeight -= 1;
			}
			else if (cells[yEnd][x].type == CLIMB_RIGHT) {
				deltaHeight -= 2;
			}

			if (cells[yEnd][x + 1].type == DESCENT_FRONT || cells[yEnd][x + 1].type == CLIMB_FRONT) {
				deltaHeight += 1;
			}
			else if (cells[yEnd][x + 1].type == DESCENT_RIGHT) {
				deltaHeight += 2;
			}
			if (deltaHeight == 0 && (cells[yEnd][x].type == DESCENT_FRONT && cells[yEnd][x + 1].type == CLIMB_FRONT || cells[yEnd][x].type == CLIMB_FRONT && cells[yEnd][x + 1].type == DESCENT_FRONT))
				deltaHeight = 2;
			triangleCount += abs(deltaHeight);
		}
		for (auto y = yStart; y < yEnd; ++y) {

			deltaHeight = (cells[y + 1][xEnd].height - cells[y][xEnd].height) * 2;

			if (cells[y][xEnd].type == DESCENT_RIGHT || cells[y][xEnd].type == CLIMB_RIGHT) {
				deltaHeight -= 1;
			}
			else if (cells[y][xEnd].type == CLIMB_FRONT) {
				deltaHeight -= 2;
			}

			if (cells[y + 1][xEnd].type == DESCENT_RIGHT || cells[y + 1][xEnd].type == CLIMB_RIGHT) {
				deltaHeight += 1;
			}
			else if (cells[y + 1][xEnd].type == DESCENT_FRONT) {
				deltaHeight += 2;
			}
			if (deltaHeight == 0 && (cells[y][xEnd].type == DESCENT_RIGHT && cells[y + 1][xEnd].type == CLIMB_RIGHT || cells[y][xEnd].type == CLIMB_RIGHT && cells[y + 1][xEnd].type == DESCENT_RIGHT))
				deltaHeight = 2;
			triangleCount += abs(deltaHeight);
		}

		vertexGridSize = triangleCount * 3;
	}


	vertexGrid = new GLfloat[vertexGridSize * 6];
	auto verticesNum = 0;
	auto targetHeight = 0;
	auto currentHeight = 0;
	for (auto y = yStart; y <= yEnd; ++y)
	{
		for (auto x = xStart; x <= xEnd; ++x)
		{
			createFace(x - xCenter, y - yCenter, cells[y][x].height, cells[y][x].type, cells[y][x].type, vertexGrid + verticesNum);
			verticesNum += 36;

			if (cells[y][x].type == DESCENT_RIGHT || cells[y][x].type == CLIMB_RIGHT) {
				if (y > yStart) {
					if (cells[y][x].height < cells[y - 1][x].height || cells[y][x].height == cells[y - 1][x].height && cells[y - 1][x].type == CLIMB_FRONT) {
						createTriangle(x - xCenter, y - yCenter, cells[y][x].height, cells[y][x].type, BACKSIDE_UP, vertexGrid + verticesNum);
						verticesNum += 18;
					}
					else if (cells[y][x].height > cells[y - 1][x].height || cells[y][x].type != cells[y - 1][x].type) {
						createTriangle(x - xCenter, y - yCenter, cells[y][x].height, cells[y][x].type, BACKSIDE_DOWN, vertexGrid + verticesNum);
						verticesNum += 18;
					}
				}
				if (y < yEnd) {
					if (cells[y][x].height < cells[y + 1][x].height || cells[y][x].height == cells[y + 1][x].height && cells[y + 1][x].type == DESCENT_FRONT) {
						createTriangle(x - xCenter, y - yCenter, cells[y][x].height, cells[y][x].type, FRONTSIDE_UP, vertexGrid + verticesNum);
						verticesNum += 18;
					}
					else if (cells[y][x].height > cells[y + 1][x].height || cells[y][x].type != cells[y + 1][x].type) {
						createTriangle(x - xCenter, y - yCenter, cells[y][x].height, cells[y][x].type, FRONTSIDE_DOWN, vertexGrid + verticesNum);
						verticesNum += 18;
					}
				}
			}else if(cells[y][x].type == DESCENT_FRONT || cells[y][x].type == CLIMB_FRONT) {
				if (x > xStart) {
					if (cells[y][x].height < cells[y][x - 1].height || cells[y][x].height == cells[y][x - 1].height && cells[y][x - 1].type == CLIMB_RIGHT) {
						createTriangle(x - xCenter, y - yCenter, cells[y][x].height, cells[y][x].type,LEFTSIDE_UP, vertexGrid + verticesNum);
						verticesNum += 18;
					}
					else if (cells[y][x].height > cells[y][x - 1].height || cells[y][x].type != cells[y][x - 1].type) {
						createTriangle(x - xCenter, y - yCenter, cells[y][x].height, cells[y][x].type, LEFTSIDE_DOWN, vertexGrid + verticesNum);
						verticesNum += 18;
					}
				}
				if (x < xEnd) {
					if (cells[y][x].height < cells[y][x + 1].height || cells[y][x].height == cells[y][x + 1].height && cells[y][x + 1].type == DESCENT_RIGHT) {
						createTriangle(x - xCenter, y - yCenter, cells[y][x].height, cells[y][x].type, RIGHTSIDE_UP, vertexGrid + verticesNum);
						verticesNum += 18;
					}
					else if (cells[y][x].height > cells[y][x + 1].height || cells[y][x].type != cells[y][x + 1].type) {
						createTriangle(x - xCenter, y - yCenter, cells[y][x].height, cells[y][x].type, RIGHTSIDE_DOWN, vertexGrid + verticesNum);
						verticesNum += 18;
					}
				}
			}

			currentHeight = cells[y][x].height;
			if (cells[y][x].type == CLIMB_RIGHT)
				currentHeight += 1;

			if (x < xEnd) {
				targetHeight = cells[y][x + 1].height;
				if (cells[y][x + 1].type == DESCENT_RIGHT)
					targetHeight += 1;
				if (currentHeight < targetHeight) {
					if (cells[y][x].type == CLIMB_FRONT || cells[y][x].type == DESCENT_FRONT)
						currentHeight += 1;
				}
				else if (currentHeight > targetHeight) {
					if (cells[y][x + 1].type == CLIMB_FRONT || cells[y][x + 1].type == DESCENT_FRONT)
						targetHeight += 1;
				}
			}
			else {
				targetHeight = currentHeight;
			}

			if (currentHeight < targetHeight) {
				for (; currentHeight < targetHeight; ++currentHeight) {
					createFace(x - xCenter, y - yCenter, currentHeight, RIGHTSIDE_UP, RIGHTSIDE_UP, vertexGrid + verticesNum);
					verticesNum += 36;
				}
			}
			else {
				for (; currentHeight > targetHeight; --currentHeight) {
					createFace(x - xCenter, y - yCenter, currentHeight, RIGHTSIDE_DOWN, RIGHTSIDE_DOWN, vertexGrid + verticesNum);
					verticesNum += 36;
				}
			}


			currentHeight = cells[y][x].height;
			if (cells[y][x].type == CLIMB_FRONT)
				currentHeight += 1;

			if (y < yEnd) {
				targetHeight = cells[y + 1][x].height;
				if (cells[y + 1][x].type == DESCENT_FRONT)
					targetHeight += 1;
				if (currentHeight < targetHeight) {
					if (cells[y][x].type == CLIMB_RIGHT || cells[y][x].type == DESCENT_RIGHT)
						currentHeight += 1;
				}
				else if (currentHeight > targetHeight) {
					if (cells[y + 1][x].type == CLIMB_RIGHT || cells[y + 1][x].type == DESCENT_RIGHT)
						targetHeight += 1;
				}
			}
			else {
				targetHeight = currentHeight;
			}

			if (currentHeight < targetHeight) {
				for (; currentHeight < targetHeight; ++currentHeight) {
					createFace(x - xCenter, y - yCenter, currentHeight, FRONTSIDE_UP, FRONTSIDE_UP, vertexGrid + verticesNum);
					verticesNum += 36;
				}
			}
			else {
				for (; currentHeight > targetHeight; --currentHeight) {
					createFace(x - xCenter, y - yCenter, currentHeight, FRONTSIDE_DOWN, FRONTSIDE_DOWN, vertexGrid + verticesNum);
					verticesNum += 36;
				}
			}
		}
	}
	

	glGenVertexArrays(1, &VAO[VERTEX_GRID]);
	glGenBuffers(1, &VBO[VERTEX_GRID]);

	glBindVertexArray(VAO[VERTEX_GRID]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[VERTEX_GRID]);
	glBufferData(GL_ARRAY_BUFFER, vertexGridSize * 6 * sizeof(GLfloat), vertexGrid, GL_STATIC_DRAW);

	//position: x, y, z
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//texture coordinates: x, y
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	//type texture
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Render::bindPlayer(Player player) {
	objectShader.Use();
	
	createFace((GLfloat)player.x - 0.5, (GLfloat)player.y + 0.5, 0.001, 13, RIGHTSIDE_UP, vertexPlayer);

	glBindVertexArray(VAO[VERTEX_PLAYER]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[VERTEX_PLAYER]);
	glBufferData(GL_ARRAY_BUFFER, 36 * sizeof(GLfloat), vertexPlayer, GL_DYNAMIC_DRAW);

	//position: x, y, z
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//texture coordinates: x, y
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	//type texture
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

GLuint Render::bufferImage(const char* filename, const char* nameUniform, Shader& shader, const int numberTexture = 0)
{
	shader.Use();
	int width, height;
	unsigned char* image = SOIL_load_image(filename, &width, &height, 0, SOIL_LOAD_RGB);
	GLuint textureBufferID;
	glGenTextures(1, &textureBufferID);
	glActiveTexture(GL_TEXTURE0 + numberTexture);
	glBindTexture(GL_TEXTURE_2D, textureBufferID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glUniform1i(glGetUniformLocation(shader.Program, nameUniform), numberTexture);

	SOIL_free_image_data(image);
	//glBindTexture(GL_TEXTURE_2D, 0);

	return textureBufferID;
}

void Render::createFont(){
	//COPIED FROM: https://github.com/JoeyDeVries/LearnOpenGL/blob/master/src/7.in_practice/2.text_rendering/text_rendering.cpp

	interfaceShader.Use();

	FT_Library ft;
	// All functions return a value different than 0 whenever an error occurred
	if (FT_Init_FreeType(&ft))
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

	// Load font as face
	FT_Face face;
	if (FT_New_Face(ft, "fonts/calibri.ttf", 0, &face))
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;

	// Set size to load glyphs as
	FT_Set_Pixel_Sizes(face, 0, 48);

	// Disable byte-alignment restriction
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// Load first 128 characters of ASCII set
	for (GLubyte c = 0; c < 128; c++)
	{
		// Load character glyph 
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
			continue;
		}
		// Generate texture
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);
		// Set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// Now store character for later use
		Sumbol sumbol = {
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->glyph->advance.x
		};
		font.insert(std::pair<GLchar, Sumbol>(c, sumbol));
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	// Destroy FreeType once we're finished
	FT_Done_Face(face);
	FT_Done_FreeType(ft);


	// Configure VAO/VBO for texture quads
	glGenVertexArrays(1, &VAO[VERTEX_FONT]);
	glGenBuffers(1, &VBO[VERTEX_FONT]);
	glBindVertexArray(VAO[VERTEX_FONT]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[VERTEX_FONT]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void  Render::renderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color)
{
	//COPIED FROM: https://github.com/JoeyDeVries/LearnOpenGL/blob/master/src/7.in_practice/2.text_rendering/text_rendering.cpp

	interfaceShader.Use();
	glUniform3f(glGetUniformLocation(interfaceShader.Program, "textColor"), color.x, color.y, color.z);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(VAO[VERTEX_FONT]);

	// Iterate through all characters
	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		Sumbol ch = font[*c];

		GLfloat xpos = x + ch.Bearing.x * scale;
		GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

		GLfloat w = ch.Size.x * scale;
		GLfloat h = ch.Size.y * scale;
		// Update VBO for each character
		GLfloat vertices[6][4] = {
			{ xpos, ypos + h, 0.0, 0.0 },
			{ xpos, ypos, 0.0, 1.0 },
			{ xpos + w, ypos, 1.0, 1.0 },
			{ xpos, ypos + h, 0.0, 0.0 },
			{ xpos + w, ypos, 1.0, 1.0 },
			{ xpos + w, ypos + h, 1.0, 0.0 }
		};
		// Render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, ch.textureID);
		// Update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, VBO[VERTEX_FONT]);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// Render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		x += (ch.Advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64)
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}


void Render::createFace(GLfloat _x, GLfloat _y, GLfloat _z, GLfloat _type, int _rotate, GLfloat* arr) {
	switch (_rotate) {
	case HORIZONTAL:
		arr[30] = arr[0] = _x;
		arr[31] = arr[1] = _y + 1;
		arr[32] = arr[2] = _z;

		arr[6] = _x;
		arr[7] = _y;
		arr[8] = _z;

		arr[18] = arr[12] = _x + 1;
		arr[19] = arr[13] = _y;
		arr[20] = arr[14] = _z;

		arr[24] = _x + 1;
		arr[25] = _y + 1;
		arr[26] = _z;
		break;
	case RIGHTSIDE_UP:
	case CLIMB_RIGHT:
		arr[30] = arr[0] = _x + 1;
		arr[31] = arr[1] = _y + 1;
		arr[32] = arr[2] = _z + 1;

		arr[6] = _x + 1;
		arr[7] = _y + 1;
		arr[8] = _z;

		arr[18] = arr[12] = _x + 1;
		arr[19] = arr[13] = _y;
		arr[20] = arr[14] = _z;

		arr[24] = _x + 1;
		arr[25] = _y;
		arr[26] = _z + 1;
		break;
	case RIGHTSIDE_DOWN:
	case DESCENT_RIGHT:
		arr[30] = arr[0] = _x + 1;
		arr[31] = arr[1] = _y;
		arr[32] = arr[2] = _z;

		arr[6] = _x + 1;
		arr[7] = _y;
		arr[8] = _z - 1;

		arr[18] = arr[12] = _x + 1;
		arr[19] = arr[13] = _y + 1;
		arr[20] = arr[14] = _z - 1;

		arr[24] = _x + 1;
		arr[25] = _y + 1;
		arr[26] = _z;
		break;
	case FRONTSIDE_UP:
	case CLIMB_FRONT:
		arr[30] = arr[0] = _x;
		arr[31] = arr[1] = _y + 1;
		arr[32] = arr[2] = _z + 1;

		arr[6] = _x;
		arr[7] = _y + 1;
		arr[8] = _z;

		arr[18] = arr[12] = _x + 1;
		arr[19] = arr[13] = _y + 1;
		arr[20] = arr[14] = _z;

		arr[24] = _x + 1;
		arr[25] = _y + 1;
		arr[26] = _z + 1;
		break;
	case FRONTSIDE_DOWN:
	case DESCENT_FRONT:
		arr[30] = arr[0] = _x + 1;
		arr[31] = arr[1] = _y + 1;
		arr[32] = arr[2] = _z;

		arr[6] = _x + 1;
		arr[7] = _y + 1;
		arr[8] = _z - 1;

		arr[18] = arr[12] = _x;
		arr[19] = arr[13] = _y + 1;
		arr[20] = arr[14] = _z - 1;

		arr[24] = _x;
		arr[25] = _y + 1;
		arr[26] = _z;
		break;
	}
	switch (_rotate) {
	case CLIMB_RIGHT:
		arr[18] = arr[12] = arr[6] = _x;
		break;
	case DESCENT_RIGHT:
		arr[30] = arr[24] = arr[0] = _x;
		arr[32] = arr[26] = arr[2] = _z + 1;
		arr[20] = arr[14] = arr[8] = _z;
		break;
	case CLIMB_FRONT:
		arr[19] = arr[13] = arr[7] = _y;
		break;
	case DESCENT_FRONT:
		arr[31] = arr[25] = arr[1] = _y;
		arr[32] = arr[26] = arr[2] = _z + 1;
		arr[20] = arr[14] = arr[8] = _z;
		break;
	}

	//LEFT UP:
	arr[33] = arr[3] = 0;		//x
	arr[34] = arr[4] = 0;		//y

	//RIGHT UP:
	arr[27] = 1;	//x
	arr[28] = 0;	//y

	//LEFT DOWN:
	arr[9] = 0;		//x
	arr[10] = 1;	//y

	//RIGHT DOWN:
	arr[21] = arr[15] = 1;	//x
	arr[22] = arr[16] = 1;	//y

	//TYPES:
	/*if (_rotate == RIGHTSIDE_UP || _rotate == RIGHTSIDE_DOWN || _rotate == FRONTSIDE_UP || _rotate == FRONTSIDE_DOWN)
		arr[35] = arr[29] = arr[23] = arr[17] = arr[11] = arr[5] = _rotate;
	else*/
		arr[35] = arr[29] = arr[23] = arr[17] = arr[11] = arr[5] = _type;
}

void Render::createTriangle(GLfloat _x, GLfloat _y, GLfloat _z, GLfloat _type, int _rotate, GLfloat* arr) {

	if (_type == CLIMB_RIGHT || _type == CLIMB_FRONT) {
		switch (_rotate) {
		case FRONTSIDE_UP:
			arr[0] = _x + 1;
			arr[1] = _y + 1;
			arr[2] = _z + 1;

			arr[6] = _x;
			arr[7] = _y + 1;
			arr[8] = _z + 1;

			arr[12] = _x;
			arr[13] = _y + 1;
			arr[14] = _z;
			break;
		case FRONTSIDE_DOWN:
			arr[0] = _x + 1;
			arr[1] = _y + 1;
			arr[2] = _z + 1;

			arr[6] = _x + 1;
			arr[7] = _y + 1;
			arr[8] = _z;

			arr[12] = _x;
			arr[13] = _y + 1;
			arr[14] = _z;
			break;
		case RIGHTSIDE_UP:
			arr[0] = _x + 1;
			arr[1] = _y;
			arr[2] = _z;

			arr[6] = _x + 1;
			arr[7] = _y;
			arr[8] = _z + 1;

			arr[12] = _x + 1;
			arr[13] = _y + 1;
			arr[14] = _z + 1;
			break;
		case RIGHTSIDE_DOWN:
			arr[0] = _x + 1;
			arr[1] = _y;
			arr[2] = _z;

			arr[6] = _x + 1;
			arr[7] = _y + 1;
			arr[8] = _z;

			arr[12] = _x + 1;
			arr[13] = _y + 1;
			arr[14] = _z + 1;
			break;
		case BACKSIDE_UP:
			arr[0] = _x;
			arr[1] = _y;
			arr[2] = _z;

			arr[6] = _x;
			arr[7] = _y;
			arr[8] = _z + 1;

			arr[12] = _x + 1;
			arr[13] = _y;
			arr[14] = _z + 1;
			break;
		case BACKSIDE_DOWN:
			arr[0] = _x;
			arr[1] = _y;
			arr[2] = _z;

			arr[6] = _x + 1;
			arr[7] = _y;
			arr[8] = _z;

			arr[12] = _x + 1;
			arr[13] = _y;
			arr[14] = _z + 1;
			break;
		case LEFTSIDE_UP:
			arr[0] = _x;
			arr[1] = _y + 1;
			arr[2] = _z + 1;

			arr[6] = _x;
			arr[7] = _y;
			arr[8] = _z + 1;

			arr[12] = _x;
			arr[13] = _y;
			arr[14] = _z;
			break;
		case LEFTSIDE_DOWN:
			arr[0] = _x;
			arr[1] = _y + 1;
			arr[2] = _z + 1;

			arr[6] = _x;
			arr[7] = _y + 1;
			arr[8] = _z;

			arr[12] = _x;
			arr[13] = _y;
			arr[14] = _z;
			break;
		}
	}else{
		switch (_rotate) {
		case FRONTSIDE_UP:
			arr[0] = _x + 1;
			arr[1] = _y + 1;
			arr[2] = _z;

			arr[6] = _x + 1;
			arr[7] = _y + 1;
			arr[8] = _z + 1;

			arr[12] = _x;
			arr[13] = _y + 1;
			arr[14] = _z + 1;
			break;
		case FRONTSIDE_DOWN:
			arr[0] = _x + 1;
			arr[1] = _y + 1;
			arr[2] = _z;

			arr[6] = _x;
			arr[7] = _y + 1;
			arr[8] = _z;

			arr[12] = _x;
			arr[13] = _y + 1;
			arr[14] = _z + 1;
			break;
		case RIGHTSIDE_UP:
			arr[0] = _x + 1;
			arr[1] = _y;
			arr[2] = _z + 1;

			arr[6] = _x + 1;
			arr[7] = _y + 1;
			arr[8] = _z + 1;

			arr[12] = _x + 1;
			arr[13] = _y + 1;
			arr[14] = _z;
			break;
		case RIGHTSIDE_DOWN:
			arr[0] = _x + 1;
			arr[1] = _y;
			arr[2] = _z + 1;

			arr[6] = _x + 1;
			arr[7] = _y;
			arr[8] = _z;

			arr[12] = _x + 1;
			arr[13] = _y + 1;
			arr[14] = _z;
			break;
		case BACKSIDE_UP:
			arr[0] = _x;
			arr[1] = _y;
			arr[2] = _z + 1;

			arr[6] = _x + 1;
			arr[7] = _y;
			arr[8] = _z + 1;

			arr[12] = _x + 1;
			arr[13] = _y;
			arr[14] = _z;
			break;
		case BACKSIDE_DOWN:
			arr[0] = _x;
			arr[1] = _y;
			arr[2] = _z + 1;

			arr[6] = _x;
			arr[7] = _y;
			arr[8] = _z;

			arr[12] = _x + 1;
			arr[13] = _y;
			arr[14] = _z;
			break;
		case LEFTSIDE_UP:
			arr[0] = _x;
			arr[1] = _y + 1;
			arr[2] = _z;

			arr[6] = _x;
			arr[7] = _y + 1;
			arr[8] = _z + 1;

			arr[12] = _x;
			arr[13] = _y;
			arr[14] = _z + 1;
			break;
		case LEFTSIDE_DOWN:
			arr[0] = _x;
			arr[1] = _y + 1;
			arr[2] = _z;

			arr[6] = _x;
			arr[7] = _y;
			arr[8] = _z;

			arr[12] = _x;
			arr[13] = _y;
			arr[14] = _z + 1;
			break;
		}
	}

	if (_rotate == FRONTSIDE_UP || _rotate == BACKSIDE_DOWN) {
		arr[3] = arr[0] - _x;			//x
		arr[9] = arr[6] - _x;			//x
		arr[15] = arr[12] - _x;			//x
	}
	else if (_rotate == FRONTSIDE_DOWN || _rotate == BACKSIDE_UP) {
		arr[3] = 1 - arr[0] + _x;		//x
		arr[9] = 1 - arr[6] + _x;		//x
		arr[15] = 1 - arr[12] + _x;		//x
	}
	else if (_rotate == RIGHTSIDE_DOWN || _rotate == LEFTSIDE_UP) {
		arr[3] = arr[1] - _y;			//x
		arr[9] = arr[7] - _y;			//x
		arr[15] = arr[13] - _y;			//x
	}
	else{
		arr[3] = 1 - arr[1] + _y;		//x
		arr[9] = 1 - arr[7] + _y;		//x
		arr[15] = 1 - arr[13] + _y;		//x
	}
	arr[4] = 1 - arr[2] + _z;			//y
	arr[10] = 1 - arr[8] + _z;			//y
	arr[16] = 1 - arr[14] + _z;			//y
	//TYPES:
	arr[17] = arr[11] = arr[5] = _rotate;
}