#include "ChunkCoords.h"



ChunkCoords::ChunkCoords(int chunkX, int chunkZ, int localX, int localZ)
{
	this->chunkX = chunkX;
	this->chunkZ = chunkZ;
	this->localX = localX;
	this->localZ = localZ;
}


ChunkCoords::~ChunkCoords()
{
}
