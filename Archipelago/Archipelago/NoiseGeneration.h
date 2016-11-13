#pragma once
#include "stdHeader.h"
#include <cstdlib>
#include <ctime>
#include <random>

class NoiseGeneration
{
	std::vector<int> p;

public:

	NoiseGeneration();

	float generateHeight(int x, int y);
	float getNoise(int x, int z);
	float getSmoothNoise(int x, int z);
	float interpolation(float a, float b, float blend);
	float getInterpolatedNoise(float x, float z);


	~NoiseGeneration();

private:
	const float AMPLITUDE = 70.0f;
	const int OCTAVES = 3; //number of noise funtion we use
	const float ROUGHNESS = 0.3f; //How much we decrease the almplitude by each higher octave
	int random_number = rand();
	int setRandomSeed;
	int seed;// = srand(time(0));

};

