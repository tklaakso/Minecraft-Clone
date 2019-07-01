#pragma once

#include "Constants.h"
#include "Math.h"
#include "Shader.h"
#include "Texture.h"

#include "stb_image.h"
#include <stdio.h>

class Chunk;

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
	void setLightValue(int lightValue, int face);
	int getLightValue(int face);
	float getTransparency();
	void calculateLighting();
	void calculateNeighborLighting();
	Block* setParent(Chunk* parent);
	Chunk* getParent();
	virtual bool shouldRenderType() = 0;
	~Block();
private:
	static unsigned int texture;
	int translationIndex;
	int type;
	int* lightValue;
	float transparency = 1.0f;
	bool render;
protected:
	Block** neighbors;
	Chunk* parent;
	void setTransparency(float transparency);
	int x, y, z;
};

