#include "Util.h"

std::string toString(int i) {
	std::string outString;
	std::stringstream ss;
	ss << i;
	outString = ss.str();
	return outString;
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