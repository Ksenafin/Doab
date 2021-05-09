#version 330 core

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
#define PLAYER			13

in vec3 position;
in vec2 texCoord;
in float type;

out vec4 color;

uniform sampler2D textureBrick;
uniform sampler2D textureDescent;
uniform sampler2D textureFloor;
uniform sampler2D textureGrass;
uniform sampler2D textureUnder;
uniform sampler2D texturePlayer;

void main()
{
	
	if(type == PLAYER){
		color = texture(texturePlayer, texCoord);
	}else if(type == HORIZONTAL){
		if(position.z == 0)
			color = texture(textureGrass, texCoord);
		else if(position.z == 14)
			color = texture(textureUnder, texCoord);
		else
			color = texture(textureFloor, texCoord);
	}else if(type == DESCENT_RIGHT || type == DESCENT_FRONT || type == CLIMB_RIGHT || type == CLIMB_FRONT)
		color = texture(textureDescent, texCoord);
	else
		color = texture(textureBrick, texCoord);

}