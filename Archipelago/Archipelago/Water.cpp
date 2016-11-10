#include "Water.h"


Water::Water(GLfloat height) : numTiles(NUMBER_OF_TILES_ACROSS), tileSize(TEXTURE_SIZE)
{
	std::cout << "Generating Water" << std::endl;

	//build VBO's
	buildVertexVBO();
	buildNormalsVBO();
	buildUVVBO();
	//build EBO
	buildIndexEBO();
	//build VAO
	buildVAO();
}

GLuint Water::getVAO()
{
	return VAO;
}

Water::~Water() {}

void Water::buildVertexVBO()
{
	GLfloat half = numTiles / 2;

	//Creates a plane at y=height, with given width and length, centered at the origin
	for (int l = 0; l < numTiles; l++) {
		for (int w = 0; w < numTiles; w++) {
			vertices.push_back(glm::vec3((GLfloat)(w * tileSize - half * tileSize), height, (GLfloat)(half * tileSize - l * tileSize)));
		}
	}

	glGenBuffers(1, &vertex_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices.front(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Water::buildNormalsVBO()
{	
	//Water is a flat plane in x and z, all normals are in the positive y direction
	for (int i = 0; i < vertices.size(); i++) {
		normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
	}

	glGenBuffers(1, &normals_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, normals_VBO);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals.front(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Water::buildUVVBO() {

	//generates uv coordinates for tiled texture settings
	for (int i = 0; i <= numTiles; i++) {
		for (int j = 0; j <= numTiles; j++) {
			uvCoordinates.push_back(glm::vec2((GLfloat)i, (GLfloat)j));
		}
	}

	glGenBuffers(1, &uv_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, uv_VBO);
	glBufferData(GL_ARRAY_BUFFER, uvCoordinates.size() * sizeof(glm::vec2), &uvCoordinates.front(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Water::buildIndexEBO()
{
	for (int i = 0; i < numTiles; i++) {
		for (int j = 0; j < numTiles; j++) {
			GLuint point = i*(numTiles+1) + j;

			indicies.push_back(glm::vec3(point, point + (numTiles+1), point + (numTiles + 1) + 1));
			indicies.push_back(glm::vec3(point, point + (numTiles+1) + 1, point + 1));
		}
	}

	glGenBuffers(1, &index_EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicies.size() * sizeof(glm::vec3), &indicies.front(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Water::buildVAO()
{
	//Bind VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//Register Vertex Buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertex_VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//Register Normals Buffer
	glBindBuffer(GL_ARRAY_BUFFER, normals_VBO);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);

	//Register UV Buffer
	glBindBuffer(GL_ARRAY_BUFFER, uv_VBO);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(2);

	//Register any other VBOs

	//Bind EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_EBO);

	//Unbind VBO & VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}