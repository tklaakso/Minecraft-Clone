#pragma once
class BiomeHeightmap
{
public:
	BiomeHeightmap();
	virtual int getHeight(int biome, int x, int z) = 0;
	int getNumBiomes();
	~BiomeHeightmap();
protected:
	void setNumBiomes(int numBiomes);
private:
	int numBiomes;
};

