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
		//std::cout << "Create state: " << c->state << std::endl;
		creationQueue->erase(creationQueue->begin());
		for (int i = 0; i < creationQueue->size(); i++) {
			if ((*creationQueue)[i]->compare(c) == 0) {
				chunkMutex->unlock();
				return;
			}
		}
		chunkMutex->unlock();
		if (c->state == Chunk::WAITING_FOR_GENERATE) {
			chunkManagerMutex->lock();
			int chunksActive = Chunk::chunksActive;
			chunkManagerMutex->unlock();
			if (chunksActive < 400) {
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
			else {
				chunkMutex->lock();
				creationQueue->push_back(c);
				chunkMutex->unlock();
			}
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
				c->updateBlockNeighbors();
				c->calculateLighting();
				c->makeLightmap();
				c->reorderBlocks();
				c->updateVAO();
				c->bakeNeighbors();
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
			//std::cout << "State: " << c->state << ", deletion size: " << deletionQueue->size() << ", creation size: " << creationQueue->size() << std::endl;
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
		updateDeletionQueue();
		updateCreationQueue();
	}
}

ChunkThread::~ChunkThread()
{
	running = false;
	thread.join();
}
