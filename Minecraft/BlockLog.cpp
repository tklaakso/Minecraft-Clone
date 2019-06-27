#include "BlockLog.h"

BlockLog::BlockLog(int x, int y, int z) : Block(BLOCK_LOG, x, y, z)
{
}

bool BlockLog::shouldRenderType() {
	return renderType;
}

BlockLog::~BlockLog()
{
}
