#ifndef MESHDATA_H
#define MESHDATA_H

#include <vector>

struct MeshData {
	std::vector<float> vertices;
	std::vector<unsigned int> indices;
};
#endif