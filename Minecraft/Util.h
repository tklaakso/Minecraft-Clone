#pragma once
#ifndef UTIL_H
#define UTIL_H

#include <sstream>
#include "BlockCoords.h"
#include "Constants.h"
#include "Chunk.h"
#include "RegionCoords.h"
#include "Block.h"

std::string toString(int i);

std::vector<std::string> split(std::string s, std::string delim);

int blockCoordsToIndex(int bx, int by, int bz);

BlockCoords indexToBlockCoords(int index);

Chunk* findChunkWithCoords(std::vector<Chunk*>* chunks, ChunkCoords* coords, int l, int r);

ChunkCoords blockToChunkCoords(int bx, int bz);

RegionCoords chunkToRegionCoords(int cx, int cz);

Block* makeBlock(int id, int x, int y, int z, Chunk* parent);

Block* clone(Block* b);

int fullFloor(float f);

int fullCeil(float f);

int fullRound(float f, int sign);

int signum(float f);

glm::vec3 neighborPositionToVector(int pos);

#endif