#pragma once
#include <cstdint>
#include <iostream>
#include <string>

enum class BlockID : uint16_t{
    Air,
    Dirt

};

class Block {
public:
    BlockID m_ID;

public:
    Block();
    Block(BlockID& ID);
};

//For BlockID cout
static std::ostream& operator<<(std::ostream& os, const BlockID& ID) {
    std::string out;

    switch (ID) {
    case(BlockID::Air):
        out = "Air";
        break;
    case(BlockID::Dirt):
        out = "Dirt";
        break;
    default:
        out = "BlockID not implemented in operator overload";
        break;
    }

    return os << out;
}

//For Block cout
static std::ostream& operator<<(std::ostream& os, const Block& block) {
    return os << "[" << block.m_ID << "]";
}