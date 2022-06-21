#pragma once

struct BlockData {
    enum class BlockClass {
        air,
        solid,
        transparent
    };

    enum class BlockType {
        bedrock,
        grass,
        dirt,
        sand,
        gravel,
        glass,
        oak_wood,
        oak_wood_plank,
        oak_leaves,
        Crafting_Table,
        air
    };

    BlockType type;
    BlockClass blockClass;

    static BlockClass typeToClass(BlockType type) {
        if (type == BlockType::air) {
            return BlockClass::air;
        }
        else if (type == BlockType::oak_leaves || type == BlockType::glass) {
            return BlockClass::transparent;
        }

        return BlockClass::solid;
    }
    BlockData(BlockData::BlockType type = BlockType::air) : type(type), blockClass(typeToClass(type)) {};
};