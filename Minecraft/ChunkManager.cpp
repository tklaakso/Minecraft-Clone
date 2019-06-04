#include "ChunkManager.h"

ChunkManager::ChunkManager()
{
	VAOs = (ChunkVAO**)malloc(sizeof(ChunkVAO*) * NUM_CHUNK_VAOS);
	for (int i = 0; i < NUM_CHUNK_VAOS; i++) {
		ChunkVAO* vao = new ChunkVAO();
		VAOs[i] = vao;
	}
	generator = new WorldGenerator();
	chunks = new std::vector<Chunk*>();
}

ChunkManager::~ChunkManager()
{
	for (int i = 0; i < NUM_CHUNK_VAOS; i++) {
		delete VAOs[i];
	}
	free(VAOs);
	delete generator;
	delete chunks;
}
