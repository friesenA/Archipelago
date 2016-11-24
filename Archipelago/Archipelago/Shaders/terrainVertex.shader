#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uvCoord;

out vec3 fragmentNormal;
out vec3 fragmentPos;
out vec4 fragmentPosLightSpace;
out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix;


void main()
{
	gl_Position = projection * view * model * vec4(position, 1.0);
	fragmentPos = vec3(model * vec4(position, 1.0));
	fragmentNormal = vec3(model * vec4(normal, 0.0));
	fragmentPosLightSpace = lightSpaceMatrix * vec4(fragmentPos, 1.0f);
	TexCoord = uvCoord;
}