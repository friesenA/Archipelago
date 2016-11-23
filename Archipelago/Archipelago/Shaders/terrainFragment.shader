#version 330 core

in vec3 fragmentNormal;
in vec3 fragmentPos;

out vec4 color;

uniform vec3 lightDirection;
uniform vec3 lightColor;
uniform vec3 viewerPos;

uniform sampler2D shadowTexture;

void main()
{
	vec3 landColor = vec3(0.0, 1.0, 0.0);

	//ambient lighting
	float ambientStrength = 0.05f;
    	vec3 ambient_contribution = ambientStrength * lightColor;

	//diffuse lighting
	float incident_degree = max(dot(fragmentNormal, lightDirection), 0.0f);
	vec3 diffuse_contribution = incident_degree * lightColor;

	//specular lighting
	float specularStrength = 1.0f;
	vec3 viewDir = normalize(viewerPos - fragmentPos);
	vec3 reflectDir = normalize(reflect(-lightDirection, fragmentNormal));
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16);
	vec3 specular_contribution = specularStrength * spec * lightColor;  

	vec3 finalColor = (ambient_contribution + diffuse_contribution + specular_contribution) * landColor;

	color = vec4(finalColor, 1.0f);
}