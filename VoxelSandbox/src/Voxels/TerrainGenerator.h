#pragma once
#include <RealEngine.h>

#include <FastNoise/FastNoise.h>

#include "Voxel.h"
#include "Constants.h"

class TerrainGenerator {
public:
	static inline void CreateTerrain(Voxel m_Voxels[Constants::CHUNK_SIZE][Constants::CHUNK_SIZE][Constants::CHUNK_SIZE], glm::ivec3 worldPos) {
		static FastNoise::SmartNode<> m_PerlinNoise = FastNoise::NewFromEncodedNodeTree("FwAAAIC/AACAPwAAAAAAAIA/BwA=");

		float* noiseOutput = new float[Constants::CHUNK_SIZE * Constants::CHUNK_SIZE];

		m_PerlinNoise->GenUniformGrid2D(noiseOutput, worldPos.x, worldPos.z, Constants::CHUNK_SIZE, Constants::CHUNK_SIZE, Constants::FREQUENCY, Constants::SEED);
		
		for (uint32_t x = 0; x < Constants::CHUNK_SIZE; x++) {
			for (uint32_t z = 0; z < Constants::CHUNK_SIZE; z++) {
				uint32_t height = (uint32_t)(noiseOutput[x + Constants::CHUNK_SIZE * z] * Constants::WORLD_HEIGHT);
				for (uint32_t y = 0; y < Constants::CHUNK_SIZE; y++) {
					uint32_t voxelWorldHeight = y + worldPos.y;
					m_Voxels[x][y][z].SetBlockType(voxelWorldHeight < height ? VoxelTypeIDs::Grass : VoxelTypeIDs::Air);
					//m_Voxels[x][y][z].SetBlockType(x % 2 == 0 && y % 2 == 0 && z % 2 == 0 ? VoxelTypeIDs::Sand : VoxelTypeIDs::Grass);
					//m_Voxels[x][y][z].SetBlockType(x % 2 == 0 && z % 2 == 0 ? VoxelTypeIDs::Grass : VoxelTypeIDs::Sand);
					//m_Voxels[x][y][z].SetBlockType(x % 2 == 0 && z % 2 == 0 ? VoxelTypeIDs::Grass : VoxelTypeIDs::Air);
				}
			}
		}
		delete[] noiseOutput;
	}
};
