#include "Block.h"
#include "Util.h"

unsigned int Block::texture = 0;

void Block::initialize(Shader* shader) {

	unsigned char* textureData = (unsigned char*)malloc(sizeof(unsigned char) * BLOCK_TEXTURE_WIDTH * BLOCK_TEXTURE_HEIGHT * NUM_TEXTURES * 3);

	stbi_set_flip_vertically_on_load(true);

	for (int i = 0; i < NUM_TEXTURES; i++) {

		int width, height, nChannels;

		string strIndex = toString(i);

		string path = std::string("Resources/") + strIndex + std::string(".png");

		unsigned char* data = stbi_load(path.c_str(), &width, &height, &nChannels, 0);
		for (int x = i * BLOCK_TEXTURE_WIDTH * BLOCK_TEXTURE_HEIGHT * 3; x < (i + 1) * BLOCK_TEXTURE_WIDTH * BLOCK_TEXTURE_HEIGHT * 3; x++) {
			textureData[x] = data[x - i * BLOCK_TEXTURE_WIDTH * BLOCK_TEXTURE_HEIGHT * 3];
		}

		stbi_image_free(data);

	}

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D_ARRAY, texture);

	glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGB, BLOCK_TEXTURE_WIDTH, BLOCK_TEXTURE_HEIGHT, NUM_TEXTURES);
	glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGB, BLOCK_TEXTURE_WIDTH, BLOCK_TEXTURE_HEIGHT, NUM_TEXTURES, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);

	glGenerateMipmap(GL_TEXTURE_2D_ARRAY);

	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	free(textureData);

}

void Block::deInitialize() {

}

Block::Block(int id, int x, int y, int z)
{
	this->type = id;
	this->x = x;
	this->y = y;
	this->z = z;
	render = true;
	neighbors = (Block**)malloc(sizeof(Block*) * 6);
	for (int i = 0; i < 6; i++) {
		neighbors[i] = NULL;
	}
}

void Block::updateNeighbors() {
	if (!shouldRenderType()) {
		render = false;
		return;
	}
	for (int i = 0; i < 6; i++) {
		if (neighbors[i] == NULL || !neighbors[i]->shouldRenderType()) {
			render = true;
			return;
		}
	}
	render = false;
}

unsigned int Block::getTexture() {
	return texture;
}

void Block::setTexture(unsigned int texture) {
	Block::texture = texture;
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

void Block::setLightValue(int lightValue) {
	this->lightValue = lightValue;
}

int Block::getLightValue() {
	return lightValue;
}

void Block::calculateLighting() {
	if (lightValue > 0) {
		for (int i = 0; i < 6; i++) {
			if (neighbors[i] != NULL && neighbors[i]->getLightValue() < lightValue - LIGHT_DEPRECIATION) {
				neighbors[i]->setLightValue(lightValue - LIGHT_DEPRECIATION);
				neighbors[i]->calculateLighting();
			}
		}
	}
}

Block::~Block()
{
	free(neighbors);
}
