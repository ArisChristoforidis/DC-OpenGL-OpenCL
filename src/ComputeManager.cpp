#include "ComputeManager.h"



ComputeManager::ComputeManager() {

	cl_device_id deviceID;
	cl_context context;
	cl_command_queue queue;
	cl_mem memObj;
	cl_program program;
	cl_kernel kernel;
	cl_platform_id platformID;
	cl_uint deviceCount;
	cl_uint platformCount;
	cl_int returnCode;

	FILE* inputFile;
	char fileName[] = "src/HelloWorld.cl";
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

	//Print debug info.
	printf("Platform ID: %ld\n", (int)platformID);
	printf("Return Code: %s\n", (int)returnCode);
	printf("Platform Count: %u\n", (int)platformCount);
	printf("CL_DEVICE_TYPE_DEFAULT: %d\n", CL_DEVICE_TYPE_GPU);
	printf("Device ID: %d\n", (int)deviceID);
	printf("Device Count: %d\n", (int)deviceCount);

	//Create OpenCL context.
	context = clCreateContext(NULL, 1, &deviceID, NULL, NULL, &returnCode);

	//Create command queue.
	queue = clCreateCommandQueueWithProperties(context, deviceID, NULL, &returnCode);

	//Create memory buffer on the device.
	memObj = clCreateBuffer(context, CL_MEM_WRITE_ONLY | CL_MEM_HOST_READ_ONLY, 256 * sizeof(WorkData), NULL, &returnCode);
	//memObj = clCreateBuffer(context, CL_MEM_WRITE_ONLY | CL_MEM_HOST_READ_ONLY, 16 * sizeof(char), NULL, &returnCode);

	//Create program from source,then build it.
	program = clCreateProgramWithSource(context, 1, (const char **)&src, (const size_t *)&srcSize, &returnCode);
	returnCode = clBuildProgram(program, 1, &deviceID, NULL, NULL, NULL);

	//Create OpenCL kernel.
	kernel = clCreateKernel(program, "EvaluateWorldSpace", &returnCode);
	//kernel = clCreateKernel(program, "HelloWorld", &returnCode);

	//Set kernel parameters.
	returnCode = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&memObj);
	cl_int nodeCount = 256;
	returnCode = clSetKernelArg(kernel, 1, sizeof(cl_int),&nodeCount);

	//Execute OpenCL kernel.
	const size_t globalWorkSize = 256;
	const size_t localWorkSize = 256;
	returnCode = clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &globalWorkSize, &localWorkSize, 0, NULL, NULL);
	std::cout << returnCode << std::endl;

	//Copy results from the device memory to host memory.
	WorkData workData[256];
	returnCode = clEnqueueReadBuffer(queue, memObj, CL_TRUE, 0, 256 * sizeof(WorkData), workData, 0, NULL, NULL);

	//Print to evaluate.
	for (int i = 0; i < 256; i++) {
		std::cout << "Workdata [" << i << "]  used: ";
		if (workData[i].used == 0) {
			std::cout << "False\n";
		}else if(workData[i].used == 1) {
			std::cout << "True\n";
		}
	}

	//Cleanup.
	returnCode = clFlush(queue);
	returnCode = clFinish(queue);
	returnCode = clReleaseKernel(kernel);
	returnCode = clReleaseProgram(program);
	returnCode = clReleaseMemObject(memObj);
	returnCode = clReleaseCommandQueue(queue);
	returnCode = clReleaseContext(context);

	free(src);
}



ComputeManager::~ComputeManager() {
}


void ComputeManager::Work(MeshData &data, bool &updateObjects) {
	DualContour dualContour;
	dualContour.ExtractSurface(CircleFunction);



	data.vertices = dualContour.vertArray;
	data.indices = dualContour.indices;
	updateObjects = true;
}