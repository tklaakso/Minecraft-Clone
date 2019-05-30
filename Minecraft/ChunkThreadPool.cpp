#include "ChunkThreadPool.h"

ChunkThreadPool::ChunkThreadPool(ChunkManager* cm, int numThreads)
{
	this->numThreads = numThreads;
	threadIndex = 0;
	threads = (ChunkThread**)malloc(sizeof(ChunkThread*) * numThreads);
	for (int i = 0; i < numThreads; i++) {
		threads[i] = new ChunkThread(cm);
		threads[i]->initialize(&chunkMutex);
	}
}

void ChunkThreadPool::createChunk(ChunkCoords coords) {
	threads[threadIndex]->createChunk(coords);
	threadIndex++;
	threadIndex %= numThreads;
}

void ChunkThreadPool::deleteChunk(ChunkCoords coords) {
	threads[threadIndex]->deleteChunk(coords);
	threadIndex++;
	threadIndex %= numThreads;
}

ChunkThreadPool::~ChunkThreadPool()
{
	for (int i = 0; i < numThreads; i++) {
		delete threads[i];
	}
	free(threads);
}
