#include "WorkerFunctions.h"


void Work(MeshData &data, bool &updateObjects) {
	DualContour dualContour;
	dualContour.ExtractSurface(CircleFunction);



	data.vertices = dualContour.vertArray;
	data.indices = dualContour.indices;
	updateObjects = true;
}
