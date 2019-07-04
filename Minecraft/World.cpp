#include "World.h"
#include "Constants.h"
#include "Player.h"
#include <limits>

RegionManager* World::rm = new RegionManager();

World::World(GLFWwindow* window)
{
	cm = new ChunkManager();
	pool = new ChunkThreadPool(cm, 1);
	renderDistance = 8;
	updateLoadedRegions();
	updateLoadedChunks();
	player = new Player(window, this);
}

Block* World::getBlock(int x, int y, int z, Chunk* suspect) {
	if (suspect != NULL && suspect->blockInChunk(x, y, z)) {
		return suspect->getBlock(x, y, z);
	}
	ChunkCoords coords = blockToChunkCoords(x, z);
	pool->chunkManagerMutex.lock();
	Chunk* chunk = findChunkWithCoords(cm->chunks, &coords, 0, cm->chunks->size() - 1);
	pool->chunkManagerMutex.unlock();
	if (chunk == NULL) {
		return NULL;
	}
	return chunk->getBlock(x, y, z);
}

void World::updateRendering(Block* block, Chunk* suspect) {
	Chunk* c = NULL;
	int x = block->getX(), y = block->getY(), z = block->getZ();
	if (suspect != NULL && suspect->blockInChunk(x, y, z)) {
		c = suspect;
	}
	else {
		ChunkCoords coords = blockToChunkCoords(x, z);
		pool->chunkManagerMutex.lock();
		c = findChunkWithCoords(cm->chunks, &coords, 0, cm->chunks->size() - 1);
		pool->chunkManagerMutex.unlock();
	}
	if (c != NULL) {
		c->textures[block->getTranslationIndex()] = block->shouldRender() ? block->getId() : -1;
	}
}

void World::setBlock(int x, int y, int z, Block* block, bool update, bool reorderNeighbors) {
	ChunkCoords coords = blockToChunkCoords(x, z);
	pool->chunkManagerMutex.lock();
	Chunk* chunk = findChunkWithCoords(cm->chunks, &coords, 0, cm->chunks->size() - 1);
	pool->chunkManagerMutex.unlock();
	if (chunk != NULL) {
		chunk->setBlock(x, y, z, block, update);
		Block* left = getBlock(x - 1, y, z, chunk);
		Block* right = getBlock(x + 1, y, z, chunk);
		Block* up = getBlock(x, y + 1, z, chunk);
		Block* down = getBlock(x, y - 1, z, chunk);
		Block* front = getBlock(x, y, z - 1, chunk);
		Block* back = getBlock(x, y, z + 1, chunk);
		if (left != NULL) {
			if (block != NULL) {
				block->getNeighbors()[BLOCK_NEIGHBOR_LEFT] = left;
			}
			left->getNeighbors()[BLOCK_NEIGHBOR_RIGHT] = block;
		}
		if (right != NULL) {
			if (block != NULL) {
				block->getNeighbors()[BLOCK_NEIGHBOR_RIGHT] = right;
			}
			right->getNeighbors()[BLOCK_NEIGHBOR_LEFT] = block;
		}
		if (up != NULL) {
			if (block != NULL) {
				block->getNeighbors()[BLOCK_NEIGHBOR_UP] = up;
			}
			up->getNeighbors()[BLOCK_NEIGHBOR_DOWN] = block;
		}
		if (down != NULL) {
			if (block != NULL) {
				block->getNeighbors()[BLOCK_NEIGHBOR_DOWN] = down;
			}
			down->getNeighbors()[BLOCK_NEIGHBOR_UP] = block;
		}
		if (front != NULL) {
			if (block != NULL) {
				block->getNeighbors()[BLOCK_NEIGHBOR_FRONT] = front;
			}
			front->getNeighbors()[BLOCK_NEIGHBOR_BACK] = block;
		}
		if (back != NULL) {
			if (block != NULL) {
				block->getNeighbors()[BLOCK_NEIGHBOR_BACK] = back;
			}
			back->getNeighbors()[BLOCK_NEIGHBOR_FRONT] = block;
		}
		if (left != NULL) {
			left->determineRendering();
			if (reorderNeighbors && left->shouldRenderType()) {
				reorderBlock(left, chunk);
			}
		}
		if (right != NULL) {
			right->determineRendering();
			if (reorderNeighbors && right->shouldRenderType()) {
				reorderBlock(right, chunk);
			}
		}
		if (up != NULL) {
			up->determineRendering();
			if (reorderNeighbors && up->shouldRenderType()) {
				reorderBlock(up, chunk);
			}
		}
		if (down != NULL) {
			down->determineRendering();
			if (reorderNeighbors && down->shouldRenderType()) {
				reorderBlock(down, chunk);
			}
		}
		if (front != NULL) {
			front->determineRendering();
			if (reorderNeighbors && front->shouldRenderType()) {
				reorderBlock(front, chunk);
			}
		}
		if (back != NULL) {
			back->determineRendering();
			if (reorderNeighbors && back->shouldRenderType()) {
				reorderBlock(back, chunk);
			}
		}
		chunk->calculateLighting();
	}
	else {
		std::cout << "Error: tried to add block in nonexistent chunk" << std::endl;
	}
}

void World::exit() {
	pool->exit();
}

void World::reorderBlocks() {
	pool->chunkManagerMutex.lock();
	for (int i = 0; i < cm->chunks->size(); i++) {
		(*(cm->chunks))[i]->reorderBlocks();
	}
	pool->chunkManagerMutex.unlock();
}

void World::reorderBlock(Block* block, Chunk* suspect) {
	int x = block->getX(), y = block->getY(), z = block->getZ();
	if (suspect->blockInChunk(x, y, z)) {
		suspect->reorderBlock(block);
		suspect->updateVAO();
	}
	else {
		ChunkCoords coords = blockToChunkCoords(x, z);
		pool->chunkManagerMutex.lock();
		Chunk* c = findChunkWithCoords(cm->chunks, &coords, 0, cm->chunks->size() - 1);
		pool->chunkManagerMutex.unlock();
		c->reorderBlock(block);
		c->updateVAO();
	}
}

void World::updateChunkVAOs() {
	pool->chunkManagerMutex.lock();
	for (int i = 0; i < cm->chunks->size(); i++) {
		(*(cm->chunks))[i]->updateVAO();
	}
	pool->chunkManagerMutex.unlock();
}

bool World::chunkExists(int x, int y) {
	ChunkCoords coords(x, y, 0, 0);
	pool->chunkManagerMutex.lock();
	Chunk* c = findChunkWithCoords(cm->chunks, &coords, 0, cm->chunks->size() - 1);
	pool->chunkManagerMutex.unlock();
	return c != NULL;
}

bool World::regionExists(int x, int y) {
	return rm->getRegion(x, y) != NULL;
}

void World::updateLoadedChunks() {
	glm::vec2 playerChunkPos((float)playerChunkX, (float)playerChunkY);
	pool->chunkManagerMutex.lock();
	for (int i = cm->chunks->size() - 1; i >= 0; i--) {
		if (glm::length(glm::vec2((float)(*(cm->chunks))[i]->x, (float)(*(cm->chunks))[i]->y) - playerChunkPos) > renderDistance) {
			deleteChunk((*(cm->chunks))[i]->x, (*(cm->chunks))[i]->y);
		}
	}
	pool->chunkManagerMutex.unlock();
	for (int x = (int)playerChunkPos.x - renderDistance; x < (int)playerChunkPos.x + renderDistance; x++) {
		for (int y = (int)playerChunkPos.y - renderDistance; y < (int)playerChunkPos.y + renderDistance; y++) {
			int localX = x - (int)playerChunkPos.x;
			int localY = y - (int)playerChunkPos.y;
			if (glm::sqrt(localX * localX + localY * localY) <= renderDistance && !chunkExists(x, y)) {
				makeChunk(x, y);
			}
		}
	}
}

void World::updateLoadedRegions() {
	glm::vec2 playerRegionPos(floor((float)playerChunkX / REGION_WIDTH_CHUNKS), floor((float)playerChunkY / REGION_WIDTH_CHUNKS));
	std::vector<Region*> regions = rm->getRegions();
	for (int i = regions.size() - 1; i >= 0; i--) {
		if (glm::abs(regions[i]->getX() - playerRegionPos.x) > REGION_GENERATION_DISTANCE || glm::abs(regions[i]->getZ() - playerRegionPos.y) > REGION_GENERATION_DISTANCE) {
			deleteRegion(regions[i]->getX(), regions[i]->getZ());
		}
	}
	for (int x = (int)playerRegionPos.x - REGION_GENERATION_DISTANCE; x <= (int)playerRegionPos.x + REGION_GENERATION_DISTANCE; x++) {
		for (int y = (int)playerRegionPos.y - REGION_GENERATION_DISTANCE; y <= (int)playerRegionPos.y + REGION_GENERATION_DISTANCE; y++) {
			if (!regionExists(x, y)) {
				makeRegion(x, y);
			}
		}
	}
}

void World::updatePlayerChunkPosition(int chunkX, int chunkY) {
	bool updateChunk = false;
	bool updateRegion = false;
	int playerRegionX = (int)floor((float)playerChunkX / REGION_WIDTH_CHUNKS);
	int playerRegionY = (int)floor((float)playerChunkY / REGION_WIDTH_CHUNKS);
	int regionX = (int)floor((float)chunkX / REGION_WIDTH_CHUNKS);
	int regionY = (int)floor((float)chunkY / REGION_WIDTH_CHUNKS);
	if (chunkX != playerChunkX || chunkY != playerChunkY) {
		updateChunk = true;
	}
	if (regionX != playerRegionX || regionY != playerRegionY) {
		updateRegion = true;
	}
	playerChunkX = chunkX;
	playerChunkY = chunkY;
	if (updateChunk) {
		updateLoadedChunks();
	}
	if (updateRegion) {
		updateLoadedRegions();
	}
}

void World::render() {
	glBindTexture(GL_TEXTURE_2D_ARRAY, Block::getBlockArrayTexture());
	pool->chunkManagerMutex.lock();
	for (int i = 0; i < cm->chunks->size(); i++) {
		if ((*(cm->chunks))[i]->state == Chunk::RENDERING) {
			(*(cm->chunks))[i]->render();
		}
	}
	pool->chunkManagerMutex.unlock();
	Camera* cam = player->getCamera();
	BlockCastInfo info = blockCast(cam->getPosition(), cam->getDirection());
	selectedBlock = info.block;
	if (selectedBlock != NULL) {
		if (Input::mouseClicked(GLFW_MOUSE_BUTTON_LEFT)) {
			setBlock(selectedBlock->getX(), selectedBlock->getY(), selectedBlock->getZ(), makeBlock(BLOCK_AIR, selectedBlock->getX(), selectedBlock->getY(), selectedBlock->getZ(), selectedBlock->getParent()), true, true);
		}
		else if (Input::mouseClicked(GLFW_MOUSE_BUTTON_RIGHT)) {
			glm::vec3 relativeVector = neighborPositionToVector(info.face);
			setBlock(selectedBlock->getX() + (int)relativeVector.x, selectedBlock->getY() + (int)relativeVector.y, selectedBlock->getZ() + (int)relativeVector.z, makeBlock(BLOCK_GRASS, selectedBlock->getX() + (int)relativeVector.x, selectedBlock->getY() + (int)relativeVector.y, selectedBlock->getZ() + (int)relativeVector.z, selectedBlock->getParent()), true, true);
		}
	}
}

void World::makeChunk(int x, int y) {
	if (!chunkExists(x, y)) {
		Chunk* c = new Chunk(x, y, pool);
		pool->createChunk(c);
	}
}

void World::makeRegion(int x, int y) {
	if (!regionExists(x, y)) {
		Region* r = new Region(x, y);
		r->generate();
		rm->addRegion(r);
	}
}

void World::updateViewFrustum(ViewFrustum* frustum) {
	pool->chunkManagerMutex.lock();
	for (int i = 0; i < cm->chunks->size(); i++) {
		(*(cm->chunks))[i]->updateViewFrustum(frustum);
	}
	pool->chunkManagerMutex.unlock();
}

void World::deleteChunk(int x, int y) {
	ChunkCoords coords(x, y, 0, 0);
	Chunk* c = findChunkWithCoords(cm->chunks, &coords, 0, cm->chunks->size() - 1);
	if (c != NULL) {
		pool->deleteChunk(c);
	}
}

void World::deleteRegion(int x, int y) {
	Region* r = rm->removeRegion(x, y);
	delete r;
}

BlockCastInfo World::blockCast(glm::vec3 origin, glm::vec3 dir) {
	dir = glm::normalize(dir);
	glm::vec3 pos = origin;
	int signX = signum(dir.x);
	int signY = signum(dir.y);
	int signZ = signum(dir.z);
	int face = 0;
	while (glm::length(pos - origin) < 20) {
		int blockX = fullFloor(pos.x) + (signX == 1 && pos.x == (int)pos.x ? 1 : 0);
		int blockY = fullFloor(pos.y) + (signY == 1 && pos.y == (int)pos.y ? 1 : 0);
		int blockZ = fullFloor(pos.z) + (signZ == 1 && pos.z == (int)pos.z ? 1 : 0);
		Block* b = getBlock(blockX, blockY, blockZ, NULL);
		if (b != NULL && b->shouldRenderType()) {
			return BlockCastInfo(b, face);
		}
		glm::vec3 targetX(0, 0, 0), targetY(0, 0, 0), targetZ(0, 0, 0);
		float distanceX = std::numeric_limits<float>::max();
		float distanceY = std::numeric_limits<float>::max();
		float distanceZ = std::numeric_limits<float>::max();
		if (signX != 0) {
			float targetXVal = fullRound(pos.x, signX);
			targetX = pos + dir * ((targetXVal - pos.x) / dir.x);
			targetX.x = round(targetX.x);
			distanceX = glm::length(targetX - pos);
		}
		if (signY != 0) {
			float targetYVal = fullRound(pos.y, signY);
			targetY = pos + dir * ((targetYVal - pos.y) / dir.y);
			targetY.y = round(targetY.y);
			distanceY = glm::length(targetY - pos);
		}
		if (signZ != 0) {
			float targetZVal = fullRound(pos.z, signZ);
			targetZ = pos + dir * ((targetZVal - pos.z) / dir.z);
			targetZ.z = round(targetZ.z);
			distanceZ = glm::length(targetZ - pos);
		}
		glm::vec3 minDistanceTarget = targetX;
		float minDistance = distanceX;
		face = signX > 0 ? BLOCK_NEIGHBOR_LEFT : BLOCK_NEIGHBOR_RIGHT;
		if (distanceY < minDistance) {
			minDistanceTarget = targetY;
			minDistance = distanceY;
			face = signY > 0 ? BLOCK_NEIGHBOR_DOWN : BLOCK_NEIGHBOR_UP;
		}
		if (distanceZ < minDistance) {
			minDistanceTarget = targetZ;
			minDistance = distanceZ;
			face = signZ > 0 ? BLOCK_NEIGHBOR_FRONT : BLOCK_NEIGHBOR_BACK;
		}
		pos = minDistanceTarget;
	}
	return BlockCastInfo(NULL, 0);
}

Player* World::getPlayer() {
	return player;
}

World::~World()
{
	delete cm;
	delete pool;
	delete player;
}
