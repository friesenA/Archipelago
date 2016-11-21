#pragma once
#include "stdHeader.h"
#include "NoiseGeneration.h"
#include "Obj.h"

// Dimensions of terrain plane
const int TERRAIN_WIDTH = 100; //600
const int TERRAIN_LENGTH = 100; //600

const int WATER_BORDER = 100;

//const int Island_Noise = ###; need to add?

//Currently: Creates a plane at y=0, with given width and length, centered at the origin
class Terrain : public Obj
{	
	unsigned int seed; // random input for the procedural generator
	int width;
	int length;

	GLuint VAO, vertex_VBO, normals_VBO, index_EBO;
	std::vector<glm::vec3> vertices;
	std::vector<GLuint> indicies;
	std::vector<glm::vec3> normals;

public:
	
	//Constructor initializes the terrain for rendering
	Terrain(unsigned int seed = 1);

	//Public access for rendering
	void draw();

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

	//function that goes through the  vertices lenght and pulls the x and z from each vec3
	//and then call the generateHeight(x , z) and then assign it to the y in the vec3 of vertices
	//place thos into vector
	void useNoise();
};

