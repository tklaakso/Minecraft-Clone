#pragma once
#ifndef UTIL_H
#define UTIL_H

#include <sstream>
#include "BlockCoords.h"
#include "Constants.h"
#include "Chunk.h"

std::string toString(int i);

std::vector<std::string> split(std::string s, std::string delim);

int blockCoordsToIndex(int bx, int by, int bz);

BlockCoords indexToBlockCoords(int index);

Chunk* findChunkWithCoords(std::vector<Chunk*>* chunks, ChunkCoords* coords, int l, int r);

#endif