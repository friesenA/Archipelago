#include "Terrain.h"


Terrain::Terrain(int seed = 1) : width(TERRAIN_WIDTH), length(TERRAIN_LENGTH)
{
	//build vertex vbo
	buildVertexVBO();
	//build index ebo
	buildIndexEBO();
	//build VAO
	buildVAO();
}

GLuint Terrain::getVAO()
{
	return VAO;
}

Terrain::~Terrain(){}


void Terrain::buildVertexVBO()
{
	int halfWidth = width / 2;
	int halfLength = length / 2;

	//Creates a plane at y=0, with given width and length, centered at the origin
	for (int l = halfLength; l <= halfLength; l++) {
		for (int w = halfWidth; w <= halfWidth; w++) {
			vertices.push_back(glm::vec3((GLfloat)w, 0.0f, (GLfloat)l));
		}
	}
	//Modify y values with perlin noise?
	//Modify y values with island creation mask?

	glGenBuffers(1, &vertex_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices.front(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Terrain::buildIndexEBO()
{
	for (int l = 0; l < length-1; l++) {
		for (int w = 0; w < width-1; w++) {
			GLuint point = l*width + w;

			indicies.push_back(glm::vec3(point, point + width, point + width + 1));
			indicies.push_back(glm::vec3(point, point + width + 1, point + 1));
		}
	}

	glGenBuffers(1, &index_EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicies.size() * sizeof(glm::vec3), &indicies.front(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Terrain::buildVAO()
{
	//Bind VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//Register Vertex Buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertex_VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//Register any other VBOs
	
	//Bind EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_EBO);

	//Unbind VBO & VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
