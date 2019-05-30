#pragma once
#include <mutex>
#include "ChunkThread.h"

class ChunkThreadPool
{
public:
	ChunkThreadPool(ChunkManager* cm, int numThreads);
	void createChunk(ChunkCoords coords);
	void deleteChunk(ChunkCoords coords);
	~ChunkThreadPool();
private:
	ChunkThread** threads;
	std::mutex chunkMutex;
	int numThreads;
	int threadIndex;
};

