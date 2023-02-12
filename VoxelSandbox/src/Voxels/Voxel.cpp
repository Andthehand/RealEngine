#include "Voxel.h"

RealEngine::Ref<RealEngine::Texture2DArray> VoxelHelper::s_Texture;
std::unordered_map<uint32_t, BlockData> VoxelHelper::s_BlockData;

void VoxelHelper::Init() {
	BlockJsonParser::Parse(s_BlockData, s_Texture);
}
