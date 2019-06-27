#include "BlockCactus.h"

BlockCactus::BlockCactus(int x, int y, int z) : Block(BLOCK_CACTUS, x, y, z)
{
}

bool BlockCactus::shouldRenderType() {
	return renderType;
}

BlockCactus::~BlockCactus()
{
}
