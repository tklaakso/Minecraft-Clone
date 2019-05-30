#include "ChunkManager.h"

ChunkManager::ChunkManager()
{
	VAOs = (ChunkVAO**)malloc(sizeof(ChunkVAO*) * NUM_CHUNK_VAOS);
	for (int i = 0; i < NUM_CHUNK_VAOS; i++) {
		ChunkVAO* vao = new ChunkVAO();
		VAOs[i] = vao;
	}
	generator = new WorldGenerator();
}

ChunkManager::~ChunkManager()
{
	for (int i = 0; i < NUM_CHUNK_VAOS; i++) {
		free(VAOs[i]);
	}
	free(VAOs);
	delete generator;
}
