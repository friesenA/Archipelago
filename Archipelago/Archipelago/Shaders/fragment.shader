#version 330 core
in vec3 fragShaderColor;
out vec4 color;
void main()
{
	color = vec4(fragShaderColor, 1.0f);
}