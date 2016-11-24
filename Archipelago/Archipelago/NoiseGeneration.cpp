/*CODE FROM:
https:// solarianprogramer.com/2012/07/18/perlin-noise-cpp-11/
https:// www.youtube.com/watch?v=yNYwZMmgTJk
https:// flafla2.github.io/2014/08/09/perlinnoise.html
*/

#include "NoiseGeneration.h"

#include <math.h>
# define M_PI  3.14159265358979323846  /* pi */

NoiseGeneration::NoiseGeneration() { }

float NoiseGeneration::generateHeight(int x, int z) {
	//More general function:
	std::vector<glm::vec2> freqAmpArray;
	freqAmpArray.push_back(glm::vec2(0.05, 300));
	freqAmpArray.push_back(glm::vec2(0.2, 60));
	freqAmpArray.push_back(glm::vec2(1.0, 20));

	float frequency= 0;
	float amplitude = 0;
	float total = 0;
	float d = (float)glm::pow(2, OCTAVES - 1);
	for (int i = 0; i < OCTAVES; i++) {
		//frequency = (float)(glm::pow(2.0, i) / d);
		frequency = freqAmpArray[i].x;
		//float amplitude = 1.0f/(float)glm::pow(2, i)*AMPLITUDE;
		//amplitude = (float)glm::pow(ROUGHNESS, i)*AMPLITUDE;
		amplitude = freqAmpArray[i].y;
		//total += (getInterpolatedNoise(x*frequency/5, z*frequency/5)*amplitude);
		//or...
		total += getInterpolatedNoise(x*frequency, z*frequency)*amplitude;
	}
	return total;
}

//Random Number generator 
//returns a random number value btwn 1 and -1
float NoiseGeneration::getNoise(int x, int z) {
	
		int n;
		n = x + z * 57;
		x = (n << 13) ^ n;
		return (1.0 - ((n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0);
		//return abs((1.0 - ((n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0));
	
	//Ref: http: //pastebin.com/VJ5uvAhY
}

float NoiseGeneration::getSmoothNoise(int x, int z)
{
	float corners = (getNoise(x - 1, z - 1) + getNoise(x + 1, z - 1) + getNoise(x - 1, z + 1) + getNoise(x + 1, z + 1)) / 160.0f; //before: 160.0f, 100.0f is a scaling factor
	float sides = (getNoise(x - 1, z) + getNoise(x + 1, z) + getNoise(x, z - 1) + getNoise(x, z + 1)) / 80.0f; //before: 80.0f,50.0f
	float center = (getNoise(x, z)) / 40.0f; //before: 40.0f, 10.0f
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

	float fracX = x -intX; //fraction/decimal section of the x value
	float fracZ = z - intZ; //fraction/decimal section of the z value

	 //calutating the height of the nearest points to the noise function
	float v1 = getSmoothNoise(intX, intZ);
	float v2 = getSmoothNoise(intX + 1, intZ);
	float v3 = getSmoothNoise(intX, intZ + 1);
	float v4 = getSmoothNoise(intX + 1, intZ + 1);

	//Interpolate betwen the heights 
	float i_1 = interpolation(v1, v2, fracX);
	float i_2 = interpolation(v3, v4, fracX);

	return interpolation(i_1, i_2, fracZ); //This will give us the height (y) of the x, z postion.
}





NoiseGeneration::~NoiseGeneration() {}
