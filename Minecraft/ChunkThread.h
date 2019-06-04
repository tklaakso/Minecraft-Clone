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
	void exit();
	void createChunk(Chunk* c);
	void finalizeChunk(Chunk* c);
	void deleteChunk(Chunk* c);
	~ChunkThread();
private:
	void run();
	std::vector<Chunk*> toCreate;
	std::vector<Chunk*> toFinalize;
	std::vector<Chunk*> toDelete;
	ChunkManager* cm;
	std::mutex createDeleteMutex;
	std::mutex* chunkMutex;
	std::mutex mtx;
	std::condition_variable cond;
	std::thread thread;
	bool running;
};
