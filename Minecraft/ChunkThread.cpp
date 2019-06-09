#include "ChunkThread.h"

ChunkThread::ChunkThread(ChunkManager* cm, std::vector<Chunk*>* creationQueue, std::vector<Chunk*>* deletionQueue)
{
	running = true;
	this->cm = cm;
	this->creationQueue = creationQueue;
	this->deletionQueue = deletionQueue;
}

void ChunkThread::initialize(std::mutex* chunkMutex, std::mutex* chunkManagerMutex) {
	this->chunkMutex = chunkMutex;
	this->chunkManagerMutex = chunkManagerMutex;
	thread = std::thread(&ChunkThread::run, this);
}

void ChunkThread::exit() {
	thread.join();
}

void ChunkThread::updateCreationQueue() {
	chunkMutex->lock();
	if (creationQueue->size() > 0) {
		Chunk* c = (*creationQueue)[0];
		std::cout << "Create state: " << c->state << std::endl;
		creationQueue->erase(creationQueue->begin());
		for (int i = 0; i < creationQueue->size(); i++) {
			if ((*creationQueue)[i]->compare(c) == 0) {
				chunkMutex->unlock();
				return;
			}
		}
		chunkMutex->unlock();
		if (c->state == Chunk::WAITING_FOR_GENERATE) {
			c->state = Chunk::GENERATING;
			ChunkVAO* vao;
			int vaoIndex;
			chunkManagerMutex->lock();
			if (!cm->freeVAOs.empty()) {
				vaoIndex = cm->freeVAOs.front();
				vao = cm->VAOs[vaoIndex];
				cm->freeVAOs.pop();
			}
			else {
				vaoIndex = cm->numUsedVAOs;
				vao = cm->VAOs[vaoIndex];
				cm->numUsedVAOs++;
			}
			WorldGenerator* generator = cm->generator;
			chunkManagerMutex->unlock();
			c->setVAO(vao, vaoIndex);
			c->generate(generator);
			c->state = Chunk::WAITING_FOR_FINALIZE;
			chunkMutex->lock();
			creationQueue->push_back(c);
			chunkMutex->unlock();
		}
		else if (c->state == Chunk::WAITING_FOR_FINALIZE) {
			Chunk** neighbors = c->neighbors;
			bool neighborProcessing = false;
			for (int i = 0; i < 4; i++) {
				if (neighbors[i] != NULL) {
					int state = neighbors[i]->state;
					if (state == Chunk::WAITING_FOR_GENERATE || state == Chunk::GENERATING || state == Chunk::DELETING) {
						neighborProcessing = true;
						break;
					}
				}
			}
			if (neighborProcessing) {
				chunkMutex->lock();
				creationQueue->push_back(c);
				chunkMutex->unlock();
			}
			else {
				c->state = Chunk::FINALIZING;
				c->updateNeighbors();
				c->state = Chunk::RENDERING;
			}
		}
	}
	else {
		chunkMutex->unlock();
	}
}

void ChunkThread::updateDeletionQueue() {
	chunkMutex->lock();
	if (deletionQueue->size() > 0) {
		Chunk* c = (*deletionQueue)[0];
		deletionQueue->erase(deletionQueue->begin());
		for (int i = 0; i < deletionQueue->size(); i++) {
			if ((*deletionQueue)[i]->compare(c) == 0) {
				chunkMutex->unlock();
				return;
			}
		}
		chunkMutex->unlock();
		if (c->state != Chunk::RENDERING) {
			std::cout << "State: " << c->state << ", deletion size: " << deletionQueue->size() << ", creation size: " << creationQueue->size() << std::endl;
			chunkMutex->lock();
			deletionQueue->push_back(c);
			chunkMutex->unlock();
		}
		else {
			Chunk** neighbors = c->neighbors;
			bool neighborProcessing = false;
			for (int i = 0; i < 4; i++) {
				if (neighbors[i] != NULL) {
					int state = neighbors[i]->state;
					if (state != Chunk::RENDERING) {
						neighborProcessing = true;
						break;
					}
				}
			}
			if (!neighborProcessing) {
				c->state = Chunk::DELETING;
				chunkManagerMutex->lock();
				cm->freeVAOs.push(c->vaoIndex);
				for (int i = 0; i < cm->chunks->size(); i++) {
					if ((*(cm->chunks))[i] == c) {
						cm->chunks->erase(cm->chunks->begin() + i);
						break;
					}
				}
				chunkManagerMutex->unlock();
				delete c;
			}
		}
	}
	else {
		chunkMutex->unlock();
	}
}

void ChunkThread::run() {
	while (running) {
		updateCreationQueue();
		updateDeletionQueue();
		/*createDeleteMutex.lock();
		if (creationQueue->size() == 0 && deletionQueue->size() == 0) {
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
		}*/
	}
}

ChunkThread::~ChunkThread()
{
	running = false;
	thread.join();
}
