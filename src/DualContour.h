#ifndef DUALCONTOUR_H
#define DUALCONTOUR_H

#include <vector>
#include <iostream>
#include <cstdint>

#include <glm/glm.hpp>

#include "TreeNode.h"
#include "Mesh.h"
#include "Qef.h"


#define MIN -1
#define MAX 1
#define THRESHOLD 0
#define STEP 0.1


class DualContour {
public:
	DualContour();
	~DualContour();

	Mesh ExtractSurface(float (*function)(float, float, float));
	std::vector<float> vertArray;
	std::vector<unsigned int> indices;

private:

	glm::dvec3 FindBestVertex(double(*function)(double, double, double), double x, double y, double z);
	float Adapt(float v0Val, float v1Val);
	glm::fvec3 CalculateNormal(float(*function)(float, float, float), glm::fvec3 pos, float d = 0.01f);
	glm::fvec3 Normalize(glm::fvec3 vector);

};



#endif
