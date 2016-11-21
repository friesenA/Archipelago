#version 330 core

in vec3 vertexNormal;
in vec3 fragmentPos;

out vec4 color;

uniform vec3 lightDirection;
uniform vec3 lightColor;
uniform vec3 viewerPos;

void main()
{
	vec3 waterColor = vec3(0.0, 0.0, 1.0);

	//ambient lighting
	float ambientStrength = 0.05f;
    	vec3 ambient_contribution = ambientStrength * lightColor;

	//diffuse lighting
	float incident_degree = max(dot(vertexNormal, lightDirection), 0.0f);
	vec3 diffuse_contribution = incident_degree * lightColor;

	//specular lighting
	vec3 viewDir = normalize(viewerPos - fragmentPos);
	vec3 reflectDir = reflect(-lightDirection, vertexNormal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64);
	vec3 specular_contribution = spec * lightColor;  

	vec3 finalColor = (ambient_contribution + diffuse_contribution + specular_contribution) * waterColor;

	color = vec4(finalColor, 1.0f);
}