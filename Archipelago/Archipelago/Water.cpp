#include "Water.h"


Water::Water(GLfloat height) : numTiles(NUMBER_OF_TILES_ACROSS), tileSize(TEXTURE_SIZE), height(height)
{
	std::cout << "Generating Water" << std::endl;

	//build vertex vbo
	buildVertexVBO();
	//build index ebo
	buildIndexEBO();
	//build VAO
	buildVAO();
	//fooTriangle();
}

GLuint Water::getVAO()
{
	return VAO;
}

Water::~Water() {}

void Water::buildVertexVBO() {
	GLfloat half = (GLfloat)numTiles / 2.0f;
	//Creates a plane at y=height, with given width and length, centered at the origin
	for (int l = 0; l <= numTiles; l++) {
		for (int w = 0; w <= numTiles; w++) {
			vertices.push_back(glm::vec3((GLfloat)(w * tileSize - half * tileSize),height, (GLfloat)(l * tileSize - half * tileSize)));
		}
	}

	glGenBuffers(1, &vertex_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices.front(), GL_STATIC_DRAW);
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
	std::vector<GLuint> indices;
	for (int i = 0; i < this->numTiles; i++) {
		for (int j = 0; j < this->numTiles; j++) {
			GLuint point = i * (this->numTiles + 1) + j;

			//first half triangle
			indices.push_back(point);
			indices.push_back(point + (this->numTiles + 1));
			indices.push_back(point + (this->numTiles + 1) + 1);
			//second half triangle
			indices.push_back(point);
			indices.push_back(point + (this->numTiles + 1) + 1);
			indices.push_back(point + 1);
		}
	}
	indiceSize = indices.size();
	glGenBuffers(1, &index_EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices.front(), GL_STATIC_DRAW);
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

	//Bind EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_EBO);

	//Unbind VBO & VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

//// Test Triangle ///
void Water::fooTriangle() {
	GLfloat vertices[] = {
		50.0f,  -0.5f, 50.0f,  // Top Right
		50.0f, -0.5f, -50.0f,  // Bottom Right
		-50.0f, -0.5f, 50.0f,  // Bottom Left
		-50.0f,  -0.5f, -50.0f   // Top Left
	};
	GLuint indices[] = {  // Note that we start from 0!
		0, 1, 3,  // First Triangle
		1, 2, 3   // Second Triangle
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &vertex_VBO);
	glGenBuffers(1, &index_EBO);
	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, vertex_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind

	glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs), remember: do NOT unbind the EBO, keep it bound to this VAO
}