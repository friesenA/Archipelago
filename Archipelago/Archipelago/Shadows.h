#pragma once
#include "stdHeader.h"
#include "Obj.h"

const GLuint SHADOW_WIDTH = 1024;
const GLuint SHADOW_HEIGHT = 1024;

class Shadows
{
	GLuint shadowFB;
	GLuint shadowMapTexture;
	GLint lightLoc;
	glm::mat4 lightSpaceMatrix;


public:
	Shadows();

	void initializeShadowMap();

	void drawObj(Obj *mesh, glm::vec3 lightDir);

	void endShadowMap();

	GLuint getShadowMapTexture();


	~Shadows();

private:

	void setupFrameBuffer();

	void setupDepthTexture();

	void setupLightSpaceMatrix(glm::vec3 lightDir);
};

