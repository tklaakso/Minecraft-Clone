#pragma once
#include <mutex>
#include "ChunkThread.h"

class ChunkThreadPool
{
public:
	ChunkThreadPool(ChunkManager* cm, int numThreads);
	void createChunk(Chunk* c);
	void deleteChunk(Chunk* c);
	void exit();
	~ChunkThreadPool();
private:
	ChunkThread** threads;
	ChunkManager* cm;
	std::mutex chunkMutex;
	std::vector<Chunk*>* creationQueue;
	std::vector<Chunk*>* deletionQueue;
	int numThreads;
	int threadIndex;
};

