#pragma once
#include <GL/glew.h>
// GLFW
#include <glm/glm.hpp>
struct Sumbol {
	GLuint textureID;   // ID handle of the glyph texture
	glm::ivec2 Size;    // Size of glyph
	glm::ivec2 Bearing;  // Offset from baseline to left/top of glyph
	GLuint Advance;    // Horizontal offset to advance to next glyph
};