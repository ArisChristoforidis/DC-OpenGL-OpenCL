#ifndef COMPUTEMANAGER_H
#define COMPUTEMANAGER_H


#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <fstream>
#include <chrono>

#include "CL/cl.h"
#include "omp.h"

#include "MeshData.h"
#include "ShapeFunctions.h"
#include "DualContour.h"
#include "WorkData.h"



class ComputeManager {
public:
	ComputeManager();
	~ComputeManager();

	static void Work(MeshData &info, bool &updateObjects);

private:
	
};
#endif

