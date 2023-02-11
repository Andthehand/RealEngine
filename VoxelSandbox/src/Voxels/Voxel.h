#pragma once
#include <RealEngine.h>

#include "JsonParsers/BlockJsonParser.h"

enum VoxelSide {
	Side = 0,
	Top = 1,
	Bottom = 2
};

//TODO: Make this dynamic
enum VoxelTypeIDs : uint16_t {
	Air = std::numeric_limits<uint16_t>::max(),
	Grass = 0,
	Sand = 1,
	Gravel = 2,
	OakLog = 3,
	OakPlank = 4,
	Obsidian = 5,
	Bedrock = 6,
};

//Holds static UVs and names for voxels
class VoxelHelper {
public:
	static void Init();
	static inline const uint32_t GetTexCordID(VoxelTypeIDs type, VoxelSide side) {
		//Check if the block exists
		if (s_BlockData.count(type)) {
			switch (side) {
				case Side:		return s_BlockData.at(type).Side;
				case Top:		return s_BlockData.at(type).Top;
				case Bottom:	return s_BlockData.at(type).Bottom;
			}
		}

		RE_ASSERT(false, "Block type not in unordered map");
		return 0;
	}
private:
	static RealEngine::Ref<RealEngine::Texture2DArray> s_Texture;
	static std::unordered_map<uint32_t, BlockData> s_BlockData;
};

//Class for keeping a voxel block data
class Voxel {
public:
	Voxel() = default;
	Voxel(VoxelTypeIDs type) : m_BlockType(type) {};

	inline VoxelTypeIDs GetBlockType() { return m_BlockType; }
	inline void SetBlockType(VoxelTypeIDs blockType) { m_BlockType = blockType; }

	inline bool IsAir() { return m_BlockType == Air; }

	operator bool() const {
		return m_BlockType != Air;
	}
	bool operator!=(const Voxel& other) const {
		return m_BlockType != ((Voxel&)other).GetBlockType();
	}
	bool operator==(const Voxel& other) const {
		return m_BlockType == ((Voxel&)other).GetBlockType();
	}

private:
	VoxelTypeIDs m_BlockType;
};
