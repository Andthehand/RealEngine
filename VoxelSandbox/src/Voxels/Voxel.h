#pragma once

enum BlockType {
	BlockType_Default = 0,
	BlockType_Grass,
	BlockType_Dirt,
	BlockType_Water,
	BlockType_Stone,
	BlockType_Wood,
	BlockType_Sand,
	BlockType_NumTypes,
};

class Voxel {
public: 
	inline BlockType GetBlockType() { return m_BlockType; }
	inline void SetBlockType(BlockType blockType) { m_BlockType = blockType; }

	inline bool IsActive() { return m_Active; }
	inline void SetActive(bool active) { m_Active = active; }
private: 
	bool m_Active = true;
	BlockType m_BlockType;
};
