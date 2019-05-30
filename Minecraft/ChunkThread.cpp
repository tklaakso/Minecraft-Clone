#include "ChunkThread.h"

ChunkThread::ChunkThread(ChunkManager* cm)
{
	running = true;
	this->cm = cm;
}

void ChunkThread::initialize(std::mutex* chunkMutex) {
	this->chunkMutex = chunkMutex;
	thread = std::thread(&ChunkThread::run, this);
}

void ChunkThread::run() {
	while (running) {
		createDeleteMutex.lock();
		if (toCreate.size() == 0 && toDelete.size() == 0) {
			createDeleteMutex.unlock();
			std::unique_lock<std::mutex> lock(mtx);
			cond.wait(lock);
		}
		else {
			createDeleteMutex.unlock();
		}
		while (toCreate.size() > 0) {
			createDeleteMutex.lock();
			ChunkCoords coords = toCreate[0];
			createDeleteMutex.unlock();
			int x = coords.chunkX;
			int y = coords.chunkZ;
			ChunkVAO* vao;
			int vaoIndex;
			if (!cm->freeVAOs.empty()) {
				chunkMutex->lock();
				vaoIndex = cm->freeVAOs.front();
				vao = cm->VAOs[vaoIndex];
				cm->freeVAOs.pop();
				chunkMutex->unlock();
			}
			else {
				chunkMutex->lock();
				vaoIndex = cm->numUsedVAOs;
				vao = cm->VAOs[vaoIndex];
				cm->numUsedVAOs++;
				chunkMutex->unlock();
			}
			Chunk* c = new Chunk(x, y, vao, vaoIndex);
			Chunk** neighbors = (Chunk**)malloc(sizeof(Chunk*) * 4);
			ChunkCoords left(c->x - 1, c->y, 0, 0);
			ChunkCoords right(c->x + 1, c->y, 0, 0);
			ChunkCoords front(c->x, c->y - 1, 0, 0);
			ChunkCoords back(c->x, c->y + 1, 0, 0);
			chunkMutex->lock();
			neighbors[CHUNK_NEIGHBOR_LEFT] = findChunkWithCoords(cm->chunks, &left, 0, cm->chunks.size() - 1);
			neighbors[CHUNK_NEIGHBOR_RIGHT] = findChunkWithCoords(cm->chunks, &right, 0, cm->chunks.size() - 1);
			neighbors[CHUNK_NEIGHBOR_FRONT] = findChunkWithCoords(cm->chunks, &front, 0, cm->chunks.size() - 1);
			neighbors[CHUNK_NEIGHBOR_BACK] = findChunkWithCoords(cm->chunks, &back, 0, cm->chunks.size() - 1);
			chunkMutex->unlock();
			c->generate(cm->generator, neighbors);
			free(neighbors);
			if (cm->chunks.size() == 0) {
				chunkMutex->lock();
				cm->chunks.push_back(c);
				chunkMutex->unlock();
				continue;
			}
			bool shouldContinue = false;
			chunkMutex->lock();
			for (int i = 0; i < cm->chunks.size(); i++) {
				int cmp = cm->chunks[i]->compare(c);
				if (cmp == 0) {
					std::cout << "Error: tried to create existing chunk" << std::endl;
					shouldContinue = true;
					break;
				}
				if (cmp > 0) {
					cm->chunks.insert(cm->chunks.begin() + i, c);
					shouldContinue = true;
					break;
				}
			}
			chunkMutex->unlock();
			createDeleteMutex.lock();
			toCreate.erase(toCreate.begin());
			createDeleteMutex.unlock();
			if (shouldContinue) {
				continue;
			}
			chunkMutex->lock();
			cm->chunks.push_back(c);
			chunkMutex->unlock();
		}
		while (toDelete.size() > 0) {
			createDeleteMutex.lock();
			ChunkCoords coords = toDelete[0];
			createDeleteMutex.unlock();
			int x = coords.chunkX;
			int y = coords.chunkZ;
			chunkMutex->lock();
			Chunk* chunk = findChunkWithCoords(cm->chunks, &coords, 0, cm->chunks.size() - 1);
			chunkMutex->unlock();
			if (chunk != NULL) {
				chunkMutex->lock();
				for (int i = 0; i < cm->chunks.size(); i++) {
					if (cm->chunks[i] == chunk) {
						cm->chunks.erase(cm->chunks.begin() + i);
						break;
					}
				}
				cm->freeVAOs.push(chunk->vaoIndex);
				chunkMutex->unlock();
				delete chunk;
				ChunkCoords leftCoords(x - 1, y, 0, 0);
				ChunkCoords rightCoords(x + 1, y, 0, 0);
				ChunkCoords frontCoords(x, y - 1, 0, 0);
				ChunkCoords backCoords(x, y + 1, 0, 0);
				chunkMutex->lock();
				Chunk* left = findChunkWithCoords(cm->chunks, &leftCoords, 0, cm->chunks.size() - 1);
				Chunk* right = findChunkWithCoords(cm->chunks, &rightCoords, 0, cm->chunks.size() - 1);
				Chunk* front = findChunkWithCoords(cm->chunks, &frontCoords, 0, cm->chunks.size() - 1);
				Chunk* back = findChunkWithCoords(cm->chunks, &backCoords, 0, cm->chunks.size() - 1);
				chunkMutex->unlock();
				if (left != NULL) {
					left->reorderBlocks();
					left->updateVAO();
				}
				if (right != NULL) {
					right->reorderBlocks();
					right->updateVAO();
				}
				if (front != NULL) {
					front->reorderBlocks();
					front->updateVAO();
				}
				if (back != NULL) {
					back->reorderBlocks();
					back->updateVAO();
				}
			}
			else {
				std::cout << "Error: tried to delete nonexistent chunk" << std::endl;
			}
			toDelete.erase(toDelete.begin());
		}
	}
}

void ChunkThread::createChunk(ChunkCoords coords) {
	createDeleteMutex.lock();
	toCreate.push_back(coords);
	createDeleteMutex.unlock();
	cond.notify_all();
}

void ChunkThread::deleteChunk(ChunkCoords coords) {
	createDeleteMutex.lock();
	toDelete.push_back(coords);
	createDeleteMutex.unlock();
	cond.notify_all();
}

ChunkThread::~ChunkThread()
{
	running = false;
	thread.join();
}
