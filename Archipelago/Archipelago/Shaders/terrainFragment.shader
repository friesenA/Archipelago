#version 330 core

in vec3 vertexNormal;

out vec4 color;

void main()
{
	vec3 landColor = vec3(0.0, 1.0, 0.0);
	vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);
	vec3 lightPosition = vec3(0.0f, 500.0f, 0.0f); //world coords (overhead)
	vec3 lightDirection = normalize(lightPosition - vec3(0.0)); //directional light

	//ambient lighting
	float ambientStrength = 0.15f;
    	vec3 ambient_contribution = ambientStrength * lightColor;

	//diffuse lighting
	float incident_degree = max(dot(vertexNormal, lightDirection), 0.0f);
	vec3 diffuse_contribution = incident_degree * lightColor;

	vec3 finalColor = (ambient_contribution + diffuse_contribution) * landColor;

	color = vec4(finalColor, 1.0f);
}