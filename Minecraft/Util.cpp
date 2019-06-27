#include "Util.h"
#include "BlockAir.h"
#include "BlockDirt.h"
#include "BlockGrass.h"
#include "BlockLeaf.h"
#include "BlockLightGrass.h"
#include "BlockGravel.h"
#include "BlockMagma.h"
#include "BlockWater.h"
#include "BlockGoldOre.h"
#include "BlockIronOre.h"
#include "BlockDiamondOre.h"
#include "BlockCoalOre.h"
#include "BlockLog.h"
#include "BlockSand.h"
#include "BlockCactus.h"
#include "BlockSandstone.h"
#include "BlockLeafOrange.h"
#include "BlockLeafRed.h"

std::string toString(int i) {
	std::string outString;
	std::stringstream ss;
	ss << i;
	outString = ss.str();
	return outString;
}

std::vector<std::string> split(std::string s, std::string delim) {
	std::vector<std::string> vs;
	int start = 0;
	int end = s.find(delim);
	while (end != std::string::npos) {
		vs.push_back(s.substr(start, end - start));
		start = end + delim.length();
		end = s.find(delim, start);
	}
	vs.push_back(s.substr(start, end));
	return vs;
}

int blockCoordsToIndex(int bx, int by, int bz) {
	return bx + bz * CHUNK_WIDTH + by * CHUNK_WIDTH * CHUNK_WIDTH;
}

BlockCoords indexToBlockCoords(int index) {
	int bx = index % CHUNK_WIDTH;
	int bz = ((index - bx) % (CHUNK_WIDTH * CHUNK_WIDTH)) / CHUNK_WIDTH;
	int by = (index - bx - bz * CHUNK_WIDTH) / (CHUNK_WIDTH * CHUNK_WIDTH);
	return BlockCoords(bx, by, bz);
}

Chunk* findChunkWithCoords(std::vector<Chunk*>* chunks, ChunkCoords* coords, int l, int r) {
	if (r >= l) {
		int mid = l + (r - l) / 2;
		int cmp = (*chunks)[mid]->compare(coords);
		if (cmp == 0) {
			return (*chunks)[mid];
		}
		if (cmp > 0) {
			return findChunkWithCoords(chunks, coords, l, mid - 1);
		}
		return findChunkWithCoords(chunks, coords, mid + 1, r);
	}
	return NULL;
}

ChunkCoords blockToChunkCoords(int bx, int bz) {
	int chunkX = (int)floor((double)bx / CHUNK_WIDTH);
	int chunkZ = (int)floor((double)bz / CHUNK_WIDTH);
	int localX = bx - chunkX * CHUNK_WIDTH;
	int localZ = bz - chunkZ * CHUNK_WIDTH;
	return ChunkCoords(chunkX, chunkZ, localX, localZ);
}

RegionCoords chunkToRegionCoords(int cx, int cz) {
	RegionCoords r((int)floor((double)cx / REGION_WIDTH_CHUNKS), (int)floor((double)cz / REGION_WIDTH_CHUNKS));
	return r;
}

Block* makeBlock(int id, int x, int y, int z, Chunk* parent) {
	switch (id) {
	case BLOCK_AIR:
		return (new BlockAir(x, y, z))->setParent(parent);
	case BLOCK_DIRT:
		return (new BlockDirt(x, y, z))->setParent(parent);
	case BLOCK_GRASS:
		return (new BlockGrass(x, y, z))->setParent(parent);
	case BLOCK_LEAF:
		return (new BlockLeaf(x, y, z))->setParent(parent);
	case BLOCK_LIGHT_GRASS:
		return (new BlockLightGrass(x, y, z))->setParent(parent);
	case BLOCK_GRAVEL:
		return (new BlockGravel(x, y, z))->setParent(parent);
	case BLOCK_MAGMA:
		return (new BlockMagma(x, y, z))->setParent(parent);
	case BLOCK_WATER:
		return (new BlockWater(x, y, z))->setParent(parent);
	case BLOCK_GOLD_ORE:
		return (new BlockGoldOre(x, y, z))->setParent(parent);
	case BLOCK_IRON_ORE:
		return (new BlockIronOre(x, y, z))->setParent(parent);
	case BLOCK_DIAMOND_ORE:
		return (new BlockDiamondOre(x, y, z))->setParent(parent);
	case BLOCK_COAL_ORE:
		return (new BlockCoalOre(x, y, z))->setParent(parent);
	case BLOCK_LOG:
		return (new BlockLog(x, y, z))->setParent(parent);
	case BLOCK_SAND:
		return (new BlockSand(x, y, z))->setParent(parent);
	case BLOCK_CACTUS:
		return (new BlockCactus(x, y, z))->setParent(parent);
	case BLOCK_SANDSTONE:
		return (new BlockSandstone(x, y, z))->setParent(parent);
	case BLOCK_LEAF_ORANGE:
		return (new BlockLeafOrange(x, y, z))->setParent(parent);
	case BLOCK_LEAF_RED:
		return (new BlockLeafRed(x, y, z))->setParent(parent);
	default:
		return (new BlockAir(x, y, z))->setParent(parent);
	}
}

Block* clone(Block* b) {
	assert(b != NULL);
	return makeBlock(b->getId(), b->getX(), b->getY(), b->getZ(), b->getParent());
}