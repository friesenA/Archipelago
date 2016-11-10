#pragma once
#include "stdHeader.h"

// Dimensions of terrain plane
const int TERRAIN_WIDTH = 600;
const int TERRAIN_LENGTH = 600;

const int WATER_BORDER = 100;

//Currently: Creates a plane at y=0, with given width and length, centered at the origin
class Terrain
{	
	unsigned int seed; // random input for the procedural generator
	int width;
	int length;

	GLuint VAO, vertex_VBO, normals_VBO, index_EBO;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec3> indicies;

public:
	
	//Constructor initializes the terrain for rendering
	Terrain(unsigned int seed = 1);

	//Public access to VAO for rendering
	GLuint getVAO();

	~Terrain();

private:

	//Generates plane verticies and loads into VBO
	void buildVertexVBO();

	//Generates VBO of normals for each vertex
	void buildNormalsVBO();

	//Generates triangle indicies and loads into EBO
	void buildIndexEBO();

	//Loads VAO with all terrain VBO's, EBO's
	void buildVAO();

	//Modifies vertex height values to form a random number of cone style extrusions
	void islandMask();
};

