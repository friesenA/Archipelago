#pragma once
#include "stdHeader.h"
#include <cstdlib>
#include <ctime>
#include <random>



	const int NOISE_WIDTH = 20;
	const int NOISE_HEIGH = 30;

class NoiseGeneration
{

public:

	NoiseGeneration();

	

	float generateHeight(int x, int z);
	float getNoise(int x, int z);
	float getSmoothNoise(int x, int z);
	float interpolation(float a, float b, float blend);
	float getInterpolatedNoise(float x, float z);

	
	


	void buildVertexVBO();

	void buildVAO();

	~NoiseGeneration();

private:
	const float AMPLITUDE = 70.0f;
	const int OCTAVES = 3; //number of noise funtion we use
	const float ROUGHNESS = 0.3f; //How much we decrease the almplitude by each higher octave
	//srand(time(0));
	int random_number = rand()%1000;
	//float setRandomSeed;
	int seed;// = srand(time(0));

	float noises;

	GLuint VAO, vertex_VBO, index_EBO; //Need to possublity add
	std::vector<GLfloat> vertices;

};

