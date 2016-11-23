#pragma once
#include "stdHeader.h"
#include "Obj.h"

const GLuint SHADOW_WIDTH = 2048;
const GLuint SHADOW_HEIGHT = 2048;

class Shadows
{
	GLuint shadowFB;
	GLuint shadowMapTexture;
	GLint lightLoc;
	glm::mat4 lightSpaceMatrix;


public:
	Shadows();

	void setupFrameBuffer();

	void initializeShadowMap();

	void drawObj(Obj *mesh, glm::vec3 lightDir);

	void endShadowMap();

	GLuint getShadowMapTexture();

	~Shadows();

private:

	void setupDepthTexture();

	void setupLightSpaceMatrix(glm::vec3 lightDir);
};

