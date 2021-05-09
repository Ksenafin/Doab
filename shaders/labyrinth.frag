#version 330 core

#define HORIZONTAL		0
#define RIGHTSIDE_UP	1
#define RIGHTSIDE_DOWN	2
#define FRONTSIDE_UP	3
#define FRONTSIDE_DOWN	4
#define DESCENT_RIGHT	5
#define DESCENT_FRONT	6
#define CLIMB_RIGHT		7
#define CLIMB_FRONT		8

in vec3 position;
in vec2 texCoord;
in float type;

out vec4 color;

uniform sampler2D textureBrick;
uniform sampler2D textureDescent;
uniform sampler2D textureFloor;
uniform sampler2D textureGrass;
uniform sampler2D textureUnder;

void main()
{

	if(type == HORIZONTAL){
		if(position.z == 0)
			color = texture(textureGrass, texCoord);
		else if(position.z < 0)
			color = texture(textureUnder, texCoord);
		else
			color = texture(textureFloor, texCoord);
	}else if(type == DESCENT_RIGHT || type == DESCENT_FRONT || type == CLIMB_RIGHT || type == CLIMB_FRONT)
		color = texture(textureDescent, texCoord);
	else
		color = texture(textureBrick, texCoord);
		//color = vec4(texCoord, texCoord);

	//color = vec4(0.3f, 0.3f, 0.5f, 1.0f);
	/*if(condition >= 0.5f){
		color = vec4(condition.x, 0.5f, 0.2f, 1.0f);
	}else if(condition < 0.5f){
		color = vec4(condition.x, 0.8f, 0.6f, 1.0f);
	}else{
		color = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	}*/
}