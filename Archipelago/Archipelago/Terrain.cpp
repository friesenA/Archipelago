#include "Terrain.h"
#include "NoiseGeneration.h"

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

int Terrain::getNumIndices()
{
	return indicies.size();
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
	this->useNoise();
		
	//Modify y values with island mask
		//this->islandMask();

	glGenBuffers(1, &this->vertex_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, this->vertex_VBO);
	glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(glm::vec3), &this->vertices.front(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Terrain::buildIndexEBO()
{
	for (int l = 0; l < this->length-1; l++) {
		for (int w = 0; w < this->width-1; w++) {
			GLuint point = l*width + w;

			//first half triangle
			indicies.push_back(point);
			indicies.push_back(point + (this->width + 1));
			indicies.push_back(point + (this->width + 1) + 1);
			//second half triangle
			indicies.push_back(point);
			indicies.push_back(point + (this->width + 1) + 1);
			indicies.push_back(point + 1);
		}
	}
	
	glGenBuffers(1, &this->index_EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->index_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indicies.size() * sizeof(GLuint), &this->indicies.front(), GL_STATIC_DRAW);
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

	//Register any other VBOs
	
	//Bind EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->index_EBO);

	//Unbind VBO & VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Terrain::islandMask()
{	
	const GLfloat MAGNITUDE = 0.2f;
	const GLfloat DISTANCE_TO_ZERO = 100;

	int numCentrePoints;
	int centreXCoord, centerZCoord;
	GLfloat vertex, distance;

	//generate # of centre points
		//rule: Any value between 1-5 inclusive
	numCentrePoints = 1 + rand() % 10;

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

//function that goes through the  vertices lenght and pulls the x and z from each vec3
//and then call the generateHeight(x , z) and then assign it to the y in the vec3 of vertices
//place thos into vector
void Terrain::useNoise() {
	

	for (int i =0; i < this->vertices.size(); i++) {
		
		NoiseGeneration noise;
		vertices[i].y = (noise.generateHeight(vertices[i].x, vertices[i].z))-7;		

	}

}
