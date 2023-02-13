#pragma once
#include <RealEngine.h>

#include <FastNoise/FastNoise.h>

#include "Voxel.h"
#include "Constants.h"

class TerrainGenerator {
public:
	static int TO1D(int x, int y, int z) {
		return (z * Constants::CHUNK_SIZE * Constants::CHUNK_SIZE) + (y * Constants::CHUNK_SIZE) + x;
	}

	static inline void CreateTerrain(Voxel m_Voxels[Constants::CHUNK_SIZE][Constants::CHUNK_SIZE][Constants::CHUNK_SIZE], glm::ivec3 worldPos) {
		//static FastNoise::SmartNode<> m_PerlinNoise = FastNoise::NewFromEncodedNodeTree("FwAAAIC/AACAPwAAAAAAAIA/EQACAAAAAAAgQBAAAAAAQBkAEwDD9Sg/DQAEAAAAAAAgQAkAAGZmJj8AAAAAPwEEAAAAAAAAAEBAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAM3MTD4AMzMzPwAAAAA/");
		static FastNoise::SmartNode<> m_PerlinNoise = FastNoise::NewFromEncodedNodeTree("FwAAAIC/AACAPwAAAAAAAIA/BwA=");

		float* noiseOutput3D = new float[Constants::CHUNK_SIZE * Constants::CHUNK_SIZE * Constants::CHUNK_SIZE];
		float* noiseOutput2D = new float[Constants::CHUNK_SIZE * Constants::CHUNK_SIZE];

		m_PerlinNoise->GenUniformGrid3D(noiseOutput3D, worldPos.x, worldPos.y, worldPos.z, Constants::CHUNK_SIZE, Constants::CHUNK_SIZE, Constants::CHUNK_SIZE, Constants::FREQUENCY, Constants::SEED);
		m_PerlinNoise->GenUniformGrid2D(noiseOutput2D, worldPos.x, worldPos.z, Constants::CHUNK_SIZE, Constants::CHUNK_SIZE, Constants::FREQUENCY, Constants::SEED + 10);
		
		for (uint32_t x = 0; x < Constants::CHUNK_SIZE; x++) {
			for (uint32_t z = 0; z < Constants::CHUNK_SIZE; z++) {
				uint32_t height = (uint32_t)(noiseOutput2D[x + Constants::CHUNK_SIZE * z] * Constants::WORLD_HEIGHT);
				for (uint32_t y = 0; y < Constants::CHUNK_SIZE; y++) {
					uint32_t voxelWorldHeight = y + worldPos.y;
					m_Voxels[x][y][z].SetBlockType(voxelWorldHeight < height && noiseOutput3D[TO1D(x, y, z)] < Constants::THRESHOLD ? VoxelTypeIDs::Grass : VoxelTypeIDs::Air);
					//m_Voxels[x][y][z].SetBlockType(x % 2 == 0 && y % 2 == 0 && z % 2 == 0 ? VoxelTypeIDs::Sand : VoxelTypeIDs::Grass);
					//m_Voxels[x][y][z].SetBlockType(x % 2 == 0 && z % 2 == 0 ? VoxelTypeIDs::Grass : VoxelTypeIDs::Sand);
					//m_Voxels[x][y][z].SetBlockType(x % 2 == 0 && z % 2 == 0 ? VoxelTypeIDs::Grass : VoxelTypeIDs::Air);

				}
			}
		}
		delete[] noiseOutput3D;
		delete[] noiseOutput2D;
	}
};
