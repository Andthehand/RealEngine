#pragma once
#include "BlockData.h"
#include <array>

struct BlockName {
private:
    using Name = std::pair<BlockData::BlockType, const char*>;

public:
    static constexpr size_t BlockCount = 17;
    using NameArray = std::array<const char*, BlockCount>;
    static constexpr std::array<Name, BlockCount> BlockNames{ {
       {BlockData::BlockType::bedrock, "Bedrock"},
       {BlockData::BlockType::grass, "Grass"},
       {BlockData::BlockType::dirt, "Dirt"},
       {BlockData::BlockType::sand, "Sand"},
       {BlockData::BlockType::gravel, "Gravel"},
       {BlockData::BlockType::glass, "Glass"},
       {BlockData::BlockType::oak_wood, "Oak Wood"},
       {BlockData::BlockType::oak_wood_plank, "Oak Wood Plank"},
       {BlockData::BlockType::oak_leaves, "Oak Leaves"},
       {BlockData::BlockType::Crafting_Table, "Crafting Table"}
    }};

    static consteval NameArray getBlockNames() {
        NameArray names{};
        for (int32_t i = 0; i < BlockCount; ++i) { names[i] = BlockNames[i].second; }

        return names;
    }

    static int32_t BlockTypeToIndex(BlockData::BlockType type);
    static const char* blockTypeToName(BlockData::BlockType type);
};