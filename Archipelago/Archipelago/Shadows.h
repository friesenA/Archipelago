#pragma once
#include "stdHeader.h"

const GLuint SHADOW_WIDTH = 1024;
const GLuint SHADOW_HEIGHT = 1024;

class Shadows
{
	GLuint shadowFB;
	GLuint shadowMapTexture;

public:
	Shadows();

	GLuint getFrameBuffer();

	~Shadows();

private:

	void setupFrameBuffer();

	void setupDepthTexture();
};

