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

void ChunkThreadPool::exit() {
	for (int i = 0; i < numThreads; i++) {
		threads[i]->exit();
	}
}

void ChunkThreadPool::createChunk(Chunk* c) {
	threads[threadIndex]->createChunk(c);
	threadIndex++;
	threadIndex %= numThreads;
}

void ChunkThreadPool::finalizeChunk(Chunk* c) {
	threads[threadIndex]->finalizeChunk(c);
	threadIndex++;
	threadIndex %= numThreads;
}

void ChunkThreadPool::deleteChunk(Chunk* c) {
	threads[threadIndex]->deleteChunk(c);
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
