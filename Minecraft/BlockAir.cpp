#include "BlockAir.h"

BlockAir::BlockAir(int x, int y, int z) : InvisibleBlock(BLOCK_AIR, x, y, z)
{
}

Block* BlockAir::clone() {
	return new BlockAir(x, y, z);
}

BlockAir::~BlockAir()
{
}
