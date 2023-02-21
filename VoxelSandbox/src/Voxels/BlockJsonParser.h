#pragma once
#include <RealEngine.h>

//TODO: Make parse a template and move this to Voxel.h
struct BlockData {
	std::string Name;
	uint32_t Side;
	uint32_t Top;
	uint32_t Bottom;
};

class BlockJsonParser {
public:
	static void Parse(std::unordered_map<uint32_t, BlockData>& blockData, RealEngine::Ref<RealEngine::Texture2DArray>& texture);
};

