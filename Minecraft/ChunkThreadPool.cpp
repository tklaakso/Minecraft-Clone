#include "ChunkThreadPool.h"

ChunkThreadPool::ChunkThreadPool(ChunkManager* cm, int numThreads)
{
	this->numThreads = numThreads;
	this->cm = cm;
	creationQueue = new std::vector<Chunk*>();
	deletionQueue = new std::vector<Chunk*>();
	threadIndex = 0;
	threads = (ChunkThread**)malloc(sizeof(ChunkThread*) * numThreads);
	for (int i = 0; i < numThreads; i++) {
		threads[i] = new ChunkThread(cm, creationQueue, deletionQueue);
		threads[i]->initialize(&chunkMutex);
	}
}

void ChunkThreadPool::exit() {
	for (int i = 0; i < numThreads; i++) {
		threads[i]->exit();
	}
}

void ChunkThreadPool::createChunk(Chunk* c) {
	for (int i = 0; i < 4; i++) {
		ChunkCoords coords(0, 0, 0, 0);
		if (i == CHUNK_NEIGHBOR_LEFT) {
			coords.chunkX = c->x - 1;
			coords.chunkZ = c->y;
		}
		else if (i == CHUNK_NEIGHBOR_RIGHT) {
			coords.chunkX = c->x + 1;
			coords.chunkZ = c->y;
		}
		else if (i == CHUNK_NEIGHBOR_FRONT) {
			coords.chunkX = c->x;
			coords.chunkZ = c->y - 1;
		}
		else if (i == CHUNK_NEIGHBOR_BACK) {
			coords.chunkX = c->x;
			coords.chunkZ = c->y + 1;
		}
		c->neighbors[i] = findChunkWithCoords(cm->chunks, &coords, 0, cm->chunks->size() - 1);
		if (c->neighbors[i] != NULL) {
			if (i == CHUNK_NEIGHBOR_LEFT) {
				c->neighbors[i]->neighbors[CHUNK_NEIGHBOR_RIGHT] = c;
			}
			else if (i == CHUNK_NEIGHBOR_RIGHT) {
				c->neighbors[i]->neighbors[CHUNK_NEIGHBOR_LEFT] = c;
			}
			else if (i == CHUNK_NEIGHBOR_FRONT) {
				c->neighbors[i]->neighbors[CHUNK_NEIGHBOR_BACK] = c;
			}
			else if (i == CHUNK_NEIGHBOR_BACK) {
				c->neighbors[i]->neighbors[CHUNK_NEIGHBOR_FRONT] = c;
			}
		}
	}
	chunkMutex.lock();
	creationQueue->push_back(c);
	chunkMutex.unlock();
}

void ChunkThreadPool::deleteChunk(Chunk* c) {
	chunkMutex.lock();
	deletionQueue->push_back(c);
	chunkMutex.unlock();
}

ChunkThreadPool::~ChunkThreadPool()
{
	delete creationQueue;
	delete deletionQueue;
	for (int i = 0; i < numThreads; i++) {
		delete threads[i];
	}
	free(threads);
}
