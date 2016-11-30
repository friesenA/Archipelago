#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uvCoord;

out vec3 fragmentNormal;
out vec3 fragmentPos;
out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float time;

void main()
{
	//Waves
	float ampOfWave = 0.050f;
	float period = 0.1f;
	float frequency = 3*2*3.1415;
	float height = position.y * ampOfWave * sin(period*position.x - frequency*time);
	vec3 newPosition = vec3(position.x, position.y + height, position.z);	

	gl_Position = projection * view * model * vec4(newPosition, 1.0);
	
	//fragment shader information
	fragmentPos = vec3(model * vec4(newPosition, 1.0));
	fragmentNormal = vec3(model*vec4(normal, 0.0));
	TexCoord = uvCoord;
}