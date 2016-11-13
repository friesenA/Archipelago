#pragma once
#include "stdHeader.h"

const int TEXTURE_SIZE = 1;
const int NUMBER_OF_TILES_ACROSS = 200;

//Creates a plane at y=height, of square dimension based on the size of the texture file and the number of tiles across, centered at the origin
class Water
{
	GLfloat height;
	int numTiles;
	int tileSize;
	GLuint indiceSize;
	
	GLuint VAO, vertex_VBO, uv_VBO, index_EBO;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> indicies;
	std::vector<glm::vec2> uvCoordinates;

public:
	//Constructor initializes the water plane for rendering
	Water(GLfloat height);

	//Public access to VAO for rendering
	GLuint getVAO();

	GLuint getNumIndices() { return indiceSize; };

	~Water();

private:

	//Generates plane verticies and loads into VBO
	void buildVertexVBO();

	//Generates UV coordinates and loads into VBO
	void buildUVVBO();

	//Generates triangle indicies and loads into EBO
	void buildIndexEBO();

	//Loads VAO with all water VBO's, EBO's
	void buildVAO();

	void fooTriangle();
};

