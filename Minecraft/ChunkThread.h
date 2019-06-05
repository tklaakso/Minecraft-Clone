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
	ChunkThread(ChunkManager *cm, std::vector<Chunk*>* creationQueue, std::vector<Chunk*>* deletionQueue);
	void initialize(std::mutex* chunkMutex);
	void updateCreationQueue();
	void updateDeletionQueue();
	void exit();
	~ChunkThread();
private:
	void run();
	std::vector<Chunk*>* creationQueue;
	std::vector<Chunk*>* deletionQueue;
	ChunkManager* cm;
	std::mutex createDeleteMutex;
	std::mutex* chunkMutex;
	std::mutex mtx;
	std::condition_variable cond;
	std::thread thread;
	bool running;
};
