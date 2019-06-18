#include "Region.h"

Region::Region(int x, int z)
{
	this->x = x;
	this->z = z;
	blocks = (std::vector<Block*>**)malloc(sizeof(std::vector<Block*>*) * REGION_WIDTH_CHUNKS);
	for (int i = 0; i < REGION_WIDTH_CHUNKS; i++) {
		blocks[i] = (std::vector<Block*>*)malloc(sizeof(std::vector<Block*>) * REGION_WIDTH_CHUNKS);
	}
}

void Region::generate() {
	std::vector<Structure*>* structures = Structure::getStructureList();
	for (int s = 0; s < structures->size(); s++) {
		Structure* str = (*structures)[s];
		
	}
}

Region::~Region()
{
	for (int i = 0; i < REGION_WIDTH_CHUNKS; i++) {
		free(blocks[i]);
	}
	free(blocks);
}
