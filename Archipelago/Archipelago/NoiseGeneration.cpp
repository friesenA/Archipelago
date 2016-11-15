/*CODE FROM:
https:// solarianprogramer.com/2012/07/18/perlin-noise-cpp-11/
https:// www.youtube.com/watch?v=yNYwZMmgTJk
https:// flafla2.github.io/2014/08/09/perlinnoise.html
*/

#include "NoiseGeneration.h"

#include <math.h>
# define M_PI  3.14159265358979323846  /* pi */

NoiseGeneration::NoiseGeneration() {
	//srand(time(0));
	//this->seed = random_number;
}

float NoiseGeneration::generateHeight(int x, int z) {
	//this should be returning a vector of vertices of y values?


	//More general function:
	float total = 0;
	float d = (float)glm::pow(2, OCTAVES - 1);
	for (int i = 0; i < OCTAVES; i++) {
		float frequency = (float)(glm::pow(2, 1) / d);
		float amplitude = (float)glm::pow(ROUGHNESS, i)*AMPLITUDE;
		total += (getInterpolatedNoise(x*frequency, z*frequency)*amplitude);
	}
	return total;
}

//Random Number generator 
//returns a random number value btwn 1 and -1
float NoiseGeneration::getNoise(int x, int z) {
	

	x = rand()%10;
	z = rand() % 10;
	return x + z;

	//float setRandomSeed = x* 49632 + z* 325176 + random_number; //+ seed;
	//return setRandomSeed; //...
}

float NoiseGeneration::getSmoothNoise(int x, int z)
{
	float corners = (getNoise(x - 1, z - 1) + getNoise(x + 1, z - 1) + getNoise(x - 1, z + 1) + getNoise(x + 1, z + 1)) / 1600.0f; //16.0f is a scaling factor
	float sides = (getNoise(x - 1, z) + getNoise(x + 1, z) + getNoise(x, z - 1) + getNoise(x, z + 1)) / 800.0f;
	float center = (getNoise(x, z)) / 400.0f;
	return corners + sides + center;
}

//cosine interpolations betwen the noise values
float NoiseGeneration::interpolation(float a, float b, float blend) {
	double theta = blend * M_PI;
	float f = (float)(1.0f - glm::cos(theta))* 0.5f; //want a value bten -1 and 1
	return a*(1.0f - f) + b*f;
}

float NoiseGeneration::getInterpolatedNoise(float x, float z)
{
	int intX = (int)x; //interger part of value x
	int intZ = (int)z; //interger part of value z

	int fracX = x; //fraction/decimal section of the x value
	int fracZ = z; //fraction/decimal section of the z value

	 //calutating the height of the nearest points to the noise function
	float v1 = getSmoothNoise(intX, intZ);
	float v2 = getSmoothNoise(intX + 1, intZ);
	float v3 = getSmoothNoise(intX, intZ + 1);
	float v4 = getSmoothNoise(intX + 1, intZ + 1);

	//Interpolate betwen the heights 
	float i_1 = interpolation(v1, v2, fracX);
	float i_2 = interpolation(v3, v4, fracX);

	return interpolation(i_1, i_2, fracZ);
}



void buildVertexVBO() {


	/*
	GLfloat half = (GLfloat)this->numTiles / 2.0f;
	//Creates a plane at y=height, with given width and length, centered at the origin
	for (int l = 0; l <= this->numTiles; l++) {
	for (int w = 0; w <= this->numTiles; w++) {
	this->vertices.push_back(glm::vec3((GLfloat)(w * this->tileSize - half * this->tileSize), this->height, (GLfloat)(l * this->tileSize - half * this->tileSize)));
	}
	}

	glGenBuffers(1, &this->vertex_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, this->vertex_VBO);
	glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(glm::vec3), &this->vertices.front(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	*/
}

void buildVAO()
{

	/*
	//Bind VAO
	glGenVertexArrays(1, &this->VAO);
	glBindVertexArray(this->VAO);

	//Register Vertex Buffer
	glBindBuffer(GL_ARRAY_BUFFER, this->vertex_VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//Bind EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->index_EBO);

	//Unbind VBO & VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	*/
}


NoiseGeneration::~NoiseGeneration() {}
