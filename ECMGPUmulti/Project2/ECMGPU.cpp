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
#define NUM 1
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
long Isqrt(long x){
	long   squaredbit, remainder, root;
	if (x<1) return 0;
	squaredbit = (long)((((unsigned long)~0L) >> 1) &
		~(((unsigned long)~0L) >> 2));
	remainder = x;  root = 0;
	while (squaredbit > 0) {
		if (remainder >= (squaredbit | root)) {
			remainder -= (squaredbit | root);
			root >>= 1; root |= squaredbit;
		}
		else {
			root >>= 1;
		}
		squaredbit >>= 2;
	}

	return root;
}
void mult2l(cl_ulong* c, cl_ulong* n, cl_ulong* points, cl_uint Bp, cl_uint a)
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
	context = clCreateContextFromType(cps, CL_DEVICE_TYPE_CPU, NULL, NULL, &status);
	if (status != CL_SUCCESS){
		printf("Error: In context \n");
	}
	status = clGetContextInfo(context, CL_CONTEXT_DEVICES, 0, NULL, &deviceListSize);
	if (status != CL_SUCCESS){
		printf("Error: In contextinfo \n");
	}
	devices = (cl_device_id *)malloc(deviceListSize);
	status = clGetContextInfo(context, CL_CONTEXT_DEVICES, deviceListSize, devices, NULL);
	if (status != CL_SUCCESS){
		printf("Error: In contextinfo2 \n");
	}
	commandQueue = clCreateCommandQueue(context, devices[0], 0, &status);

	const char * filename = "C:\\Users\\tolgag\\Documents\\Visual Studio 2013\\Projects\\ECMGPU\\Project2\\ECM.cl";
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
	kernel = clCreateKernel(program, "mult2l", &status);
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
	cl_mem cc = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(cl_ulong)*NUM, c, &status);
	cl_mem nn = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(cl_ulong)*NUM * 2, n, &status);
	cl_mem ppoints = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(cl_ulong)*NUM * 2, points, &status);
	clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&cc);
	clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&nn);
	clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&ppoints);
	clSetKernelArg(kernel, 3, sizeof(cl_ushort), &Bp);
	clSetKernelArg(kernel, 4, sizeof(cl_ushort), &a);
	status = clEnqueueNDRangeKernel(commandQueue, kernel, 1, NULL, globalThreads, localThreads, 0, NULL, &events[0]);
	if (status != CL_SUCCESS) { printf("%d", status); printf("Error: Enqueueing kernel onto command queue.\n"); }

	// wait for the kernel call to finish execution
	if (status != CL_SUCCESS){ printf("Error: Waiting for kernel run to finish.\n"); }

	status = clReleaseEvent(events[0]);
	if (status != CL_SUCCESS){ printf("Error: clReleaseEvent.\n"); }

	status = clEnqueueReadBuffer(commandQueue, cc, CL_TRUE, 0, sizeof(cl_ulong)*NUM, c, 0, NULL, &events[1]);
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
void gcd(cl_ulong* c, cl_ulong* fp, cl_ulong* n){

}
int main() {
	cl_ulong n[NUM*2];
	cl_ulong c[NUM];
	cl_ulong d[NUM];
	cl_ulong fpoint[NUM*2];
	srand((unsigned)time(NULL));
	for (cl_uint i = 0; i < NUM; i++){
		c[i] = 0;
		n[i] = rand()*rand();
		n[i*2+1] = rand()*rand();
	}
	bool check;
	cl_ushort Bp = 100;
	unsigned char bounda = 5;
	unsigned char boundb = 5;
	unsigned long long y2, y3;
	unsigned char a = 0, b, x;
	do{
		a += 1;
		if ((4 * a * a * a + 27) == 0)
			continue;
		for (b = 1; b <= boundb; b++){
			x = 0;
			do{
				y2 = (x * x * x + (a * x) + b);
				x = x + 1;
				y3 = Isqrt(y2);
			} while (((y3*y3) != y2)&&(x < 25));
			if (x >= 25){
				continue;
			}
			mult2l(c, n, fpoint, Bp, a );
			gcd(d, fpoint, n);
			check = 0;
			for (int i=0; i < NUM; i++){
				if (d[i] == 1)
					check = 1;
			}
		}
	} while ((a < bounda) && check);
	system("pause");
}
