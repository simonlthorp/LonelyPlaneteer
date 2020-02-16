#pragma once
#include "CL/cl.hpp"

//#define PACK( __Declaration__ ) __pragma( pack(push, 1) ) __Declaration__ __pragma( pack(pop) )

#define PATH_SIZE 2
#define SEARCH_LIST_SIZE 32
#define MAX_RANGE 64


//PACK(
	//struct PathConfig {
	//	cl_uint mapWidth;
	//	cl_uint mapHeight;

	//	cl_uint sourceX;
	//	cl_uint sourceY;

	//	cl_uint targetX;
	//	cl_uint targetY;

	//	cl_uint maxPathSize;
	//	cl_uint searchListSize;
	//	cl_uint maxRange;

	//	cl_uint pathSize;
	//	cl_uint idCounter;

	//	cl_uint abc;
	//};
//);

struct PathConfig {
	cl_uint sourceX;
	cl_uint sourceY;

	cl_uint targetX;
	cl_uint targetY;
};

//PACK(
	struct PathNode {
		cl_uint x;
		cl_uint y;

		cl_uint g;
		cl_uint f;

		cl_uint ID;
		cl_uint pID;
	};
//);

//PACK(
	struct AdjacentNodesConfig {
		cl_bool top;
		cl_bool left;
		cl_bool bottom;
		cl_bool right;

		cl_bool topLeft;
		cl_bool bottomLeft;
		cl_bool topRight;
		cl_bool bottomRight;
	};
//);