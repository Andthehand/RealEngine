#include "Chunk.h"

#include "BlockMesh.h"
#include "RealEngine.h"

void Chunk::CreateMesh() {
    /*
    static RealEngine::Ref<std::vector<BlockVertex>> solidVertices = RealEngine::CreateRef<std::vector<BlockVertex>>(100000);
    static RealEngine::Ref<std::vector<BlockVertex>> semiTransparentVertices =
        RealEngine::CreateRef<std::vector<BlockVertex>>(100000);

    int32_t solidVertexCount = 0;
    int32_t semiTransparentVertexCount = 0;

    const std::array<glm::ivec3, 6> offsetsToCheck = { {
       {1, 0, 0},
       {-1, 0, 0},
       {0, 1, 0},
       {0, -1, 0},
       {0, 0, 1},
       {0, 0, -1},
    }};

    for (int32_t x = 16 - 1; x >= 0; --x) {
        for (int32_t y = 256 - 1; y >= 0; --y) {
            for (int32_t z = 16 - 1; z >= 0; --z) {
                glm::ivec3 blockPos = { x, y, z };
                const auto& [type, blockClass] = data[x][y][z];
                if (blockClass == BlockData::BlockClass::air) {
                    continue;
                }

                for (const glm::ivec3& offset : offsetsToCheck) {
                    const BlockData* block = getBlockAtOptimized(blockPos + offset, world);
                    if (block != nullptr &&
                        (block->blockClass == blockClass || block->blockClass == BlockData::BlockClass::solid)) {
                        continue;
                    }

                    for (const auto& vertex : BlockMesh::getVerticesFromDirection(offset)) {
                        BlockVertex vert = vertex;
                        vert.offset(x, y, z);
                        vert.setType(offset, type);

                        uint8_t occlusionLevel = 3;
                        if (useAmbientOcclusion) {
                            if (offset.y == -1) {
                                occlusionLevel = 0;
                            }
                            else {
                                occlusionLevel = calculateOcclusionLevel(blockPos, vert.getPosition() - blockPos, *this, world);
                            }
                        }
                        vert.setOcclusionLevel(occlusionLevel);

                        if (blockClass == BlockData::BlockClass::semiTransparent ||
                            blockClass == BlockData::BlockClass::transparent) {
                            semiTransparentVertices->at(semiTransparentVertexCount) = vert;
                            semiTransparentVertexCount++;
                        }
                        else {
                            solidVertices->at(solidVertexCount) = vert;
                            solidVertexCount++;
                        }
                    }
                }
            }
        }
    }
        */
}