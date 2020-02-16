#define PATH_SIZE 16
#define SEARCH_LIST_SIZE 64
#define MAX_RANGE 128

#define MAP_WIDTH 100
#define MAP_HEIGHT 100
#define MAP_SIZE (MAP_WIDTH * MAP_HEIGHT)
#define ENEMY_COUNT 32

#define PATH_PERECNT 0.1f

typedef struct {
	uint x;
	uint y;

	uint g;
	uint f;

	uint ID;
	uint pID;
} PathNode;

void insertInListByFScore(__private PathNode* nodeList, size_t nodeListSize, PathNode node) {
	int index = 0;

	for (; index < nodeListSize; index++) {
		if (node.f <= nodeList[index].f) {
			break;
		}
	}

	for (int i = nodeListSize; i > index; i--)
	{
		nodeList[i] = nodeList[i - 1];
	}

	nodeList[index].x = node.x;
	nodeList[index].y = node.y;
	nodeList[index].f = node.f;
	nodeList[index].g = node.g;
	nodeList[index].ID = node.ID;
	nodeList[index].pID = node.pID;
}

uint calculateHeuristic(uint sourceX, uint sourceY, uint targetX, uint targetY) {
	// Chebyshev distance
	//const int D = 1;
	//const int D2 = 1;

	// Octile distance
	uint D = 10;
	uint D2 = 14;

	int xDistance, yDistance;

	xDistance = sourceX - targetX;
	yDistance = sourceY - targetY;

	if (xDistance < 0) {
		xDistance *= -1;
	}

	if (yDistance < 0) {
		yDistance *= -1;
	}

	uint min;

	if (xDistance <= yDistance) {
		min = xDistance;
	}
	else {
		min = yDistance;
	}

	return D * (xDistance + yDistance) + (D2 - 2 * D) * min;
}

bool listContains(__private PathNode* nodeList, size_t nodeListSize, PathNode currentNode) {
	for (int i = 0; i < nodeListSize; i++) {
		if (nodeList[i].x == currentNode.x && nodeList[i].y == currentNode.y) {
			return true;
		}
	}
	return false;
}

__kernel void AStar(
	__constant int* map,
	__global uint* targetX,
	__global uint* targetY,
	uint enemyCount,
	uint sourceX,
	uint sourceY,
	__global int* xBuffer,
	__global int* yBuffer) {
	int globalID = get_global_id(0);

	xBuffer[globalID] = -1;
	yBuffer[globalID] = -1;

	uint idCounter = 1;
	uint openListSize = 0;
	uint closedListSize = 0;
	uint adjacentNodesSize = 0;
	uint pathSize = 0;

	PathNode path[PATH_SIZE];
	PathNode openList[SEARCH_LIST_SIZE];
	PathNode closedList[SEARCH_LIST_SIZE];
	PathNode adjacentNodes[8];

	// check if source is inside a wall
	if (map[sourceX * MAP_WIDTH + sourceY] == 1) {
		//printf("source inside wall\n");
		return;
	}

	// check if target is inside a wall
	if (map[targetX[globalID] * MAP_WIDTH + targetY[globalID]] == 1) {
		return;
	}

	// check if target if out of range
	uint distanceToTarget = calculateHeuristic(sourceX, sourceY, targetX[globalID], targetY[globalID]);
	if (distanceToTarget > MAX_RANGE) {
		return;
	}

	// create a node based on the source position
	PathNode sourceNode;
	sourceNode.x = sourceX;
	sourceNode.y = sourceY;
	sourceNode.g = 0;
	sourceNode.f = 0;
	sourceNode.ID = idCounter++;
	sourceNode.pID = 0;

	// create a node based on the target position
	PathNode targetNode;
	targetNode.x = targetX[globalID];
	targetNode.y = targetY[globalID];
	targetNode.g = 0;
	targetNode.f = 0;
	targetNode.ID = idCounter++;
	targetNode.pID = 0;

	// add source node to the open list
	//addToList(openList, openListSize, sourceNode);
	openList[openListSize] = sourceNode;
	openListSize++;

	while (openListSize > 0) {
		// make sure open/closed lists are lower than their max size
		if (openListSize >= SEARCH_LIST_SIZE || closedListSize >= SEARCH_LIST_SIZE) {
			return;
		}

		// get node from open list with lowest f score
		// because the list is sorted, the first element will have the lowest f score
		PathNode currentNode = openList[0];

		// add the current node to the closed list
		//addToList(closedList, closedListSize, currentNode);
		closedList[closedListSize] = currentNode;
		closedListSize++;

		// remove current node from open list
		bool foundNode = false;
		for (uint i = 0; i < openListSize; i++) {
			if (openList[i].x == currentNode.x && openList[i].y == currentNode.y) {
				foundNode = true;

				// zero out the node
				openList[i].x = 0;
				openList[i].y = 0;
				openList[i].g = 0;
				openList[i].f = 0;
				openList[i].ID = 0;
				openList[i].pID = 0;
			}
			if (foundNode && i < openListSize - 1) {
				openList[i] = openList[i + 1];
			}
		}
		openListSize--;

		if (listContains(closedList, closedListSize, targetNode)) {
		// found the target node, complete path found

			PathNode tmpNode = currentNode;

			// follow the path back from the target to the source, using the node's parent
			while (tmpNode.pID != 0) {
				// convert map index coords to pixel coords
				tmpNode.x *= 32;
				tmpNode.y *= 32;

				// moves tile origin to the center of the tile
				tmpNode.x += 16;
				tmpNode.y += 16;

				// add each node to the path
				//addToList(path, pathSize, tmpNode);
				path[pathSize] = tmpNode;
				pathSize++;

				if (pathSize > PATH_SIZE) {
					return;
				}

				// traverse to the next node
				for (uint i = 0; i < closedListSize; i++) {
					if (closedList[i].ID == tmpNode.pID) {
						tmpNode = closedList[i];
					}
				}
			}

			// convert map index coords to pixel coords
			tmpNode.x *= 32;
			tmpNode.y *= 32;

			// moves tile origin to the center of the tile
			tmpNode.x += 16;
			tmpNode.y += 16;

			//addToList(path, pathSize, tmpNode);
			path[pathSize] = tmpNode;
			pathSize++;

			if (pathSize > PATH_SIZE) {
				return;
			}
			int pathIndex = convert_int_rtp((pathSize - 1) * PATH_PERECNT);

			xBuffer[globalID] = path[pathIndex].x;
			yBuffer[globalID] = path[pathIndex].y;

			break;
		}

		// get all available adjacent nodes
		bool top = false;
		bool left = false;
		bool bottom = false;
		bool right = false;
		adjacentNodesSize = 0;

		// top
		int adjX = currentNode.x;
		int adjY = currentNode.y - 1;

		if (adjX > -1 && adjX < MAP_WIDTH &&
			adjY > -1 && adjY < MAP_HEIGHT) {
			PathNode nodeToAdd;
			nodeToAdd.x = adjX;
			nodeToAdd.y = adjY;
			nodeToAdd.g = 0;
			nodeToAdd.f = 0;
			nodeToAdd.ID = idCounter++;
			nodeToAdd.pID = 0;

			if (!listContains(closedList, closedListSize, nodeToAdd) && !map[nodeToAdd.x * MAP_WIDTH + nodeToAdd.y] == 1)
			{
				adjacentNodes[adjacentNodesSize] = nodeToAdd;

				adjacentNodesSize++;
				top = true;
			}
		}

		// left
		adjX = currentNode.x - 1;
		adjY = currentNode.y;
		if (adjX > -1 && adjX < MAP_WIDTH &&
			adjY > -1 && adjY < MAP_HEIGHT) {
			PathNode nodeToAdd;
			nodeToAdd.x = adjX;
			nodeToAdd.y = adjY;
			nodeToAdd.g = 0;
			nodeToAdd.f = 0;
			nodeToAdd.ID = idCounter++;
			nodeToAdd.pID = 0;

			if (!listContains(closedList, closedListSize, nodeToAdd) && !map[nodeToAdd.x * MAP_WIDTH + nodeToAdd.y] == 1) {
				adjacentNodes[adjacentNodesSize] = nodeToAdd;
				adjacentNodesSize++;
				left = true;
			}
		}

		// bottom
		adjX = currentNode.x;
		adjY = currentNode.y + 1;
		if (adjX > -1 && adjX < MAP_WIDTH &&
			adjY > -1 && adjY < MAP_HEIGHT) {
			PathNode nodeToAdd;
			nodeToAdd.x = adjX;
			nodeToAdd.y = adjY;
			nodeToAdd.g = 0;
			nodeToAdd.f = 0;
			nodeToAdd.ID = idCounter++;
			nodeToAdd.pID = 0;

			if (!listContains(closedList, closedListSize, nodeToAdd) && !map[nodeToAdd.x * MAP_WIDTH + nodeToAdd.y] == 1) {
				adjacentNodes[adjacentNodesSize] = nodeToAdd;
				adjacentNodesSize++;
				bottom = true;
			}
		}

		// right
		adjX = currentNode.x + 1;
		adjY = currentNode.y;
		if (adjX > -1 && adjX < MAP_WIDTH &&
			adjY > -1 && adjY < MAP_HEIGHT) {
			PathNode nodeToAdd;
			nodeToAdd.x = adjX;
			nodeToAdd.y = adjY;
			nodeToAdd.g = 0;
			nodeToAdd.f = 0;
			nodeToAdd.ID = idCounter++;
			nodeToAdd.pID = 0;

			if (!listContains(closedList, closedListSize, nodeToAdd) && !map[nodeToAdd.x * MAP_WIDTH + nodeToAdd.y] == 1) {
				adjacentNodes[adjacentNodesSize] = nodeToAdd;
				adjacentNodesSize++;
				right = true;
			}
		}

		// top left
		adjX = currentNode.x - 1;
		adjY = currentNode.y - 1;
		if (top && left &&
			adjX > -1 && adjX < MAP_WIDTH &&
			adjY > -1 && adjY < MAP_HEIGHT) {
			PathNode nodeToAdd;
			nodeToAdd.x = adjX;
			nodeToAdd.y = adjY;
			nodeToAdd.g = 0;
			nodeToAdd.f = 0;
			nodeToAdd.ID = idCounter++;
			nodeToAdd.pID = 0;

			if (!listContains(closedList, closedListSize, nodeToAdd) && !map[nodeToAdd.x * MAP_WIDTH + nodeToAdd.y] == 1) {
				adjacentNodes[adjacentNodesSize] = nodeToAdd;
				adjacentNodesSize++;
			}
		}

		// bottom left
		adjX = currentNode.x - 1;
		adjY = currentNode.y + 1;
		if (bottom && left &&
			adjX > -1 && adjX < MAP_WIDTH &&
			adjY > -1 && adjY < MAP_HEIGHT) {
			PathNode nodeToAdd;
			nodeToAdd.x = adjX;
			nodeToAdd.y = adjY;
			nodeToAdd.g = 0;
			nodeToAdd.f = 0;
			nodeToAdd.ID = idCounter++;
			nodeToAdd.pID = 0;

			if (!listContains(closedList, closedListSize, nodeToAdd) && !map[nodeToAdd.x * MAP_WIDTH + nodeToAdd.y] == 1) {
				adjacentNodes[adjacentNodesSize] = nodeToAdd;
				adjacentNodesSize++;
			}
		}

		// top right
		adjX = currentNode.x + 1;
		adjY = currentNode.y - 1;
		if (top && right &&
			adjX > -1 && adjX < MAP_WIDTH &&
			adjY > -1 && adjY < MAP_HEIGHT) {
			PathNode nodeToAdd;
			nodeToAdd.x = adjX;
			nodeToAdd.y = adjY;
			nodeToAdd.g = 0;
			nodeToAdd.f = 0;
			nodeToAdd.ID = idCounter++;
			nodeToAdd.pID = 0;

			if (!listContains(closedList, closedListSize, nodeToAdd) && !map[nodeToAdd.x * MAP_WIDTH + nodeToAdd.y] == 1) {
				adjacentNodes[adjacentNodesSize] = nodeToAdd;
				adjacentNodesSize++;
			}
		}

		// bottom right
		adjX = currentNode.x + 1;
		adjY = currentNode.y + 1;
		if (bottom && right &&
			adjX > -1 && adjX < MAP_WIDTH &&
			adjY > -1 && adjY < MAP_HEIGHT) {
			PathNode nodeToAdd;
			nodeToAdd.x = adjX;
			nodeToAdd.y = adjY;
			nodeToAdd.g = 0;
			nodeToAdd.f = 0;
			nodeToAdd.ID = idCounter++;
			nodeToAdd.pID = 0;

			if (!listContains(closedList, closedListSize, nodeToAdd) && !map[nodeToAdd.x * MAP_WIDTH + nodeToAdd.y] == 1) {
				adjacentNodes[adjacentNodesSize] = nodeToAdd;
				adjacentNodesSize++;
			}
		}

		for (int i = 0; i < adjacentNodesSize; i++) {
			PathNode currentAdjacentNode = adjacentNodes[i];

			// check if current adjacent node is in the closed list already
			if (listContains(closedList, closedListSize, currentAdjacentNode)) {
				// if so, ignore it
				continue;
			}

			// Check if the current adjacent node is already in the open list and save the index
			uint currentIndex = 0;
			bool isNodeInOpenList = false;
			for (int i = 0; i < openListSize; i++) {
				if (openList[i].x == currentAdjacentNode.x && openList[i].y == currentAdjacentNode.y) {
					currentIndex = i;
					isNodeInOpenList = true;
				}
			}

			if (!isNodeInOpenList) {
				// add it to the open list

				// set the current node as the parent so we can trace back a path
				currentAdjacentNode.pID = currentNode.ID;

				// g score is equal to the parent's g score + the cost to move to the target tile (currently always 1)
				currentAdjacentNode.g = currentNode.g + 1;

				// calculate f score by adding the g score to a heuristic function
				currentAdjacentNode.f = currentAdjacentNode.g + calculateHeuristic(currentAdjacentNode.x, currentAdjacentNode.y, targetX[globalID], targetY[globalID]);

				// insert into open list sorting by f score
				insertInListByFScore(openList, openListSize, currentAdjacentNode);
				openListSize++;

			} else {
				// this tile has already been calculated so retrive it using the saved index

				// check if the g score is lower
				if (currentNode.g + 1 < openList[currentIndex].g) {
					//if it is, update it with the current g score + cost
					openList[currentIndex].g = currentNode.g + 1;

					// recalculate f score
					openList[currentIndex].f = openList[currentIndex].g + calculateHeuristic(openList[currentIndex].x, openList[currentIndex].y, targetX[globalID], targetY[globalID]);

					// because f score may have changed, sort the list again
					PathNode temp;
					temp.x = 0;
					temp.y = 0;
					temp.g = 0;
					temp.f = 0;
					temp.ID = 0;
					temp.pID = 0;

					for (int x = 0; x < openListSize - 1; x++) {
						for (int y = 0; y < openListSize - x - 1; y++) {
							if (openList[y].f > openList[y + 1].f) {
								temp = openList[y];
								openList[y] = openList[y + 1];
								openList[y + 1] = temp;
							}
						}
					}
				}
			}
		}
	}
}
