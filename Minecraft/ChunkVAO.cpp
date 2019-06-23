#include "ChunkVAO.h"
#include "Constants.h"

ChunkVAO::ChunkVAO()
{

	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glGenBuffers(1, &uvVBO);
	glGenBuffers(1, &textureVBO);
	glGenBuffers(1, &instanceVBO);
	glGenBuffers(1, &lightmapVBO);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(BLOCK_VERTICES), BLOCK_VERTICES, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(BLOCK_INDICES), BLOCK_INDICES, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, uvVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(BLOCK_UVS), BLOCK_UVS, GL_STATIC_DRAW);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);

	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glVertexAttribDivisor(2, 1);
	
	glEnableVertexAttribArray(3);
	glBindBuffer(GL_ARRAY_BUFFER, textureVBO);
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(int), (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glVertexAttribDivisor(3, 1);

	glEnableVertexAttribArray(4);
	glBindBuffer(GL_ARRAY_BUFFER, lightmapVBO);
	glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(int), (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glVertexAttribDivisor(4, 1);

}

void ChunkVAO::updateTranslationData(glm::vec3* translations, int len) {

	bind();
	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * len, &translations[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void ChunkVAO::updateTextureData(int* textures, int len) {
	
	bind();
	glBindBuffer(GL_ARRAY_BUFFER, textureVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(int) * len, &textures[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void ChunkVAO::updateLightmapData(int* lightMap, int len) {

	bind();
	glBindBuffer(GL_ARRAY_BUFFER, lightmapVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(int) * len, &lightMap[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void ChunkVAO::bind() {

	glBindVertexArray(VAO);

}

ChunkVAO::~ChunkVAO()
{

	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteBuffers(1, &uvVBO);
	glDeleteBuffers(1, &textureVBO);
	glDeleteBuffers(1, &instanceVBO);
	glDeleteVertexArrays(1, &VAO);

}
