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
	Block(int id, int x, int y, int z);
	static void initialize(Shader* shader);
	static void deInitialize();
	void updateNeighbors();
	static unsigned int getTexture();
	static void setTexture(unsigned int texture);
	int getTranslationIndex();
	void setTranslationIndex(int index);
	bool shouldRender();
	Block** getNeighbors();
	int getX();
	int getY();
	int getZ();
	int getId();
	void setLightValue(int lightValue);
	int getLightValue();
	void calculateLighting();
	virtual Block* clone() = 0;
	virtual bool shouldRenderType() = 0;
	~Block();
private:
	static unsigned int texture;
	int translationIndex;
	int type;
	int lightValue = 0;
	bool render;
protected:
	Block** neighbors;
	int x, y, z;
};

