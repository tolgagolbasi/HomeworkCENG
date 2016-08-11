//============================================================================
// Name        : ECMGPU.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================
#include <time.h>
#include <CL/cl.h>
#include <string.h>
#include <fstream>
#include <stdio.h>
using namespace std;
#define SDK_SUCCESS 0
#define SDK_FAILURE 1
#define NUM 256
int convertToString(const char * filename, std::string& str);

int convertToString(const char *filename, std::string& s){
	size_t size;
	char*  str;

	std::fstream f(filename, (std::fstream::in | std::fstream::binary));

	if (f.is_open())
	{
		size_t fileSize;
		f.seekg(0, std::fstream::end);
		size = fileSize = (size_t)f.tellg();
		f.seekg(0, std::fstream::beg);

		str = new char[size + 1];
		if (!str)
		{
			f.close();
			return SDK_FAILURE;
		}

		f.read(str, fileSize);
		f.close();
		str[size] = '\0';

		s = str;
		delete[] str;
		return SDK_SUCCESS;
	}
	printf("Error: Failed to open file %s\n", filename);
	return SDK_FAILURE;
}


cl_uint gcd(cl_uint a, cl_uint b) {
	cl_uint c;
	while (a != 0) {
		c = a;
		a = b % a;
		b = c;
	}
	return b;
}

cl_uint lcm(cl_uint B) {
	cl_uint a = 2;
	for (cl_uint i = 3; i <= B; i++)
		a = a * i / gcd(a, i);
	return a;
}

void Ecm(cl_uint* a, cl_uint Bp, cl_uint* c)
{
	cl_context          context;
	cl_device_id        *devices;
	cl_command_queue    commandQueue;
	cl_program program;
	cl_kernel  kernel;
	cl_int status = 0;
	size_t deviceListSize;

	cl_uint numPlatforms;
	cl_platform_id platform = NULL;
	clGetPlatformIDs(0, NULL, &numPlatforms);

	if (numPlatforms > 0)
	{
		cl_platform_id* platforms = (cl_platform_id *)malloc(numPlatforms * sizeof(cl_platform_id));
		clGetPlatformIDs(numPlatforms, platforms, NULL);
		for (unsigned int i = 0; i < numPlatforms; ++i)
		{
			char pbuff[100];
			clGetPlatformInfo(platforms[i], CL_PLATFORM_VENDOR, sizeof(pbuff), pbuff, NULL);
			platform = platforms[i];
		}
		delete platforms;
	}
	cl_context_properties cps[3] = { CL_CONTEXT_PLATFORM, (cl_context_properties)platform, 0 };
	context = clCreateContextFromType(cps, CL_DEVICE_TYPE_GPU, NULL, NULL, &status);
	status = clGetContextInfo(context, CL_CONTEXT_DEVICES, 0, NULL, &deviceListSize);
	devices = (cl_device_id *)malloc(deviceListSize);
	status = clGetContextInfo(context, CL_CONTEXT_DEVICES, deviceListSize, devices, NULL);

	commandQueue = clCreateCommandQueue(context, devices[0], 0, &status);

	const char * filename = "ECM.cl";
	std::string  sourceStr;

	convertToString(filename, sourceStr);

	const char * source = sourceStr.c_str();
	size_t sourceSize[] = { strlen(source) };
	program = clCreateProgramWithSource(context, 1, &source, sourceSize, &status);

	status = clBuildProgram(program, 1, devices, NULL, NULL, NULL);
	if (status != CL_SUCCESS) {
		// Determine the size of the log
		size_t log_size;
		clGetProgramBuildInfo(program, devices[0], CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);

		// Allocate memory for the log
		char *log = (char *)malloc(log_size);

		// Get the log
		clGetProgramBuildInfo(program, devices[0], CL_PROGRAM_BUILD_LOG, log_size, log, NULL);

		// Print the log
		printf("%s\n", log);
	}
	// get a kernel object handle for a kernel with the given name
	kernel = clCreateKernel(program, "Ecm", &status);
	cl_uint maxDims;
	cl_event events[2];
	size_t globalThreads[1];
	size_t localThreads[1];
	size_t maxWorkGroupSize;
	size_t maxWorkItemSizes[3];

	status = clGetDeviceInfo(devices[0], CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(size_t), (void*)&maxWorkGroupSize, NULL);
	status = clGetDeviceInfo(devices[0], CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS, sizeof(cl_uint), (void*)&maxDims, NULL);
	status = clGetDeviceInfo(devices[0], CL_DEVICE_MAX_WORK_ITEM_SIZES, sizeof(size_t)*maxDims, (void*)maxWorkItemSizes, NULL);
	globalThreads[0] = NUM;
	localThreads[0] = 1;
	if (localThreads[0] > maxWorkGroupSize || localThreads[0] > maxWorkItemSizes[0]){
		printf("Unsupported: Device does not support requested number of work items.");
	}

	cl_mem aa = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(cl_uint)*NUM, a, &status);
	cl_mem cc = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(cl_uint)*NUM, c, &status);
	clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&cc);
	clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&aa);
	clSetKernelArg(kernel, 2, sizeof(cl_uint), &Bp);

	status = clEnqueueNDRangeKernel(commandQueue, kernel, 1, NULL, globalThreads, localThreads, 0, NULL, &events[0]);
	if (status != CL_SUCCESS) { printf("%d", status); printf("Error: Enqueueing kernel onto command queue.\n"); }

	// wait for the kernel call to finish execution
	if (status != CL_SUCCESS){ printf("Error: Waiting for kernel run to finish.\n"); }

	status = clReleaseEvent(events[0]);
	if (status != CL_SUCCESS){ printf("Error: clReleaseEvent.\n"); }

	status = clEnqueueReadBuffer(commandQueue, cc, CL_TRUE, 0, sizeof(cl_uint)*NUM, c, 0, NULL, &events[1]);
	if (status != CL_SUCCESS){ printf("%d", status); printf("Error: clEnqueueReadBuffer failed.\n"); }

	// Wait for the read buffer to finish execution
	status = clReleaseEvent(events[1]); if (status != CL_SUCCESS){ printf("Error: clReleaseEvent.\n"); }

	status = clReleaseKernel(kernel);
	if (status != CL_SUCCESS){
		printf("Error: In clReleaseKernel \n");
	}
	status = clReleaseProgram(program);
	if (status != CL_SUCCESS){
		printf("Error: In clReleaseProgram\n");
	}
	status = clReleaseCommandQueue(commandQueue);
	if (status != CL_SUCCESS){
		printf("Error: In clReleaseCommandQueue\n");
	}
	status = clReleaseContext(context);
	if (status != CL_SUCCESS){
		printf("Error: In clReleaseContext\n");
	}
}
int main() {
	char str[80];
	cl_uint a[1024];
	cl_uint c[1024];
	srand((unsigned)time(NULL));
	for (cl_uint i = 0; i < 1024; i++){
		a[i] = rand() | (rand()) << 16;
		printf("%d\n", a[i]);
	}
	unsigned int Bs = 15;
	unsigned int Bp = lcm(Bs);
	Ecm(a, Bp, c);
	for (cl_uint i = 0; i < 1024; i++){
		printf("%d\n", c[i]);
	}
	scanf("%s", str);
}
