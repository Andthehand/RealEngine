#pragma once
#include <RealEngine.h>

#include "Voxel.h"
#include "Constants.h"

class TerrainGenerator {
public:
	static inline void ReSeed(uint32_t seed) { m_PerlinNoise.reseed(seed); }

	static inline RealEngine::BasicPerlinNoise<float> GetNoiseObject() { return m_PerlinNoise; }

	static inline void CreateTerrain(Voxel m_Voxels[Constants::CHUNK_SIZE][Constants::CHUNK_SIZE][Constants::CHUNK_SIZE], glm::ivec3 worldPos) {
		for (uint32_t x = 0; x < Constants::CHUNK_SIZE; x++) {
			for (uint32_t z = 0; z < Constants::CHUNK_SIZE; z++) {
				float tempX = ((float)x + worldPos.x) * Constants::NOISE_STEP;
				float tempZ = ((float)z + worldPos.z) * Constants::NOISE_STEP;
				float noiseValue = m_PerlinNoise.noise2D_01(tempX, tempZ);
				int height = (uint32_t)(std::abs(noiseValue * Constants::WORLD_HEIGHT));
				for (uint32_t y = 0; y < Constants::CHUNK_SIZE; y++) {
					int voxelWorldHeight = y + worldPos.y;
					m_Voxels[x][y][z].SetBlockType(voxelWorldHeight < height ? VoxelType::BlockType_Grass : VoxelType::BlockType_Air);
				}
			}
		}
	}

private:
	inline static RealEngine::BasicPerlinNoise<float> m_PerlinNoise;
};
