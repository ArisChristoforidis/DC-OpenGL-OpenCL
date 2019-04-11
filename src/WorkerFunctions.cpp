#include "WorkerFunctions.h"


void Work(MeshInfo &info, bool &updateObjects) {
	DualContour dualContour;
	dualContour.ExtractSurface(CircleFunction);

	info.vertices = dualContour.vertArray;
	info.indices = dualContour.indices;
	updateObjects = true;
}
