#pragma once
#include "stdHeader.h"
#include "Obj.h"

const int TEXTURE_SIZE = 50;
const int NUMBER_OF_TILES_ACROSS = 40;

//Creates a plane at y=height, of square dimension based on the size of the texture file and the number of tiles across, centered at the origin
class Water : public Obj
{
	GLfloat height;
	int numTiles;
	int tileSize;
	GLuint waterTexture;
	
	GLuint VAO, vertex_VBO, normals_VBO, uv_VBO, index_EBO;
	std::vector<glm::vec3> vertices;
	std::vector<GLuint> indicies;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> uvCoordinates;

public:
	//Constructor initializes the water plane for rendering
	Water(GLfloat height);

	//Public access for rendering
	void draw();

	float getWidth() { return (tileSize*(numTiles / 2)); };
	float getLength() { return (tileSize*(numTiles / 2)); };
	void incrementSurface(float scale = 1.0) { 
		numTiles *= scale; 
	};

	~Water();

private:

	//Generates plane verticies and loads into VBO
	void buildVertexVBO();

	//Generates VBO of normals for each vertex
	void buildNormalsVBO();

	//Generates UV coordinates and loads into VBO
	void buildUVVBO();

	//Generates triangle indicies and loads into EBO
	void buildIndexEBO();

	//Loads VAO with all water VBO's, EBO's
	void buildVAO();

	void buildTexture();
};

