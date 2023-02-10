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

enum VoxelSide {
	Side = 0,
	Top = 1,
	Bottom = 2
};

class Voxel {
public: 
	Voxel() = default;
	Voxel(VoxelType type) : m_BlockType(type) {};

	static void UploadTextureCords();

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

	inline const uint32_t GetTexCordID(VoxelSide side) {
		switch (m_BlockType) {
			case BlockType_Air:
				break;
			case BlockType_Grass:
				switch (side) {
					case Side:
						return VoxelTextureCordID::Grass_Cords_Side;
					case Top:
						return VoxelTextureCordID::Grass_Cords_Top;
					case Bottom:
						return VoxelTextureCordID::Grass_Cords_Bottom;
				}
				break;
			case BlockType_Dirt:
				return VoxelTextureCordID::Dirt_Cords;
			case BlockType_Water:
				return VoxelTextureCordID::Water_Cords;
			case BlockType_Stone:
				return VoxelTextureCordID::Stone_Cords;
			case BlockType_Wood:
				switch (side) {
				case Side:
					return VoxelTextureCordID::Wood_Cords_Side;
				case Top:
					return VoxelTextureCordID::Wood_Cords_Top;
				case Bottom:
					return VoxelTextureCordID::Wood_Cords_Bottom;
				}
				break;
			case BlockType_Sand:
				return VoxelTextureCordID::Sand_Cords;
		}

		RE_ASSERT(false, "Block type not implemented yet or block not loaded");
		return 0;
	}

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
	static RealEngine::Ref<RealEngine::TextureBuffer> s_TextureBuffer;

	VoxelType m_BlockType;
};
