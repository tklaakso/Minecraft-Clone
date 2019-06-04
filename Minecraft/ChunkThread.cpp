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

void ChunkThread::exit() {
	thread.join();
}

void ChunkThread::run() {
	while (running) {
		createDeleteMutex.lock();
		if (toCreate.size() == 0 && toDelete.size() == 0 && toFinalize.size() == 0) {
			createDeleteMutex.unlock();
			std::unique_lock<std::mutex> lock(mtx);
			cond.wait(lock);
		}
		else {
			createDeleteMutex.unlock();
		}
		if (toCreate.size() > 0) {
			createDeleteMutex.lock();
			Chunk* c = toCreate[0];
			createDeleteMutex.unlock();
			int x = c->x;
			int y = c->y;
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
			c->setVAO(vao, vaoIndex);
			c->generate(cm->generator);
			createDeleteMutex.lock();
			toCreate.erase(toCreate.begin());
			createDeleteMutex.unlock();
			c->generating = false;
			c->finalizing = true;
		}
		if (toFinalize.size() > 0) {
			createDeleteMutex.lock();
			Chunk* c = toFinalize[0];
			createDeleteMutex.unlock();
			int x = c->x;
			int y = c->y;
			if (!c->generating) {
				Chunk** neighbors = (Chunk**)malloc(sizeof(Chunk*) * 4);
				ChunkCoords left(x - 1, y, 0, 0);
				ChunkCoords right(x + 1, y, 0, 0);
				ChunkCoords front(x, y - 1, 0, 0);
				ChunkCoords back(x, y + 1, 0, 0);
				chunkMutex->lock();
				neighbors[CHUNK_NEIGHBOR_LEFT] = findChunkWithCoords(cm->chunks, &left, 0, cm->chunks->size() - 1);
				neighbors[CHUNK_NEIGHBOR_RIGHT] = findChunkWithCoords(cm->chunks, &right, 0, cm->chunks->size() - 1);
				neighbors[CHUNK_NEIGHBOR_FRONT] = findChunkWithCoords(cm->chunks, &front, 0, cm->chunks->size() - 1);
				neighbors[CHUNK_NEIGHBOR_BACK] = findChunkWithCoords(cm->chunks, &back, 0, cm->chunks->size() - 1);
				chunkMutex->unlock();
				bool neighborProcessing = false;
				for (int i = 0; i < 4; i++) {
					if (neighbors[i] != NULL && (neighbors[i]->generating || neighbors[i]->deleting)) {
						neighborProcessing = true;
						break;
					}
				}
				if (neighborProcessing) {
					createDeleteMutex.lock();
					toFinalize.push_back(c);
					createDeleteMutex.unlock();
				}
				else {
					c->updateNeighbors(neighbors);
					c->finalizing = false;
				}
				createDeleteMutex.lock();
				toFinalize.erase(toFinalize.begin());
				createDeleteMutex.unlock();
				free(neighbors);
			}
		  std::cout << "reached: " << c->generating << ", " << c->deleting << std::endl;
		}
		if (toDelete.size() > 0) {
			createDeleteMutex.lock();
			Chunk* chunk = toDelete[0];
			createDeleteMutex.unlock();
			if (chunk->generating || chunk->finalizing) {
				toDelete.erase(toDelete.begin());
				toDelete.push_back(chunk);
				continue;
			}
			if (chunk != NULL) {
				int x = chunk->x;
				int y = chunk->y;
				chunkMutex->lock();
				cm->freeVAOs.push(chunk->vaoIndex);
				delete chunk;
				chunkMutex->unlock();
				ChunkCoords leftCoords(x - 1, y, 0, 0);
				ChunkCoords rightCoords(x + 1, y, 0, 0);
				ChunkCoords frontCoords(x, y - 1, 0, 0);
				ChunkCoords backCoords(x, y + 1, 0, 0);
				chunkMutex->lock();
				Chunk* left = findChunkWithCoords(cm->chunks, &leftCoords, 0, cm->chunks->size() - 1);
				Chunk* right = findChunkWithCoords(cm->chunks, &rightCoords, 0, cm->chunks->size() - 1);
				Chunk* front = findChunkWithCoords(cm->chunks, &frontCoords, 0, cm->chunks->size() - 1);
				Chunk* back = findChunkWithCoords(cm->chunks, &backCoords, 0, cm->chunks->size() - 1);
				chunkMutex->unlock();
				if (left != NULL && !left->generating && !left->deleting) {
					left->reorderBlocks();
					left->updateVAO();
				}
				if (right != NULL && !right->generating && !right->deleting) {
					right->reorderBlocks();
					right->updateVAO();
				}
				if (front != NULL && !front->generating && !front->deleting) {
					front->reorderBlocks();
					front->updateVAO();
				}
				if (back != NULL && !back->generating && !back->deleting) {
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

void ChunkThread::createChunk(Chunk* c) {
	createDeleteMutex.lock();
	toCreate.push_back(c);
	createDeleteMutex.unlock();
	cond.notify_all();
}

void ChunkThread::finalizeChunk(Chunk* c) {
	createDeleteMutex.lock();
	toFinalize.push_back(c);
	createDeleteMutex.unlock();
	cond.notify_all();
}

void ChunkThread::deleteChunk(Chunk* c) {
	createDeleteMutex.lock();
	toDelete.push_back(c);
	createDeleteMutex.unlock();
	cond.notify_all();
}

ChunkThread::~ChunkThread()
{
	running = false;
	thread.join();
}
