#pragma once
#include "stdHeader.h"
class Texture
{
	GLuint tex;
	unsigned char* texture_image;
public:
	Texture();
	virtual ~Texture();
	void initTexture();
	void loadImage(const char * image_file, int width, int height);
	unsigned char* getTextureImage();
	void setTextureImage(unsigned char* texture_image);


};