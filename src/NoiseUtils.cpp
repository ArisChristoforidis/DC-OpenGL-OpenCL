#include "NoiseUtils.h"



NoiseUtils::NoiseUtils() {
}


NoiseUtils::~NoiseUtils() {
}

//Static members should be initialized outside the header file.
double const NoiseUtils::F3 = 1.0 / 3.0;
double const NoiseUtils::G3 = 1.0 / 6.0;
//int NoiseUtils::perm[512] = {};
//int NoiseUtils::permMod12[512] = {};


//TODO:Is probably incomplete,fix.
static  const uint8_t p[256] = {
	151,160,137,91,90,15,131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,
	142,8,99,37,240,21,10,23,190,6,148,247,120,234,75,0,26,197,62,94,252,219,
	203,117,35,11,32,57,177,33,88,237,149,56,87,174,20,125,136,171,168,68,175,
	74,165,71,134,139,48,27,166,77,146,158,231,83,111,229,122,60,211,133,230,
	220,105,92,41,55,46,245,40,244,102,143,54,65,25,63,161,1,216,80,73,209,76,
	132,187,208, 89,18,169,200,196,135,130,116,188,159,86,164,100,109,198,173,
	186, 3,64,52,217,226,250,124,123,5,202,38,147,118,126,255,82,85,212,207,206,
	59,227,47,16,58,17,182,189,28,42,223,183,170,213,119,248,152,2,44,154,163,
	70,221,153,101,155,167, 43,172,9,129,22,39,253,19,98,108,110,79,113,224,232,
	178,185, 112,104,218,246,97,228,251,34,242,193,238,210,144,12,191,179,162,
	241, 81,51,145,235,249,14,239,107,49,192,214,31,181,199,106,157,184,84,
	204,176,115,121,50,45,127, 4,150,254,138,236,205,93,222,114,67,29,24,72,
	243,141,128,195,78,66,215,61,156,180
};

glm::ivec3 NoiseUtils::grad3[] = {
		glm::ivec3(1,1,0),glm::ivec3(-1,1,0),glm::ivec3(1,-1,0),glm::ivec3(-1,-1,0),
		glm::ivec3(1,0,1),glm::ivec3(-1,0,1),glm::ivec3(1,0,-1),glm::ivec3(-1,0,-1),
		glm::ivec3(0,1,1),glm::ivec3(0,-1,1),glm::ivec3(0,1,-1),glm::ivec3(0,-1,-1)
};

static float grad(int32_t hash, float x, float y, float z) {
	int h = hash & 15;     // Convert low 4 bits of hash code into 12 simple
	float u = h < 8 ? x : y; // gradient directions, and compute dot product.
	float v = h < 4 ? y : h == 12 || h == 14 ? x : z; // Fix repeats at h = 12 to 15
	return ((h & 1) ? -u : u) + ((h & 2) ? -v : v);
}

static inline uint8_t hash(int32_t i) {
	return p[static_cast<uint8_t>(i)];
}

//TODO:Add description.
double NoiseUtils::SimplexNoise(double x, double y, double z, float frequency) {
	x *= frequency;
	y *= frequency;
	z *= frequency;

	//Noise value in each corner.
	double n0, n1, n2, n3;

	//Skew the input.
	double s = (x + y + z)*F3;
	 
	int i = FastFloor(x + s);
	int j = FastFloor(y + s);
	int k = FastFloor(z + s);

	double t = (i + j + k)*G3;
	//Unskew the cell origin.
	double X0 = i - t;
	double Y0 = j - t;
	double Z0 = k - t;
	//The distances from the cell origin.
	double x0 = x - X0;
	double y0 = y - Y0;
	double z0 = z - Z0;

	int i1, j1, k1;
	int i2, j2, k2;

	if(x0>=y0){
		if(y0>=z0){
			//X Y Z order.
			i1 = 1;
			j1 = 0;
			k1 = 0;
			i2 = 1;
			j2 = 1;
			k2 = 0;
		}else if(x0 >=z0){
			//X Z Y order.
			i1 = 1;
			j1 = 0;
			k1 = 0;
			i2 = 1;
			j2 = 0; 
			k2 = 1;
		}else{
			//Z X Y order.
			i1 = 0;
			j1 = 0;
			k1 = 1;
			i2 = 1;
			j2 = 0;
			k2 = 1;
		}
	}else{
		if(y0<z0){
			//Z Y X order.
			i1 = 0;
			j1 = 0;
			k1 = 1;
			i2 = 0;
			j2 = 1;
			k2 = 1;
		} else if (x0 < z0) {
			//Y Z X order.
			i1 = 0;
			j1 = 1;
			k1 = 0;
			i2 = 0;
			j2 = 1;
			k2 = 1;
		}else{
			//Y X Z order.
			i1 = 0;
			j1 = 1;
			k1 = 0;
			i2 = 1;
			j2 = 1;
			k2 = 0;
		}
	}

	//Offsets for the second corner.
	double x1 = x0 - i1 + G3;
	double y1 = y0 - j1 + G3;
	double z1 = z0 - k1 + G3;

	//Offsets for the third corner.
	double x2 = x0 - i2 + 2.0*G3;
	double y2 = y0 - j2 + 2.0*G3;
	double z2 = z0 - k2 + 2.0*G3;

	//Offsets for the fourth corner.
	double x3 = x0 - 1.0 + 3.0*G3;
	double y3 = y0 - 1.0 + 3.0*G3;
	double z3 = z0 - 1.0 + 3.0*G3;

	//Calculate the hashed gradient indices on the four simplex corners.
	int ii = i & 255;
	int jj = j & 255;
	int kk = k & 255;

	int gi0 = hash(i + hash(j + hash(k)));
	int gi1 = hash(i + i1 + hash(j + j1 + hash(k + k1)));
	int gi2 = hash(i + i2 + hash(j + j2 + hash(k + k2)));
	int gi3 = hash(i + 1 + hash(j + 1 + hash(k + 1)));

	//Calculate contribution from the four corners.
	double t0 = 0.6 - x0 * x0 - y0 * y0 - z0 * z0;
	if(t0<0){
		n0 = 0.0;
	}else{
		t0 *= t0;
		n0 = t0 * t0 * grad(gi0, x0, y0, z0);
	}

	double t1 = 0.6 - x1 * x1 - y1 * y1 - z1 * z1;
	if(t1<0){
		n1 = 0.0;
	}else{
		t1 *= t1;
		n1 = t1 * t1*grad(gi1, x1, y1, z1);
	}

	double t2 = 0.6 - x2 * x2 - y2 * y2 - z2 * z2;
	if (t2 < 0) {
		n2 = 0.0;
	} else {
		t2 *= t2;
		n2 = t2 * t2*grad(gi2, x2, y2, z2);
	}

	double t3 = 0.6 - x3 * x3 - y3 * y3 - z3 * z3;
	if (t3 < 0) {
		n3 = 0.0;
	} else {
		t3 *= t3;
		n3 = t3 * t3*grad(gi3, x3, y3, z3);
	}

	return 32.0*(n0 + n1 + n2 + n3);
	
}

double NoiseUtils::Sum(double x, double y, double z, float frequency, float lacunarity, float persistence, int octaves) {
	float sum = SimplexNoise(x, y, z, frequency);
	float amplitude = 1.0f;
	float range = 1.0f;
	for (int i = 0; i < octaves; i++) {
		frequency *= lacunarity;
		amplitude *= persistence;
		range += amplitude;
		sum += SimplexNoise(x, y, z, frequency)*amplitude;
	}
	return sum / range;
}

int NoiseUtils::FastFloor(double x) {
	int xInt = (int)x;
	return x < xInt ? xInt - 1 : xInt;
}

double NoiseUtils::Dot(glm::ivec3 gradient, double x, double y, double z) {
	return gradient.x*x + gradient.y*y + gradient.z*z;
}
