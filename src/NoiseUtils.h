#ifndef NOISEUTILS_H
#define NOISEUTILS_H

#include <stdint.h>
#include <math.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#define FREQUENCY 0.6
class NoiseUtils {
public:
	NoiseUtils();
	~NoiseUtils();

	static double SimplexNoise(double x,double y,double z,float frequency);
	static double Sum(double x, double y, double z, float frequency, float lacunarity, float persistence, int octaves = 2);

private:

	static glm::ivec3 grad3[];

	//Skew factors for the 3D Simplex noise.
	static const double F3;
	static const double G3;



	static int FastFloor(double x);
	static double Dot(glm::ivec3 gradient, double x, double y, double z);
};
#endif