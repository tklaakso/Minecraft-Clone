#pragma once

class Block;

class BlockCastInfo
{
public:
	BlockCastInfo(Block* block, int face);
	~BlockCastInfo();
	Block* block;
	int face;
};

