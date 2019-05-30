#pragma once
#include <thread>
#include <mutex>
#include <vector>
#include "ChunkCoords.h"
#include "Chunk.h"
#include "ChunkManager.h"
#include "Util.h"

class ChunkThread
{
public:
	ChunkThread(ChunkManager *cm);
	void initialize(std::mutex* chunkMutex);
	void createChunk(ChunkCoords coords);
	void deleteChunk(ChunkCoords coords);
	~ChunkThread();
private:
	void run();
	std::vector<ChunkCoords> toCreate;
	std::vector<ChunkCoords> toDelete;
	ChunkManager* cm;
	std::mutex createDeleteMutex;
	std::mutex* chunkMutex;
	std::mutex mtx;
	std::condition_variable cond;
	std::thread thread;
	bool running;
};
