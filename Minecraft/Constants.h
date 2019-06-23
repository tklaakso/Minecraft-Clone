#pragma once
#ifndef CONSTANTS_H
#define CONSTANTS_H

const int NUM_BLOCK_UVS = 48;

const float BLOCK_UVS[] = {
	// Front
	0.0f, 0.0f, // 0
	0.3333333f, 0.0f, // 1
	0.3333333f, 0.3333333f, // 2
	0.0f, 0.3333333f, // 3
	// Back
	0.6666666f, 0.0f, // 4
	1.0f, 0.0f, // 5
	1.0f, 0.3333333f, // 6
	0.6666666f, 0.3333333f, // 7
	// Left
	0.6666666f, 0.3333333f, // 8
	0.3333333f, 0.3333333f, // 9
	0.3333333f, 0.6666666f, // 10
	0.6666666f, 0.6666666f, // 11
	// Right
	1.0f, 0.3333333f, // 12
	0.6666666f, 0.3333333f, // 13
	0.6666666f, 0.6666666f, // 14
	1.0f, 0.6666666f, // 15
	// Top
	0.3333333f, 0.0f, // 16
	0.3333333f, 0.3333333f, // 17
	0.6666666f, 0.3333333f, // 18
	0.6666666f, 0.0f, // 19
	// Bottom
	0.0f, 0.3333333f, // 20
	0.0f, 0.6666666f, // 21
	0.3333333f, 0.6666666f, // 22
	0.3333333f, 0.3333333f // 23
};

const float BLOCK_VERTICES[] = {
	// Front
	-0.5f, -0.5f, -0.5f, // 0
	0.5f, -0.5f, -0.5f, // 1
	0.5f, 0.5f, -0.5f, // 2
	-0.5f, 0.5f, -0.5f, // 3
	// Back
	-0.5f, -0.5f, 0.5f, // 4
	0.5f, -0.5f, 0.5f, // 5
	0.5f, 0.5f, 0.5f, // 6
	-0.5f, 0.5f, 0.5f, // 7
	// Left
	-0.5f, -0.5f, -0.5f, // 8
	-0.5f, -0.5f, 0.5f, // 9
	-0.5f, 0.5f, 0.5f, // 10
	-0.5f, 0.5f, -0.5f, // 11
	// Right
	0.5f, -0.5f, -0.5f, // 12
	0.5f, -0.5f, 0.5f, // 13
	0.5f, 0.5f, 0.5f, // 14
	0.5f, 0.5f, -0.5f, // 15
	// Top
	-0.5f, 0.5f, -0.5f, // 16
	-0.5f, 0.5f, 0.5f, // 17
	0.5f, 0.5f, 0.5f, // 18
	0.5f, 0.5f, -0.5f, // 19
	// Bottom
	-0.5f, -0.5f, -0.5f, // 20
	-0.5f, -0.5f, 0.5f, // 21
	0.5f, -0.5f, 0.5f, // 22
	0.5f, -0.5f, -0.5f // 23
};

const int BLOCK_INDICES[] = {
		0, 2, 1,
		0, 3, 2,
		4, 5, 6,
		4, 6, 7,
		8, 9, 10,
		8, 10, 11,
		12, 14, 13,
		12, 15, 14,
		16, 17, 18,
		16, 18, 19,
		20, 22, 21,
		20, 23, 22
};

const int NUM_TEXTURES = 17;
const int BLOCK_TEXTURE_WIDTH = 48;
const int BLOCK_TEXTURE_HEIGHT = 48;

const int NUM_CHUNK_VAOS = 512;
const int CHUNK_HEIGHT = 128;
const int CHUNK_WIDTH = 16;
const int BLOCKS_PER_CHUNK = CHUNK_WIDTH * CHUNK_WIDTH * CHUNK_HEIGHT;

const int REGION_WIDTH = 1024;
const int REGION_WIDTH_CHUNKS = REGION_WIDTH / CHUNK_WIDTH;

const int REGION_GENERATION_DISTANCE = 1;

const int TEXTURE_ATLAS_WIDTH = 10;
const int TEXTURE_ATLAS_HEIGHT = 10;

const int CHUNK_NEIGHBOR_LEFT = 0;
const int CHUNK_NEIGHBOR_RIGHT = 1;
const int CHUNK_NEIGHBOR_FRONT = 2;
const int CHUNK_NEIGHBOR_BACK = 3;

const int BLOCK_NEIGHBOR_LEFT = 0;
const int BLOCK_NEIGHBOR_RIGHT = 1;
const int BLOCK_NEIGHBOR_UP = 2;
const int BLOCK_NEIGHBOR_DOWN = 3;
const int BLOCK_NEIGHBOR_FRONT = 4;
const int BLOCK_NEIGHBOR_BACK = 5;

const int BLOCK_DIRT = 0;
const int BLOCK_GRASS = 1;
const int BLOCK_LEAF = 2;
const int BLOCK_LIGHT_GRASS = 3;
const int BLOCK_GRAVEL = 4;
const int BLOCK_MAGMA = 5;
const int BLOCK_WATER = 6;
const int BLOCK_GOLD_ORE = 7;
const int BLOCK_IRON_ORE = 8;
const int BLOCK_DIAMOND_ORE = 9;
const int BLOCK_COAL_ORE = 10;
const int BLOCK_LOG = 11;
const int BLOCK_SAND = 12;
const int BLOCK_CACTUS = 13;
const int BLOCK_SANDSTONE = 14;
const int BLOCK_LEAF_ORANGE = 15;
const int BLOCK_LEAF_RED = 16;

const int BIOME_FOREST = 0;
const int BIOME_DESERT = 1;
const int BIOME_JUNGLE = 2;
const int BIOME_PLAINS = 3;

const float MOUSE_SENSITIVITY = 0.1f;

const float BIOME_HEIGHT_BLEND = 0.02f;
const float BIOME_SPARSITY = 5.0f;

#endif