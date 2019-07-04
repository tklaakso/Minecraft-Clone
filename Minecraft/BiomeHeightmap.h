#pragma once

/*
Used for determining the height of specific biome types at given block x,z coordinates
*/
class BiomeHeightmap
{
public:

	BiomeHeightmap();

	/*
	Get the height of a specific integer-coded biome type at block x,z coordinates
	*/
	virtual int getHeight(int biome, int x, int z) = 0;

	int getNumBiomes();

	~BiomeHeightmap();

protected:

	/*
	Set number of distinct biome types represented by this biome heightmap
	*/
	void setNumBiomes(int numBiomes);

private:

	int numBiomes;

};

