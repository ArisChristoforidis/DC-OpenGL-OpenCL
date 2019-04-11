#include "ShapeFunctions.h"

float CircleFunction(float x, float y, float z) {
	return 15.0f - sqrt(x * x + y * y + z * z);
}

float TorusFunction(float x, float y, float z) {
	const float c = 50.0f;
	const float a = 6.0f;
	return (c - sqrt(x*x + y * y))*(c - sqrt(x*x + y * y)) + z * z - a * a;
}

float HelixFunction(float x, float y, float z) {
	const float r = 10.0f;
	const float a = 1.5f;
	return pow(x - x * (z / a), 2) + pow(y - y * (z / a), 2) - r * r;
}
