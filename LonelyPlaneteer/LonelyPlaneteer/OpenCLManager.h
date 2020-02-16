#pragma once

#include "CL/cl.hpp"
#include "PathNode.h"

class OpenCLManager {
public:
	OpenCLManager(cl_int* map, size_t size);
	~OpenCLManager() {}

	void init();
	bool runAStar(cl_uint*& targetX, cl_uint*& targetY, cl_uint enemyCount, cl_uint sourceX, cl_uint sourceY, cl_int*& pathX, cl_int*& pathY);
	void clean();

	cl::Context createContext();
	cl::CommandQueue createCommandQueue(cl::Context context, cl::Device* device);
	cl::Program createProgram(cl::Context context, cl::Device device, std::string filename);

private:
	cl::Context context;
	cl::Program program;
	cl::CommandQueue queue;
	cl::Kernel kernel;

	cl_int* map;
	size_t mapSize;

	cl::Buffer mapBuffer;
};