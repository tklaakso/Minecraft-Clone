#pragma once
class ChunkCoords
{
public:
	ChunkCoords(int chunkX, int chunkZ, int localX, int localZ);
	int chunkX;
	int chunkZ;
	int localX;
	int localZ;
	~ChunkCoords();
};

