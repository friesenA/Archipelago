#version 330 core
layout (location = 0) in vec3 position;
out vec3 TexCoords;

uniform mat4 projection;
uniform mat4 view;


//Testing out fog stuff
/*
	out float visibility;
	const float density = 0.007f;
	const float gradient  = 1.5f
*/
void main()
{
    gl_Position =   projection * view * vec4(position, 1.0);  

	TexCoords = position;

	//Fog stuff:
	/*
	vec4 positionReltoCam = view * gl_Position;
	float distance = lenght(positionReltoCam.xyz);
	visibility = exp(-pow((distance * density), gradient));
	visibility = clamp(visility, 0.0, 1.0);*/
}  