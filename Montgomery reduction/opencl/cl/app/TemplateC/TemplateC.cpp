/**********************************************************************
Copyright ©2012 Advanced Micro Devices, Inc. All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

•	Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
•	Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or
 other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY
 DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************/

#include <time.h>
#include <CL/cl.h>
#include <string.h>
#include <cstdlib>
#include <iostream>
#include <string>
#include <fstream>

/*** GLOBALS ***/
#define SDK_SUCCESS 0
#define SDK_FAILURE 1
#define LEN 8 /* 8*32 = 256 bit */
#define NUM 10

int convertToString(const char * filename, std::string& str);

int convertToString(const char *filename, std::string& s){
    size_t size;
    char*  str;

    std::fstream f(filename, (std::fstream::in | std::fstream::binary));

    if(f.is_open())
    {
        size_t fileSize;
        f.seekg(0, std::fstream::end);
        size = fileSize = (size_t)f.tellg();
        f.seekg(0, std::fstream::beg);

        str = new char[size+1];
        if(!str)
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

void Multiply(cl_uint* a, cl_uint* b, cl_uint* c)
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
    
    if(numPlatforms > 0)
    {
        cl_platform_id* platforms = (cl_platform_id *)malloc(numPlatforms * sizeof(cl_platform_id));
        clGetPlatformIDs(numPlatforms, platforms, NULL);
        for(unsigned int i = 0; i < numPlatforms; ++i)
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

    const char * filename  = "Multiply.cl";
    std::string  sourceStr;

    convertToString(filename, sourceStr);

    const char * source    = sourceStr.c_str();
    size_t sourceSize[]    = { strlen(source) };
    program = clCreateProgramWithSource(context, 1, &source, sourceSize, &status);

    status = clBuildProgram(program, 1, devices, NULL, NULL, NULL);
	if (status != CL_SUCCESS) {
		// Determine the size of the log
		size_t log_size;
		clGetProgramBuildInfo(program, devices[0], CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);

		// Allocate memory for the log
		char *log = (char *) malloc(log_size);

		// Get the log
		clGetProgramBuildInfo(program, devices[0], CL_PROGRAM_BUILD_LOG, log_size, log, NULL);

		// Print the log
		printf("%s\n", log);
		system("pause");
	}
    // get a kernel object handle for a kernel with the given name
    kernel = clCreateKernel(program, "multiply", &status);
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
    localThreads[0]  = 1;
    if(localThreads[0] > maxWorkGroupSize || localThreads[0] > maxWorkItemSizes[0]){
        printf("Unsupported: Device does not support requested number of work items.");
    }

	cl_mem aa = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(cl_uint)*LEN*NUM, a, &status);
	cl_mem bb = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(cl_uint)*LEN*NUM, b, &status);
	cl_mem cc = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(cl_uint)*2*LEN*NUM, c, &status);
	clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&cc);
	clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&aa);
	clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&bb);

    status = clEnqueueNDRangeKernel(commandQueue, kernel, 1, NULL, globalThreads, localThreads, 0, NULL, &events[0]);
	if(status != CL_SUCCESS) {printf("%d",status);printf("Error: Enqueueing kernel onto command queue.\n");system("pause");}

    // wait for the kernel call to finish execution 
    status = clWaitForEvents(1, &events[0]);
	if(status != CL_SUCCESS){printf("Error: Waiting for kernel run to finish.\n");}

    status = clReleaseEvent(events[0]);
	if(status != CL_SUCCESS){printf("Error: clReleaseEvent.\n");}

	status = clEnqueueReadBuffer(commandQueue, cc, CL_TRUE, 0, sizeof(cl_uint)*2*LEN*NUM, c, 0, NULL, &events[1]);
	if(status != CL_SUCCESS){printf("%d",status);printf("Error: clEnqueueReadBuffer failed.\n");}
    
    // Wait for the read buffer to finish execution
    status = clWaitForEvents(1, &events[1]);if(status != CL_SUCCESS) {printf("Error: Waiting for read buffer call to finish.\n");}
    
    status = clReleaseEvent(events[1]);if(status != CL_SUCCESS){printf("Error: clReleaseEvent.\n");}

	printf("\n");
	for(int i=0; i<LEN*2; i++){
			printf("%u*(2^(32*%d))+", c[i],2*LEN-1-i);
	}

    status = clReleaseKernel(kernel);
    if(status != CL_SUCCESS){
        printf("Error: In clReleaseKernel \n");
    }
    status = clReleaseProgram(program);
    if(status != CL_SUCCESS){
        printf("Error: In clReleaseProgram\n");
    }
    status = clReleaseCommandQueue(commandQueue);
    if(status != CL_SUCCESS){
        printf("Error: In clReleaseCommandQueue\n");
    }
    status = clReleaseContext(context);
    if(status != CL_SUCCESS){
        printf("Error: In clReleaseContext\n");
    }
}

void Add(cl_uint* a, cl_uint* b, cl_uint* c)
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
    
    if(numPlatforms > 0)
    {
        cl_platform_id* platforms = (cl_platform_id *)malloc(numPlatforms * sizeof(cl_platform_id));
        clGetPlatformIDs(numPlatforms, platforms, NULL);
        for(unsigned int i = 0; i < numPlatforms; ++i)
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

    const char * filename  = "Add.cl";
    std::string  sourceStr;

    convertToString(filename, sourceStr);

    const char * source    = sourceStr.c_str();
    size_t sourceSize[]    = { strlen(source) };
    program = clCreateProgramWithSource(context, 1, &source, sourceSize, &status);

    status = clBuildProgram(program, 1, devices, NULL, NULL, NULL);
	if (status != CL_SUCCESS) {
		// Determine the size of the log
		size_t log_size;
		clGetProgramBuildInfo(program, devices[0], CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);

		// Allocate memory for the log
		char *log = (char *) malloc(log_size);

		// Get the log
		clGetProgramBuildInfo(program, devices[0], CL_PROGRAM_BUILD_LOG, log_size, log, NULL);

		// Print the log
		printf("%s\n", log);
		system("pause");
	}
    // get a kernel object handle for a kernel with the given name
    kernel = clCreateKernel(program, "add", &status);
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
    localThreads[0]  = 1;
    if(localThreads[0] > maxWorkGroupSize || localThreads[0] > maxWorkItemSizes[0]){
        printf("Unsupported: Device does not support requested number of work items.");
    }

	cl_mem aa = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(cl_uint)*LEN*NUM, a, &status);
	cl_mem bb = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(cl_uint)*LEN*NUM, b, &status);
	cl_mem cc = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(cl_uint)*2*LEN*NUM, c, &status);
	clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&cc);
	clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&aa);
	clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&bb);

    status = clEnqueueNDRangeKernel(commandQueue, kernel, 1, NULL, globalThreads, localThreads, 0, NULL, &events[0]);
	if(status != CL_SUCCESS) {printf("%d",status);printf("Error: Enqueueing kernel onto command queue.\n");system("pause");}

    // wait for the kernel call to finish execution 
    status = clWaitForEvents(1, &events[0]);
	if(status != CL_SUCCESS){printf("Error: Waiting for kernel run to finish.\n");}

    status = clReleaseEvent(events[0]);
	if(status != CL_SUCCESS){printf("Error: clReleaseEvent.\n");}

	status = clEnqueueReadBuffer(commandQueue, cc, CL_TRUE, 0, sizeof(cl_uint)*2*LEN*NUM, c, 0, NULL, &events[1]);
	if(status != CL_SUCCESS){printf("%d",status);printf("Error: clEnqueueReadBuffer failed.\n");}
    
    // Wait for the read buffer to finish execution
    status = clWaitForEvents(1, &events[1]);if(status != CL_SUCCESS) {printf("Error: Waiting for read buffer call to finish.\n");}
    
    status = clReleaseEvent(events[1]);if(status != CL_SUCCESS){printf("Error: clReleaseEvent.\n");}

	printf("\n");
	for(int i=0; i<LEN*2; i++){
			printf("%u*(2^(32*%d))+", c[i],2*LEN-1-i);
	}

    status = clReleaseKernel(kernel);
    if(status != CL_SUCCESS){
        printf("Error: In clReleaseKernel \n");
    }
    status = clReleaseProgram(program);
    if(status != CL_SUCCESS){
        printf("Error: In clReleaseProgram\n");
    }
    status = clReleaseCommandQueue(commandQueue);
    if(status != CL_SUCCESS){
        printf("Error: In clReleaseCommandQueue\n");
    }
    status = clReleaseContext(context);
    if(status != CL_SUCCESS){
        printf("Error: In clReleaseContext\n");
    }
}

void Reduce(cl_uint* a, cl_uint* b, cl_uint* c)
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
    
    if(numPlatforms > 0)
    {
        cl_platform_id* platforms = (cl_platform_id *)malloc(numPlatforms * sizeof(cl_platform_id));
        clGetPlatformIDs(numPlatforms, platforms, NULL);
        for(unsigned int i = 0; i < numPlatforms; ++i)
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

    const char * filename  = "Reduce.cl";
    std::string  sourceStr;

    convertToString(filename, sourceStr);

    const char * source    = sourceStr.c_str();
    size_t sourceSize[]    = { strlen(source) };
    program = clCreateProgramWithSource(context, 1, &source, sourceSize, &status);

    status = clBuildProgram(program, 1, devices, NULL, NULL, NULL);
	if (status != CL_SUCCESS) {
		// Determine the size of the log
		size_t log_size;
		clGetProgramBuildInfo(program, devices[0], CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);

		// Allocate memory for the log
		char *log = (char *) malloc(log_size);

		// Get the log
		clGetProgramBuildInfo(program, devices[0], CL_PROGRAM_BUILD_LOG, log_size, log, NULL);

		// Print the log
		printf("%s\n", log);
		system("pause");
	}
    // get a kernel object handle for a kernel with the given name
    kernel = clCreateKernel(program, "reduce", &status);
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
    localThreads[0]  = 1;
    if(localThreads[0] > maxWorkGroupSize || localThreads[0] > maxWorkItemSizes[0]){
        printf("Unsupported: Device does not support requested number of work items.");
    }

	cl_mem aa = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(cl_uint)*LEN*NUM, a, &status);
	cl_mem bb = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(cl_uint)*LEN*NUM, b, &status);
	cl_mem cc = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(cl_uint)*2*LEN*NUM, c, &status);
	clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&cc);
	clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&aa);
	clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&bb);

    status = clEnqueueNDRangeKernel(commandQueue, kernel, 1, NULL, globalThreads, localThreads, 0, NULL, &events[0]);
	if(status != CL_SUCCESS) {printf("%d",status);printf("Error: Enqueueing kernel onto command queue.\n");system("pause");}

    // wait for the kernel call to finish execution 
    status = clWaitForEvents(1, &events[0]);
	if(status != CL_SUCCESS){printf("Error: Waiting for kernel run to finish.\n");}

    status = clReleaseEvent(events[0]);
	if(status != CL_SUCCESS){printf("Error: clReleaseEvent.\n");}

	status = clEnqueueReadBuffer(commandQueue, cc, CL_TRUE, 0, sizeof(cl_uint)*2*LEN*NUM, c, 0, NULL, &events[1]);
	if(status != CL_SUCCESS){printf("%d",status);printf("Error: clEnqueueReadBuffer failed.\n");}
    
    // Wait for the read buffer to finish execution
    status = clWaitForEvents(1, &events[1]);if(status != CL_SUCCESS) {printf("Error: Waiting for read buffer call to finish.\n");}
    
    status = clReleaseEvent(events[1]);if(status != CL_SUCCESS){printf("Error: clReleaseEvent.\n");}

	printf("\n");
	for(int i=0; i<LEN*2; i++){
			printf("%u*(2^(32*%d))+", c[i],2*LEN-1-i);
	}

    status = clReleaseKernel(kernel);
    if(status != CL_SUCCESS){
        printf("Error: In clReleaseKernel \n");
    }
    status = clReleaseProgram(program);
    if(status != CL_SUCCESS){
        printf("Error: In clReleaseProgram\n");
    }
    status = clReleaseCommandQueue(commandQueue);
    if(status != CL_SUCCESS){
        printf("Error: In clReleaseCommandQueue\n");
    }
    status = clReleaseContext(context);
    if(status != CL_SUCCESS){
        printf("Error: In clReleaseContext\n");
    }
}

int main(int argc, char * argv[]){
	cl_uint a[NUM*LEN], b[NUM*LEN], modul[NUM*LEN], c[2*NUM*LEN], p[LEN], e[LEN], k[LEN], t[LEN], a1[NUM*LEN*2], b1[NUM*LEN*2], a2[NUM*LEN*2], b2[NUM*LEN*2];
	srand((unsigned)time(NULL));
    for(cl_uint i = 0; i < NUM; i++){
	    for(cl_uint j = 0; j < LEN; j++){
			a[i*LEN+j] = rand()|rand()<<16;
			b[i*LEN+j] = rand()|rand()<<16;
		}
	}
	e[7] = -1;
	for(int i=0; i<LEN; i++){
		e[i] = -1;
	}
	p[7] = -587;
	for(int i=0; i<LEN-1; i++){
		p[i] = -1;
	}
	Multiply( a, e, a1);
	Multiply( b, e, b1);
	Reduce( a1, p, a2);
	Reduce( b1, p, b2);
	Multiply( a2, b2, c);
	for (int i=0;i<LEN;i++)
		k[i] = c[i] * (-1) * (1/p[0]);
	Multiply( k, p, t);//?
	Add( a, t, a1);
	Multiply( a1, 1/(2^256), modul);
	system ("pause");
    return SDK_SUCCESS;
}