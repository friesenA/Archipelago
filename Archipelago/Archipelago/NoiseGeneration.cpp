/*CODE FROM:
https:// solarianprogramer.com/2012/07/18/perlin-noise-cpp-11/
https:// www.youtube.com/watch?v=yNYwZMmgTJk
https:// flafla2.github.io/2014/08/09/perlinnoise.html
*/

#include "NoiseGeneration.h"

#include <math.h>
# define M_PI  3.14159265358979323846  /* pi */

NoiseGeneration::NoiseGeneration() {
	srand(time(0));
	this->seed = random_number;
}

float NoiseGeneration::generateHeight(int x, int z) {
	//More general function:
	float total = 0;
	float d = (float)glm::pow(2, OCTAVES - 1);
	for (int i = 0; i < OCTAVES; i++) {
		float frequency = (float)(glm::pow(2, 1) / d);
		float amplitude = (float)glm::pow(ROUGHNESS, i)*AMPLITUDE;
		total += getInterpolatedNoise(x*frequency, z*frequency)*amplitude;
	}
	return total;
	/*
	float total = getInterpolatedNoise(x/4.0f, z/4.0f)*AMPLITUDE;
	total += getInterpolatedNoise(x / 2.0f, z / 2.0f)*AMPLITUDE/3.0f;
	total += getInterpolatedNoise(x , z )*AMPLITUDE / 9.0f;
	*/
}

//Random Number generator 
//returns a random number value btwn 1 and -1
float NoiseGeneration::getNoise(int x, int z) {

	//?????????????????
	float setRandomSeed = x* 49632 + z* 325176 + seed;
	return setRandomSeed; //...
}

float NoiseGeneration::getSmoothNoise(int x, int z)
{
	float corners = (getNoise(x - 1, z - 1) + getNoise(x + 1, z - 1) + getNoise(x - 1, z + 1) + getNoise(x + 1, z + 1)) / 16.0f;
	float sides = (getNoise(x - 1, z) + getNoise(x + 1, z) + getNoise(x, z - 1) + getNoise(x, z + 1)) / 8.0f;
	float center = (getNoise(x, z)) / 4.0f;
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



NoiseGeneration::~NoiseGeneration() {}
