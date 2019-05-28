#pragma once
#ifndef UTIL_H
#define UTIL_H

#include <sstream>
#include "BlockCoords.h"
#include "Constants.h"

std::string toString(int i);

int blockCoordsToIndex(int bx, int by, int bz);

BlockCoords indexToBlockCoords(int index);

#endif