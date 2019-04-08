#include "WorkerFunctions.h"


DualContour dualContour;
void Work(std::vector<Mesh> &objects, unsigned int &objectCount) {
	dualContour.ExtractSurface(TorusFunction);

	/*
	mesh->CreateVertexArray(dualContour.vertArray);
	mesh->CreateIndexBuffer(dualContour.indices);
	*/

	Mesh mesh;
	mesh.CreateVertexArray(dualContour.vertArray);
	mesh.CreateIndexBuffer(dualContour.indices);
	objects.push_back(mesh);
	objectCount++;
	std::cout << "Object Count: " << objectCount << std::endl;
}
