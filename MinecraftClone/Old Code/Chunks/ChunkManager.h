#pragma once
#include <cstdint>
#include <unordered_map>

#include <RealEngine.h>

#include "PerlinNoise.h"
#include "Chunk.h"

class ChunkManager {
public:
    ChunkManager(int = 5);

    void OnUpdate(const glm::vec3& playerPosition);

    void RenderAll(const RealEngine::Camera& camera);
    static inline ChunkCoordinates PositionToChunkPosition(const glm::vec2& position) {
        //TODO: Fix
        return ChunkCoordinates{ (int32_t)std::trunc(position.x / 16.0f), (int32_t)std::trunc(position.y / 16.0f) };
    }

private:
    PerlinNoise m_Noise;
    int m_RenderDistance;

    ChunkCoordinates m_CurrentChunk = { 0, 0 };
    std::unordered_map<ChunkCoordinates, std::unique_ptr<Chunk>, ChunkCoordinateHash> m_Chunks;
};