#pragma once
#include <RealEngine.h>

enum VoxelType {
	BlockType_Default = 0,
	BlockType_Grass,
	BlockType_Dirt,
	BlockType_Water,
	BlockType_Stone,
	BlockType_Wood,
	BlockType_Sand
};

enum VoxelSide {
	Side = 0,
	Top = 1,
	Bottom = 2
};

class Voxel {
public: 
	static void CreateTextureCords(RealEngine::Ref<RealEngine::Texture2D> textureAtlas);

	inline VoxelType GetBlockType() { return m_BlockType; }
	inline void SetBlockType(VoxelType blockType) { m_BlockType = blockType; }

	static struct VoxelTextureCord {
		std::array<RealEngine::Ref<RealEngine::SubTexture2D>, 3> Grass_Cords;
		RealEngine::Ref<RealEngine::SubTexture2D> Dirt_Cords;
		RealEngine::Ref<RealEngine::SubTexture2D> Water_Cords;
		RealEngine::Ref<RealEngine::SubTexture2D> Stone_Cords;
		std::array<RealEngine::Ref<RealEngine::SubTexture2D>, 3> Wood_Cords;
		RealEngine::Ref<RealEngine::SubTexture2D> Sand_Cords;
	} s_VoxelTextureCord;

	inline const glm::vec2* GetTexCord(VoxelSide side) {
		switch (m_BlockType) {
			case BlockType_Default:
				return nullptr;
			case BlockType_Grass:
				return Voxel::s_VoxelTextureCord.Grass_Cords[side]->GetTexCoords();
			case BlockType_Dirt:
				return Voxel::s_VoxelTextureCord.Dirt_Cords->GetTexCoords();
			case BlockType_Water:
				return Voxel::s_VoxelTextureCord.Water_Cords->GetTexCoords();
			case BlockType_Stone:
				return Voxel::s_VoxelTextureCord.Stone_Cords->GetTexCoords();
			case BlockType_Wood:
				return Voxel::s_VoxelTextureCord.Wood_Cords[side]->GetTexCoords();
			case BlockType_Sand:
				return Voxel::s_VoxelTextureCord.Sand_Cords->GetTexCoords();
			default:
				RE_ASSERT(false, "Block type not implemented yet")
				return nullptr;
		}
	}

	inline bool IsActive() { return m_Active; }
	inline void SetActive(bool active) { m_Active = active; }
private: 
	bool m_Active = true;
	VoxelType m_BlockType;
};
