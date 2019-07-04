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

	/*
	Must specify block id (block type), as well as global coordinates
	*/
	Block(int id, int x, int y, int z);

	/*
	Initialize block texture data
	*/
	static void initialize(Shader* shader);

	/*
	Determines whether this block should be rendered based on neighboring blocks
	*/
	void determineRendering();

	/*
	Determines the light values of each of this block's faces and recursively calculates light spread to neighboring blocks
	*/
	void determineLighting();

	/*
  Calls determineLighting on neighboring blocks, used when block is on the border of a chunk
	*/
	void determineNeighborLighting();

	/*
	Returns the opengl array texture containing each block texture
	*/
	static unsigned int getBlockArrayTexture();

	/*
	Returns the index of this block in its parent chunk's block translation array
	*/
	int getTranslationIndex();

	/*
	Set the index of this block in its parent chunk's block translation array
	*/
	void setTranslationIndex(int index);

	/*
	Returns an array of pointers to the 6 blocks neighboring this one
	*/
	Block** getNeighbors();

	/*
	Get global block x,y,z coordinates
	*/
	int getX();
	int getY();
	int getZ();

	/*
	Get the integer-coded type of this block
	*/
	int getId();

	/*
	Returns whether this block should be queued for rendering
	*/
	bool shouldRender();

	/*
	Sets the light value of this block on one of its faces
	*/
	void setLightValue(int lightValue, int face);

	/*
	Returns the light value of this block for one of its faces
	*/
	int getLightValue(int face);

	/*
	Returns the transparency of this block for calculating light spread (0.0f -> invisible, 1.0f -> opaque)
	*/
	float getTransparency();

	/*
	Set the parent chunk of this block
	*/
	Block* setParent(Chunk* parent);

	/*
	Returns the parent chunk of this block
	*/
	Chunk* getParent();

	/*
	Block type-specific function which determines whether a block of a given type should be rendered
	*/
	virtual bool shouldRenderType() = 0;

	~Block();

private:

	/*
	Opengl array texture storing all block textures
	*/
	static unsigned int blockArrayTexture;

	/*
	Index of this block in parent chunk's translation array
	*/
	int translationIndex;
	
	/*
	Type of this block
	*/
	int type;

	/*
	Array containing a light value for each face of this block
	*/
	int* lightValue;

	/*
	Transparency value for determining light spread to neighboring blocks (0.0f -> invisible, 1.0f -> opaque)
	*/
	float transparency = 1.0f;

	/*
	Controls whether this block should be queued for rendering
	*/
	bool render;

protected:

	/*
	Array of pointers to neighboring blocks surrounding this block
	*/
	Block** neighbors;

	/*
	Pointer to the parent chunk of this block
	*/
	Chunk* parent;

	/*
	Set the lighting transparency of this block
	*/
	void setTransparency(float transparency);

	/*
	Keeps track of global x,y,z coordinates of this block
	*/
	int x, y, z;

};

