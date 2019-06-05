#include "ComputeManager.h"


#define RANGE 64
#define CUBECOUNT (RANGE*2)*(RANGE*2)*(RANGE*2)

ComputeManager::ComputeManager() {

}

ComputeManager::~ComputeManager() {
}


void ComputeManager::Work(MeshData &data, bool &updateObjects) {

	auto start = std::chrono::high_resolution_clock::now();


	cl_device_id deviceID;
	cl_context context;
	cl_command_queue queue;
	cl_mem workDataArray;
	cl_int nodeCount = CUBECOUNT;
	cl_program program;
	cl_kernel kernel;
	cl_platform_id platformID;
	cl_uint deviceCount;
	cl_uint platformCount;
	cl_int returnCode;

	FILE* inputFile;
	char fileName[] = "src/KernelFile.cl";
	char* src;
	size_t srcSize;

	//Open CL file.
	inputFile = fopen(fileName, "r");
	if (!inputFile) {
		std::cout << "Failed to open CL file.No kernel functions will be executed." << std::endl;
		return;
	}

	//Read CL file.
	int maxSourceSize = 0x100000;
	src = (char*)malloc(maxSourceSize);
	srcSize = fread(src, 1, maxSourceSize, inputFile);
	fclose(inputFile);


	//Get available platforms.
	returnCode = clGetPlatformIDs(1, &platformID, &platformCount);

	//Get available GPU devices.
	returnCode = clGetDeviceIDs(platformID, CL_DEVICE_TYPE_GPU, 1, &deviceID, &deviceCount);

	//Get max local work size.
	size_t maxWorkGroupSize[3];
	returnCode = clGetDeviceInfo(deviceID, CL_DEVICE_MAX_WORK_ITEM_SIZES, sizeof(size_t) * 3, maxWorkGroupSize, NULL);

	//Print debug info.
	printf("Local work size: [%d, %d, %d]\n", maxWorkGroupSize[0], maxWorkGroupSize[1], maxWorkGroupSize[2]);
	printf("Platform ID: %ld\n", (int)platformID);
	printf("Return Code: %s\n", (int)returnCode);
	printf("Platform Count: %u\n", (int)platformCount);
	printf("CL_DEVICE_TYPE_DEFAULT: %d\n", CL_DEVICE_TYPE_GPU);
	printf("Device ID: %d\n", (int)deviceID);
	printf("Device Count: %d\n", (int)deviceCount);

	//Create OpenCL context.
	context = clCreateContext(NULL, 1, &deviceID, NULL, NULL, &returnCode);

	//Create command queue.
	queue = clCreateCommandQueueWithProperties(context, deviceID,NULL, &returnCode);

	//Create memory buffer on the device.
	workDataArray = clCreateBuffer(context, CL_MEM_WRITE_ONLY | CL_MEM_HOST_READ_ONLY, CUBECOUNT * sizeof(WorkData), NULL, &returnCode);

	//Create program from source,then build it.
	program = clCreateProgramWithSource(context, 1, (const char **)&src, (const size_t *)&srcSize, &returnCode);
	//Disable optimization with "-O0" instruction - without it the kernel crashes on while solving the linear equation.
	returnCode = clBuildProgram(program, 1, &deviceID, "-O0", NULL, NULL);

	//Create OpenCL kernel.
	kernel = clCreateKernel(program, "EvaluateWorldSpace", &returnCode);
	//kernel = clCreateKernel(program, "HelloWorld", &returnCode);

	//Set kernel parameters.
	returnCode = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&workDataArray);
	returnCode = clSetKernelArg(kernel, 1, sizeof(cl_int), &nodeCount);

	//Define work sizes.
	const size_t globalWorkSize = CUBECOUNT;
	const size_t localWorkSize = maxWorkGroupSize[0];
	//Execute OpenCL kernel.
	returnCode = clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &globalWorkSize, &localWorkSize, 0, NULL, NULL);
	std::cout << returnCode << std::endl;

	//Kernel executes on the GPU...

	//Copy results from the device memory to host memory.
	WorkData* workData = (WorkData*)malloc(globalWorkSize * sizeof(WorkData));
	returnCode = clEnqueueReadBuffer(queue, workDataArray, CL_TRUE, 0, globalWorkSize * sizeof(WorkData), workData, 0, NULL, NULL);

	auto kernelTime = std::chrono::high_resolution_clock::now();
	auto dur = kernelTime - start;
	auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
	std::cout << "Finished the first part in " << ms << " ms\n";


	std::vector<float> vertexArray;
	for (int i = 0; i < globalWorkSize; i++) {
		if (workData[i].used == 1) {
			workData[i].index = vertexArray.size() / 3;
			vertexArray.push_back(workData[i].x);
			vertexArray.push_back(workData[i].y);
			vertexArray.push_back(workData[i].z);
		}
	}
	
	std::vector<unsigned int> indexArray;
	#pragma omp parallel for num_threads(16)
	for (int oneDimIndex = 0; oneDimIndex < globalWorkSize; oneDimIndex++) {
		//Find item
		WorkData item = workData[oneDimIndex];

		//Find "unsigned" 3D coordinates.
		unsigned int uz = oneDimIndex % RANGE;
		unsigned int uy = (oneDimIndex / RANGE) % RANGE;
		unsigned int ux = oneDimIndex / (RANGE * RANGE);
		
		//Find real 3D coordinates.
		int z = uz - RANGE / 2;
		int y = uy - RANGE / 2;
		int x = ux - RANGE / 2;

		//Check the z axis edge.
		if (x > MIN && y > MIN) {
			bool corner0Sign = (item.cornerValues >> 0) & 1;
			bool corner1Sign = (item.cornerValues >> 1) & 1;

			if (corner0Sign != corner1Sign) {
				unsigned int x0y0 = workData[uz + (uy - 1) * RANGE + (ux - 1) * RANGE*RANGE].index;
				unsigned int x0y1 = workData[uz + uy * RANGE + (ux - 1) * RANGE*RANGE].index;
				unsigned int x1y0 = workData[uz + (uy - 1) * RANGE + ux * RANGE*RANGE].index;
				unsigned int x1y1 = workData[uz + uy * RANGE + ux * RANGE*RANGE].index;
				
				#pragma omp critical
				{
					//Triangle one.
					indexArray.push_back(x0y0);
					indexArray.push_back(x0y1);
					indexArray.push_back(x1y1);

					//Triangle two.
					indexArray.push_back(x0y0);
					indexArray.push_back(x1y1);
					indexArray.push_back(x1y0);
				}
			}
		}

		//Check the y axis edge.
		if (x > MIN && z > MIN) {
			bool corner0Sign = (item.cornerValues >> 0) & 1;
			bool corner1Sign = (item.cornerValues >> 2) & 1;

			if (corner0Sign != corner1Sign) {
				unsigned int x0z0 = workData[(uz - 1) + uy * RANGE + (ux - 1) * RANGE*RANGE].index;
				unsigned int x0z1 = workData[uz + uy* RANGE + (ux - 1) * RANGE*RANGE].index;
				unsigned int x1z0 = workData[(uz - 1) + uy * RANGE + ux* RANGE*RANGE].index;
				unsigned int x1z1 = workData[uz + uy * RANGE + ux * RANGE*RANGE].index;
				
				#pragma omp critical
				{
					//Triangle one.
					indexArray.push_back(x0z0);
					indexArray.push_back(x0z1);
					indexArray.push_back(x1z1);

					//Triangle two.
					indexArray.push_back(x0z0);
					indexArray.push_back(x1z1);
					indexArray.push_back(x1z0);
				}
			}
		}

		//Check the x axis edge.
		if (y > MIN && z > MIN) {
			bool corner0Sign = (item.cornerValues >> 0) & 1;
			bool corner1Sign = (item.cornerValues >> 4) & 1;

			if (corner0Sign != corner1Sign) {
				unsigned int y0z0 = workData[(uz - 1) + (uy - 1) * RANGE + ux * RANGE*RANGE].index;
				unsigned int y0z1 = workData[uz + (uy - 1) * RANGE + ux * RANGE*RANGE].index;
				unsigned int y1z0 = workData[(uz - 1) + uy * RANGE + ux * RANGE*RANGE].index;
				unsigned int y1z1 = workData[uz + uy * RANGE + ux * RANGE*RANGE].index;
				
				#pragma omp critical
				{
					//Triangle one.
					indexArray.push_back(y0z0);
					indexArray.push_back(y0z1);
					indexArray.push_back(y1z1);

					//Triangle two.
					indexArray.push_back(y0z0);
					indexArray.push_back(y1z1);
					indexArray.push_back(y1z0);
				}
			}
		}

	}


	auto end = std::chrono::high_resolution_clock::now();
	dur = end - kernelTime;
	ms = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
	std::cout << "Finished the seconds part in " << ms << " ms\n";
	
	dur = end - start;
	ms = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
	std::cout << "Finished dual contouring in " << ms << " sec\n";

	//Cleanup.
	returnCode = clFlush(queue);
	returnCode = clFinish(queue);
	returnCode = clReleaseKernel(kernel);
	returnCode = clReleaseProgram(program);
	returnCode = clReleaseMemObject(workDataArray);
	returnCode = clReleaseCommandQueue(queue);
	returnCode = clReleaseContext(context);
	free(src);

	
	//Send data back.
	data.vertices = vertexArray;
	data.indices = indexArray;
	updateObjects = true;
}