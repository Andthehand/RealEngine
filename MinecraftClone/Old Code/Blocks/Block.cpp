#include "Block.h"

Block::Block() 
	: m_ID(BlockID::Air) { }

Block::Block(BlockID& ID)
	: m_ID(ID) { }