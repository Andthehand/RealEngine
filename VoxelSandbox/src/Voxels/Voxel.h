#pragma once
#include <RealEngine.h>

enum VoxelType : uint8_t {
	BlockType_Air = 0,
	BlockType_Grass,
	BlockType_Dirt,
	BlockType_Water,
	BlockType_Stone,
	BlockType_Wood,
	BlockType_Sand
};

class Voxel {
public: 
	Voxel() = default;
	Voxel(VoxelType type) : m_BlockType(type) {};

	inline VoxelType GetBlockType() { return m_BlockType; }
	inline void SetBlockType(VoxelType blockType) { m_BlockType = blockType; }

	enum VoxelTextureCordID : int {
		Grass_Cords_Top,
		Grass_Cords_Bottom,
		Grass_Cords_Side,
		Dirt_Cords,
		Water_Cords,
		Stone_Cords,
		Wood_Cords_Top,
		Wood_Cords_Bottom,
		Wood_Cords_Side,
		Sand_Cords
	};

	inline bool IsAir() { return m_BlockType == VoxelType::BlockType_Air; }
	
	operator bool() const {
		return m_BlockType != VoxelType::BlockType_Air;
	}
	bool operator!=(const Voxel& other) const {
		return m_BlockType != ((Voxel&)other).GetBlockType();
	}
	bool operator==(const Voxel& other) const {
		return m_BlockType == ((Voxel&)other).GetBlockType();
	}

private:
	VoxelType m_BlockType;
};
