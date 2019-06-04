#pragma once
#include <mutex>
#include "ChunkThread.h"

class ChunkThreadPool
{
public:
	ChunkThreadPool(ChunkManager* cm, int numThreads);
	void createChunk(Chunk* c);
	void finalizeChunk(Chunk* c);
	void deleteChunk(Chunk* c);
	void exit();
	~ChunkThreadPool();
private:
	ChunkThread** threads;
	std::mutex chunkMutex;
	int numThreads;
	int threadIndex;
};

