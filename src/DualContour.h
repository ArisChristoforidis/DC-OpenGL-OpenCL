#ifndef DUALCONTOUR_H
#define DUALCONTOUR_H

#include <vector>
#include <iostream>
#include <cstdint>
#include <chrono>

#include <glm/glm.hpp>

#include "TreeNode.h"
#include "Mesh.h"
#include "Qef.h"

#define RANGE 64
#define MIN -RANGE
#define MAX RANGE
#define THRESHOLD 0


class DualContour {
public:
	DualContour();
	~DualContour();

	void ExtractSurface(float (*function)(float, float, float));
	std::vector<float> vertArray;
	std::vector<unsigned int> indices;

private:

	float Adapt(float v0Val, float v1Val);
	glm::fvec3 CalculateNormal(float(*function)(float, float, float), glm::fvec3 pos, float d = 0.0001f);
	glm::fvec3 Normalize(glm::fvec3 vector);

};



#endif
