#pragma once
#include "stdHeader.h"

// Dimensions of terrain plane: Must be even values
const int TERRAIN_WIDTH = 600;
const int TERRAIN_LENGTH = 600;

class Terrain
{
public:
	int seed;
	int width;
	int length;
	
	GLuint VAO, vertex_VBO, index_EBO;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> indicies;

	//Constructor initializes the terrain for rendering
	Terrain(int seed);

	//Public access to VAO for rendering
	GLuint getVAO();

	~Terrain();

private:

	//Generates plane verticies and loads into VBO
	void buildVertexVBO();

	//Generates triangle indicies and loads into EBO
	void buildIndexEBO();

	//Loads VAO with all terrain VBO's, EBO's
	void buildVAO();
};

