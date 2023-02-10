#pragma once
#include <RealEngine.h>

#include <FastNoise/FastNoise.h>

#include "Voxel.h"
#include "Constants.h"

class TerrainGenerator {
public:
	//static inline void ReSeed(uint32_t seed) { m_PerlinNoise.reseed(seed); }

	//static inline RealEngine::BasicPerlinNoise<float> GetNoiseObject() { return m_PerlinNoise; }

	static inline void CreateTerrain(Voxel m_Voxels[Constants::CHUNK_SIZE][Constants::CHUNK_SIZE][Constants::CHUNK_SIZE], glm::ivec3 worldPos) {
		FastNoise::SmartNode<> m_PerlinNoise = FastNoise::NewFromEncodedNodeTree("FwAAAIC/AACAPwAAAAAAAIA/BwA=");
		//FastNoise::SmartNode<FastNoise::Perlin> m_PerlinNoise = FastNoise::New<FastNoise::Perlin>();

		float noiseOutput[Constants::CHUNK_SIZE * Constants::CHUNK_SIZE];

		m_PerlinNoise->GenUniformGrid2D(noiseOutput, worldPos.x, worldPos.z, Constants::CHUNK_SIZE, Constants::CHUNK_SIZE, Constants::FREQUENCY, 0);
		
		for (uint32_t x = 0; x < Constants::CHUNK_SIZE; x++) {
			for (uint32_t z = 0; z < Constants::CHUNK_SIZE; z++) {
				uint32_t height = (uint32_t)(noiseOutput[x + Constants::CHUNK_SIZE * z] * Constants::WORLD_HEIGHT);
				for (uint32_t y = 0; y < Constants::CHUNK_SIZE; y++) {
					uint32_t voxelWorldHeight = y + worldPos.y;
					m_Voxels[x][y][z].SetBlockType(voxelWorldHeight < height ? VoxelType::BlockType_Grass : VoxelType::BlockType_Air);
					//m_Voxels[x][y][z].SetBlockType(x % 2 == 0 && y % 2 == 0 && z % 2 == 0 ? VoxelType::BlockType_Sand : VoxelType::BlockType_Grass);
					//m_Voxels[x][y][z].SetBlockType(x % 2 == 0 && z % 2 == 0 ? VoxelType::BlockType_Grass : VoxelType::BlockType_Sand);
					//m_Voxels[x][y][z].SetBlockType(x % 2 == 0 && z % 2 == 0 ? VoxelType::BlockType_Grass : VoxelType::BlockType_Air);
				}
			}
		}
	}
};
