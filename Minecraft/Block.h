#pragma once

#include "Constants.h"
#include "Math.h"
#include "Shader.h"
#include "Texture.h"

#include "stb_image.h"
#include <stdio.h>

class Block
{
public:
	Block(int x, int y, int z);
	static void initialize(Shader* shader);
	static void deInitialize();
	Block* clone();
	void updateNeighbors();
	static unsigned int texture;
	int translationIndex;
	int type;
	bool shouldRender;
	int x, y, z;
	Block** neighbors;
	~Block();
};

