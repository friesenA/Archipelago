#include "Terrain.h"


Terrain::Terrain(unsigned int seed) : width(TERRAIN_WIDTH), length(TERRAIN_LENGTH)
{
	std::cout << "Generating Terrain" << std::endl;
	srand(seed);

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
	GLfloat halfWidth = this->width / 2;
	GLfloat halfLength = this->length / 2;

	//Creates a plane at y=0, with given width and length, centered at the origin
	for (int l = 0; l < this->length; l++) {
		for (int w = 0; w < this->width; w++) {
			this->vertices.push_back(glm::vec3((GLfloat)w - halfWidth, 0.0f, halfLength - (GLfloat)l));
		}
	}
	//Modify y values with perlin noise?
	//Modify y values with island mask
	this->islandMask();

	glGenBuffers(1, &this->vertex_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, this->vertex_VBO);
	glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(glm::vec3), &this->vertices.front(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Terrain::buildNormalsVBO()
{
	GLfloat vertex;

	//Find normals. Reference: https://stackoverflow.com/questions/6656358/calculating-normals-in-a-triangle-mesh
	for (int l = 0; l < this->length - 1; l++) {
		for (int w = 0; w < this->width - 1; w++) {
			vertex = l * this->width + w;


		}
	}

	glGenBuffers(1, &this->normals_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, this->normals_VBO);
	glBufferData(GL_ARRAY_BUFFER, this->normals.size() * sizeof(glm::vec3), &this->normals.front(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Terrain::buildIndexEBO()
{
	GLuint vertex;

	for (int l = 0; l < this->length-1; l++) {
		for (int w = 0; w < this->width-1; w++) {
			vertex = l*width + w;

			this->indicies.push_back(glm::vec3(vertex, vertex + this->width, vertex + this->width + 1));
			this->indicies.push_back(glm::vec3(vertex, vertex + this->width + 1, vertex + 1));
		}
	}

	glGenBuffers(1, &this->index_EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->index_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indicies.size() * sizeof(glm::vec3), &this->indicies.front(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Terrain::buildVAO()
{
	//Bind VAO
	glGenVertexArrays(1, &this->VAO);
	glBindVertexArray(this->VAO);

	//Register Vertex Buffer
	glBindBuffer(GL_ARRAY_BUFFER, this->vertex_VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//Register Normals Buffer
	glBindBuffer(GL_ARRAY_BUFFER, this->normals_VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//Register any other VBOs
	
	//Bind EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->index_EBO);

	//Unbind VBO & VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

//modified implementation of concept, Reference: https://www.reddit.com/r/gamedev/comments/1g4eae/need_help_generating_an_island_using_perlin_noise/?st=iuritk3l&sh=594f7e28
void Terrain::islandMask()
{	
	const GLfloat MAGNITUDE = 30.0f;
	const GLfloat DISTANCE_TO_ZERO = 100;

	int numCentrePoints;
	int centreXCoord, centerZCoord;
	GLfloat vertex, distance;

	//generate # of centre points
		//rule: Any value between 1-5 inclusive
	numCentrePoints = 1 + rand() % 5;

	//generate centre point position for each # of points
		//rule: cannot select points within X distance of the edge
		//x coordinate = X + random # between 0 and width - 2X
		//z coordinate = X + random # between 0 and length - 2X

	for (int i = 0; i < numCentrePoints; i++) {
		centreXCoord = WATER_BORDER + rand() % (this->width - 2 * WATER_BORDER);
		centerZCoord = WATER_BORDER + rand() % (this->length - 2 * WATER_BORDER);

		//Adjust height values in array for each centre point
		//Add distance * magnitude to the height values (y) of all the verticies vector/array
		for (int l = 0; l < this->length; l++) {
			for (int w = 0; w < this->width; w++) {
				vertex = l * this->width + w;
				distance = std::sqrt(std::pow(centreXCoord - w, 2) + std::pow(centreXCoord - l, 2));
				
				//clamp if outside sloping zone;
				if (distance > DISTANCE_TO_ZERO)
					distance = DISTANCE_TO_ZERO;

				this->vertices[vertex].y += (DISTANCE_TO_ZERO - distance) * MAGNITUDE;
			}
		}
	}
}
