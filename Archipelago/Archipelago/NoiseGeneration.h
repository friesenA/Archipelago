#pragma once
#include "stdHeader.h"
#include <cstdlib>
#include <ctime>
#include <random>



	const int NOISE_WIDTH = 20;
	const int NOISE_HEIGH = 30;

	const float AMPLITUDE = 90.0f; //70.0
	const int OCTAVES = 2; //number of noise funtion we use
	const float ROUGHNESS = 0.3f; //0.3f; //How much we decrease the almplitude by each higher octave

class NoiseGeneration
{

public:

	NoiseGeneration();
	

	float generateHeight(int x, int z);
	float getNoise(int x, int z);
	float getSmoothNoise(int x, int z);
	float interpolation(float a, float b, float blend);
	float getInterpolatedNoise(float x, float z);

	
	~NoiseGeneration();

private:

	
	
	int random_number = rand()%1000;
	int seed = random_number;

	float noises;

	GLuint VAO, vertex_VBO, index_EBO; //Need to possublity add
	std::vector<GLfloat> vertices;

};

