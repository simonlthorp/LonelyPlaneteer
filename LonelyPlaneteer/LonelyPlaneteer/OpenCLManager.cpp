#include "OpenCLManager.h"
#include <iostream>
#include <string>
#include <fstream>
#include "Enemy.h"
#include <chrono>

OpenCLManager::OpenCLManager(cl_int* mapData, size_t size) : map(mapData), mapSize(size) {
	this->init();
}

void OpenCLManager::init() {
	cl_int err = 0;

	// create context
	context = createContext();

	// create device and command queue
	cl::Device device;
	queue = createCommandQueue(context, (cl::Device*) &device);

	// create program
	const std::string& filename = "AStar.cl";
	program = createProgram(context, device, filename);
	
	// create kernel
	kernel = cl::Kernel(program, "AStar", &err);
	mapBuffer = cl::Buffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(cl_int) * mapSize, (void*)map, &err);

	err = kernel.setArg(0, mapBuffer);
}


bool OpenCLManager::runAStar(cl_uint*& targetX, cl_uint*& targetY, cl_uint enemyCount, cl_uint sourceX, cl_uint sourceY, cl_int*& pathX, cl_int*& pathY)
{
	//auto start = std::chrono::steady_clock::now();

	cl_int err = 0;

	cl::Buffer xBuffer(context, CL_MEM_READ_WRITE, sizeof(cl_int) * enemyCount, (void*)pathX, &err);
	cl::Buffer yBuffer(context, CL_MEM_WRITE_ONLY, sizeof(cl_int) * enemyCount, (void*)pathY, &err);

	cl::Buffer targetXBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(cl_uint) * enemyCount, (void*)targetX, &err);
	cl::Buffer targetYBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(cl_uint) * enemyCount, (void*)targetY, &err);

	err = kernel.setArg(1, targetXBuffer);
	err = kernel.setArg(2, targetYBuffer);
	err = kernel.setArg(3, enemyCount);
	err = kernel.setArg(4, sourceX);
	err = kernel.setArg(5, sourceY);
	err = kernel.setArg(6, xBuffer);
	err = kernel.setArg(7, yBuffer);

	// enqueue kernel
	err = queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(enemyCount));

	queue.enqueueReadBuffer(xBuffer, CL_TRUE, 0, sizeof(cl_int) * enemyCount, (void*)pathX);
	queue.enqueueReadBuffer(yBuffer, CL_TRUE, 0, sizeof(cl_int) * enemyCount, (void*)pathY);

	//auto end = std::chrono::steady_clock::now();

	// Store the time difference between start and end
	//auto diff = end - start;

	//std::cout << "(kernel) Found " << enemyCount << " paths in " << std::chrono::duration <double, std::milli>(diff).count() << " ms" << std::endl;

	return true;
}

void OpenCLManager::clean() {
	free(map);
}

cl::Context OpenCLManager::createContext() {
	cl_int err = 0;

	// find a platform
	// get all available platforms and select the first available.
	std::vector<cl::Platform> platforms;
	err = cl::Platform::get(&platforms);

	if (err < 0) {
		std::cout << " No platforms found. Check OpenCL installation!\n" << std::endl;
		std::cout << "Error Code: " << err << std::endl;
		exit(1);
	}

	cl::Platform platform = platforms[0];

#if _DEBUG
	std::cout << "Using platform: " << platform.getInfo<CL_PLATFORM_NAME>() << "\n";
#endif


	// fetch the available devices

	// get all devices and select
	std::vector<cl::Device> devices;

	// first look for GPU, then CPU if not found
	err = platform.getDevices(CL_DEVICE_TYPE_GPU, &devices);
	if (err == CL_DEVICE_NOT_FOUND)
	{
		err = platform.getDevices(CL_DEVICE_TYPE_CPU, &devices);
	}
	if (err < 0)
	{
		std::cout << " No devices found. Check OpenCL installation!\n";
		std::cout << "Error Code: " << err << std::endl;
		exit(1);
	}

	// choose the first available device
	//cl::Device device = devices[0];

	// create the context with all the devices of our chosen platform
	cl::Context context(devices);

	return context;
}

cl::CommandQueue OpenCLManager::createCommandQueue(cl::Context context, cl::Device* device) {
	cl_int err = 0;

	cl_uint numDevices = context.getInfo<CL_CONTEXT_NUM_DEVICES>();

	std::vector<cl::Device> deviceList;
	deviceList = context.getInfo<CL_CONTEXT_DEVICES>(&err);

	if (err < 0)
	{
		std::cerr << " ERROR code " << err;
		switch (err) {
		case CL_INVALID_CONTEXT:
			std::cerr << " (CL_INVALID_CONTEXT)";
			break;
		case CL_INVALID_VALUE:
			std::cerr << " (CL_INVALID_VALUE)";
			break;
		case CL_OUT_OF_RESOURCES:
			std::cerr << " (CL_OUT_OF_RESOURCES)";
			break;
		case CL_OUT_OF_HOST_MEMORY:
			std::cerr << " (CL_OUT_OF_HOST_MEMORY)";
			break;
		default:
			break;
		}
		return NULL;
	}

#if _DEBUG
	// Get device information for each device and print to cout
	cl_device_type deviceType;
	std::cout << std::endl << "Device list:" << std::endl;
	for (unsigned int i = 0; i < numDevices; i++)
	{

		std::cout << "    ";

		// get device type and print it to cout
		deviceType = deviceList.at(i).getInfo<CL_DEVICE_TYPE>(&err);

		if (err < 0)
		{
			std::cout << "Can't get device type!\n";
			std::cout << "Error Code: " << err << std::endl;
			delete(&deviceList);
			exit(1);
		}

		std::cout << " type " << deviceType << ":";
		if (deviceType & CL_DEVICE_TYPE_CPU)
			std::cout << " CPU";
		if (deviceType & CL_DEVICE_TYPE_GPU)
			std::cout << " GPU";
		if (deviceType & CL_DEVICE_TYPE_ACCELERATOR)
			std::cout << " accelerator";
		if (deviceType & CL_DEVICE_TYPE_DEFAULT)
			std::cout << " default";

		// get device name and print it to cout
		cl::STRING_CLASS deviceName = deviceList.at(i).getInfo<CL_DEVICE_NAME>();

		if (err < 0)
		{
			std::cout << "can't get device name!\n";
			delete(&deviceList);
			exit(1);
		}
		std::cout << " name=<" << deviceName << ">" << std::endl;

	}
	std::cout << std::endl;
#endif

	// create a command queue with the first device
	cl::CommandQueue commandQueue(context, deviceList.front(), NULL, &err);
	if (err < 0)
	{
		std::cout << "Failed to create commandQueue for device 0";
		delete(&deviceList);
		exit(1);
	}

	*device = deviceList.front();

	return commandQueue;
}


cl::Program OpenCLManager::createProgram(cl::Context context, cl::Device device, std::string filename) {
	// load kernel file into a string
	std::ifstream kernelFile(filename);
	std::string src(std::istreambuf_iterator<char>(kernelFile), (std::istreambuf_iterator<char>()));

	// converts file string into cl readable format
	cl::Program::Sources sources(1, std::make_pair(src.c_str(), src.length() + 1));

	// create the program
	cl_int err = 0;
	cl::Program program(context, sources, &err);

	if (program.build({ device }, "-cl-std=CL1.2") != CL_SUCCESS) {
		std::cout << "Error building: " << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device) << std::endl;
		std::cout << "Error Code: " << err << std::endl;
		exit(1);
	}

	return program;
}