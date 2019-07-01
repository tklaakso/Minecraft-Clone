#include "ChunkVAO.h"
#include "Constants.h"

ChunkVAO::ChunkVAO()
{

	vertexVBO = (unsigned int*)malloc(sizeof(unsigned int) * 6);
	EBO = (unsigned int*)malloc(sizeof(unsigned int) * 6);
	VAO = (unsigned int*)malloc(sizeof(unsigned int) * 6);
	uvVBO = (unsigned int*)malloc(sizeof(unsigned int) * 6);
	textureVBO = (unsigned int*)malloc(sizeof(unsigned int) * 6);
	instanceVBO = (unsigned int*)malloc(sizeof(unsigned int) * 6);
	lightmapVBO = (unsigned int*)malloc(sizeof(unsigned int) * 6);

	for (int i = 0; i < 6; i++) {

		glGenBuffers(1, &vertexVBO[i]);
		glGenBuffers(1, &EBO[i]);
		glGenBuffers(1, &uvVBO[i]);
		glGenBuffers(1, &textureVBO[i]);
		glGenBuffers(1, &instanceVBO[i]);
		glGenBuffers(1, &lightmapVBO[i]);

		glGenVertexArrays(1, &VAO[i]);
		glBindVertexArray(VAO[i]);

		const float* vertices = NULL;
		const int* indices = NULL;
		const float* uvs = NULL;

		switch (i) {
		case BLOCK_NEIGHBOR_LEFT:
			vertices = BLOCK_VERTICES_LEFT;
			indices = BLOCK_INDICES_LEFT;
			uvs = BLOCK_UVS_LEFT;
			break;
		case BLOCK_NEIGHBOR_RIGHT:
			vertices = BLOCK_VERTICES_RIGHT;
			indices = BLOCK_INDICES_RIGHT;
			uvs = BLOCK_UVS_RIGHT;
			break;
		case BLOCK_NEIGHBOR_FRONT:
			vertices = BLOCK_VERTICES_FRONT;
			indices = BLOCK_INDICES_FRONT;
			uvs = BLOCK_UVS_FRONT;
			break;
		case BLOCK_NEIGHBOR_BACK:
			vertices = BLOCK_VERTICES_BACK;
			indices = BLOCK_INDICES_BACK;
			uvs = BLOCK_UVS_BACK;
			break;
		case BLOCK_NEIGHBOR_UP:
			vertices = BLOCK_VERTICES_TOP;
			indices = BLOCK_INDICES_TOP;
			uvs = BLOCK_UVS_TOP;
			break;
		case BLOCK_NEIGHBOR_DOWN:
			vertices = BLOCK_VERTICES_BOTTOM;
			indices = BLOCK_INDICES_BOTTOM;
			uvs = BLOCK_UVS_BOTTOM;
			break;
		}

		int verticesSize = sizeof(vertices[0]) * NUM_BLOCK_VERTICES;
		int indicesSize = sizeof(indices[0]) * NUM_BLOCK_INDICES;
		int uvsSize = sizeof(uvs[0]) * NUM_BLOCK_UVS;

		glBindBuffer(GL_ARRAY_BUFFER, vertexVBO[i]);
		glBufferData(GL_ARRAY_BUFFER, verticesSize, vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[i]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, indices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, uvVBO[i]);
		glBufferData(GL_ARRAY_BUFFER, uvsSize, uvs, GL_STATIC_DRAW);

		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);

		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, instanceVBO[i]);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glVertexAttribDivisor(2, 1);

		glEnableVertexAttribArray(3);
		glBindBuffer(GL_ARRAY_BUFFER, textureVBO[i]);
		glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(int), (void*)0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glVertexAttribDivisor(3, 1);

		glEnableVertexAttribArray(4);
		glBindBuffer(GL_ARRAY_BUFFER, lightmapVBO[i]);
		glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(int), (void*)0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glVertexAttribDivisor(4, 1);

	}

}

void ChunkVAO::updateTranslationData(glm::vec3* translations, int len) {

	for (int i = 0; i < 6; i++) {

		bind(i);
		glBindBuffer(GL_ARRAY_BUFFER, instanceVBO[i]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * len, &translations[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

	}

}

void ChunkVAO::updateTextureData(int* textures, int len) {
	
	for (int i = 0; i < 6; i++) {

		bind(i);
		glBindBuffer(GL_ARRAY_BUFFER, textureVBO[i]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(int) * len, &textures[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

	}

}

void ChunkVAO::updateLightmapData(int** lightMap, int len) {

	for (int i = 0; i < 6; i++) {

		bind(i);
		glBindBuffer(GL_ARRAY_BUFFER, lightmapVBO[i]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(int) * len, &lightMap[i][0], GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

	}

}

void ChunkVAO::bind(int face) {

	assert(face >= 0 && face < 6);

	glBindVertexArray(VAO[face]);

}

ChunkVAO::~ChunkVAO()
{

	for (int i = 0; i < 6; i++) {

		glDeleteBuffers(1, &vertexVBO[i]);
		glDeleteBuffers(1, &EBO[i]);
		glDeleteBuffers(1, &uvVBO[i]);
		glDeleteBuffers(1, &textureVBO[i]);
		glDeleteBuffers(1, &instanceVBO[i]);
		glDeleteBuffers(1, &lightmapVBO[i]);
		glDeleteVertexArrays(1, &VAO[i]);

	}

	free(vertexVBO);
	free(EBO);
	free(uvVBO);
	free(textureVBO);
	free(instanceVBO);
	free(lightmapVBO);
	free(VAO);

}
