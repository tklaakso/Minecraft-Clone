#include "BlockMagma.h"

BlockMagma::BlockMagma(int x, int y, int z) : Block(BLOCK_MAGMA, x, y, z)
{
}

bool BlockMagma::shouldRenderType() {
	return renderType;
}

Block* BlockMagma::clone() {
	return new BlockMagma(x, y, z);
}

BlockMagma::~BlockMagma()
{
}
