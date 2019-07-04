#include "Block.h"
#include "Util.h"
#include "Chunk.h"

unsigned int Block::blockArrayTexture = 0;

void Block::initialize(Shader* shader) {

	// Allocate array that will contain data of each block texture
	unsigned char* textureData = (unsigned char*)malloc(sizeof(unsigned char) * BLOCK_TEXTURE_WIDTH * BLOCK_TEXTURE_HEIGHT * NUM_TEXTURES * 3);

	stbi_set_flip_vertically_on_load(true);

	for (int i = 0; i < NUM_TEXTURES; i++) {

		int width, height, nChannels;

		string strIndex = toString(i);

		string path = std::string("Resources/") + strIndex + std::string(".png");

		// Read data of one individual texture
		unsigned char* data = stbi_load(path.c_str(), &width, &height, &nChannels, 0);

		// Record this data in textureData
		for (int x = i * BLOCK_TEXTURE_WIDTH * BLOCK_TEXTURE_HEIGHT * 3; x < (i + 1) * BLOCK_TEXTURE_WIDTH * BLOCK_TEXTURE_HEIGHT * 3; x++) {

			textureData[x] = data[x - i * BLOCK_TEXTURE_WIDTH * BLOCK_TEXTURE_HEIGHT * 3];

		}

		stbi_image_free(data);

	}

	// Initialize opengl object for block array texture
	glGenTextures(1, &blockArrayTexture);

	glBindTexture(GL_TEXTURE_2D_ARRAY, blockArrayTexture);

	glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGB, BLOCK_TEXTURE_WIDTH, BLOCK_TEXTURE_HEIGHT, NUM_TEXTURES);

	glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGB, BLOCK_TEXTURE_WIDTH, BLOCK_TEXTURE_HEIGHT, NUM_TEXTURES, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);

	// We will be using mimpaps for performance and so blocks look better at a distance
	glGenerateMipmap(GL_TEXTURE_2D_ARRAY);

	// Set TexParameter information such that blocks look appropriate at close and long distances
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	free(textureData);

}

Block::Block(int id, int x, int y, int z)
{

	this->type = id;

	this->x = x;

	this->y = y;

	this->z = z;

	// Initialize some block variables
	render = true;

	neighbors = (Block**)malloc(sizeof(Block*) * 6);

	for (int i = 0; i < 6; i++) {

		neighbors[i] = NULL;

	}

	lightValue = (int*)malloc(sizeof(int) * 6);

	for (int i = 0; i < 6; i++) {

		lightValue[i] = 0;

	}

}

void Block::determineRendering() {

	// If we're not rendering this block type, don't even bother checking neighbor states
	if (!shouldRenderType()) {

		render = false;

		return;

	}

	// Iterate through all 6 neighboring blocks
	for (int i = 0; i < 6; i++) {

		// If at least one neighbor is not being rendered, this block is exposed so we'll render it
		if (neighbors[i] != NULL && !neighbors[i]->shouldRenderType()) {

			render = true;

			return;

		}

	}

	// Otherwise this block is completely surrounded so we won't render it
	render = false;

}

unsigned int Block::getBlockArrayTexture() {

	return blockArrayTexture;

}

int Block::getTranslationIndex() {

	return translationIndex;

}

void Block::setTranslationIndex(int index) {

	this->translationIndex = index;

}

bool Block::shouldRender() {

	return render;

}

Block** Block::getNeighbors() {

	return neighbors;

}

Block* Block::setParent(Chunk* parent) {

	this->parent = parent;

	// Return this block out of convenience since this method is often used when initializing newly created blocks
	return this;

}

Chunk* Block::getParent() {

	return parent;

}

int Block::getX() {

	return x;

}

int Block::getY() {

	return y;

}

int Block::getZ() {

	return z;

}

int Block::getId() {

	return type;

}

void Block::setLightValue(int lightValue, int face) {

	this->lightValue[face] = lightValue;

}

int Block::getLightValue(int face) {

	return lightValue[face];

}

void Block::determineLighting() {

	// If parent for this block has been set, tell it to pass its lighting values to the VAO before the next render call
	if (parent != NULL) {

		parent->updateLightingOnRender();

	}

	// Determine max lighting value out of all block faces
	int maxLightValue = 0;

	for (int i = 0; i < 6; i++) {

		maxLightValue = max(maxLightValue, lightValue[i]);

	}

	if (maxLightValue > 0) {

		// If block is completely transparent, allow the maximum lighting value to pass through to other block faces
		if (getTransparency() == 0.0f) {

			for (int i = 0; i < 6; i++) {

				lightValue[i] = maxLightValue;

			}

		}
		
		for (int i = 0; i < 6; i++) {

			// Only pass lighting to neighbor if, accounting for light depreciation, this block would still have an impact on its lighting value
			if (neighbors[i] != NULL && neighbors[i]->getLightValue(getOppositeNeighbor(i)) < (1.0f - getTransparency()) * (lightValue[i] - LIGHT_DEPRECIATION)) {

				// Update neighbor lighting value, accounting for transparency level of this block
				neighbors[i]->setLightValue((1.0f - getTransparency()) * (lightValue[i] - LIGHT_DEPRECIATION), getOppositeNeighbor(i));

				// Recursively calculate lighting values
				neighbors[i]->determineLighting();

			}

		}

	}

}

void Block::determineNeighborLighting() {

	for (int i = 0; i < 6; i++) {

		if (neighbors[i] != NULL) {

			neighbors[i]->determineLighting();

		}

	}

}

void Block::setTransparency(float transparency) {

	this->transparency = transparency;

}

float Block::getTransparency() {

	return transparency;

}

Block::~Block()
{

	free(neighbors);

	free(lightValue);

}
