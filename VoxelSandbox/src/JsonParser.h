#pragma once
#include <RealEngine.h>

struct BlockData {
	uint32_t Side;
	uint32_t Top;
	uint32_t Bottom;
};

enum VoxelSide {
	Side = 0,
	Top = 1,
	Bottom = 2
};

class JsonParser {
public:
	static void Init();

	static inline const uint32_t GetTexCordID(std::string block, VoxelSide side) {
		//Check if the block exists
		if(s_BlockData.count(block)) {
			switch (side) {
				case Side:		return s_BlockData.at(block).Side;
				case Top:		return s_BlockData.at(block).Top;
				case Bottom:	return s_BlockData.at(block).Bottom;
			}
		}

		RE_ASSERT(false, "Block type not in unordered map");
		return 0;
	}
private:
	static RealEngine::Ref<RealEngine::Texture2DArray> s_Texture;
	static std::unordered_map<std::string, BlockData> s_BlockData;
};

