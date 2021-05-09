#version 330 core
layout (location = 0) in vec3 _position;
layout (location = 1) in vec2 _texCoord;
layout (location = 2) in float _type;
uniform mat4 _transform;
out vec3 position;
out vec2 texCoord;
out float type;
void main()
{
	gl_Position = _transform * vec4(_position.x/10, _position.y/10, _position.z/10, 1.0);
	texCoord = _texCoord;
	position = _position;
	type = _type;
}