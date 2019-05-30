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

Chunk* findChunkWithCoords(std::vector<Chunk*> chunks, ChunkCoords* coords, int l, int r) {
	if (r >= l) {
		int mid = l + (r - l) / 2;
		int cmp = chunks[mid]->compare(coords);
		if (cmp == 0) {
			return chunks[mid];
		}
		if (cmp > 0) {
			return findChunkWithCoords(chunks, coords, l, mid - 1);
		}
		return findChunkWithCoords(chunks, coords, mid + 1, r);
	}
	return NULL;
}