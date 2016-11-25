#version 330 core

in vec3 fragmentNormal;
in vec3 fragmentPos;
in vec4 fragmentPosLightSpace;
in vec2 TexCoord;

out vec4 color;

uniform vec3 lightDirection;
uniform vec3 lightColor;
uniform vec3 viewerPos;

uniform sampler2D shadowTexture;
uniform sampler2D terrainTexture;

float calculateShadow()
{	
	//normalized coordinates [0,1]
	vec3 projCoords = fragmentPosLightSpace.xyz / fragmentPosLightSpace.w;
	projCoords = projCoords * 0.5 + 0.5;
	
	//check if fragment rendered on map or not
	float mapDepth = texture(shadowTexture, projCoords.xy).r; 
	float fragmentDepth = projCoords.z;
	//float bias = max(0.5 * (1.0 - dot(fragmentNormal, lightDirection)), 0.005);
	float bias = 0.005f;
	float shadow = fragmentDepth - bias > mapDepth  ? 1.0 : 0.0;

	return (1.0f - shadow);
}	

void main()
{
	vec3 landColor = vec3(0.0, 0.3, 0.0);

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

	//Shadow
	//float shadow = calculateShadow();

	vec3 finalColor = (ambient_contribution + 1 * (diffuse_contribution + specular_contribution)) * landColor;

	color = texture(terrainTexture, TexCoord)*vec4(finalColor, 1.0f);
}