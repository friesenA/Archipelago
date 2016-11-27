#version 330 core
in vec3 TexCoords;
out vec4 color;

uniform samplerCube skybox;

//Fog Stuff:
/*
	in float visibility;
	out vec3 skyColour;
*/

void main()
{    
    color = texture(skybox, TexCoords);

	//Fog stuff:
	/*
		skyColour = vec3(0.5f,0.5f,0.5f);
		//color = mix(vec4(skyColour, 1.0), color, visibility);
		color = visibility*skyColour + (1-visibility) * color;
	*/
}