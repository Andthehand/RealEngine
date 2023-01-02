#include "ChunkManager.h"

ChunkManager::ChunkManager() {
	m_Chunks = new Chunk**[RENDER_DISTANCE];
	for (int x = 0; x < RENDER_DISTANCE; x++) {
		m_Chunks[x] = new Chunk*[RENDER_DISTANCE];
		for (int y = 0; y < RENDER_DISTANCE; y++) {
			m_Chunks[x][y] = new Chunk[RENDER_DISTANCE];
			for (int z = 0; z < RENDER_DISTANCE; z++) {
				m_Chunks[x][y][z].Init({ x * Chunk::CHUNK_SIZE, y * Chunk::CHUNK_SIZE, z * Chunk::CHUNK_SIZE });
			}
		}
	}
}

ChunkManager::~ChunkManager() {
	for (int i = 0; i < RENDER_DISTANCE; ++i) {
		for (int j = 0; j < RENDER_DISTANCE; ++j) {
			delete[] m_Chunks[i][j];
		}
		delete[] m_Chunks[i];
	}
	delete[] m_Chunks;
}

void ChunkManager::Render() {
	for (int x = 0; x < RENDER_DISTANCE; x++) {
		for (int y = 0; y < RENDER_DISTANCE; y++) {
			for (int z = 0; z < RENDER_DISTANCE; z++) {
				m_Chunks[x][y][z].Render();
			}
		}
	}
}
