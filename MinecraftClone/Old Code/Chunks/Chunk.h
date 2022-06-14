#pragma once
#include <iostream>
#include <cstdint>
#include <vector>

#include <RealEngine.h>

#include "PerlinNoise.h"
#include "Blocks/Block.h"
#include "Chunks/ChunkCoordinates.h"

//MAX length and width
const static uint16_t MAXCHUNK = 16;
const static uint16_t MAXHEIGHT = 256;

class Chunk {
public:
    Chunk(PerlinNoise& noise, ChunkCoordinates chunkCoords) : m_ChunkCoordinates(chunkCoords) {
        //Since m_ChunkCoordinates is not in world space we have to convert
        ChunkCoordinates worldCoords = ToChunkWorldCoordinates(m_ChunkCoordinates);

        for (int x = 0; x < MAXCHUNK; x++) {
            for (int z = 0; z < MAXCHUNK; z++) {                
                //Get the y value of the top block
                int firstBlock = noise.GetHeightValue(x + worldCoords.x, z + worldCoords.z) + MAXHEIGHT / 2;
                //int firstBlock = 1;
                for (int y = 0; y < MAXHEIGHT; y++) {
                    //Only does first layer of dirt
                    if (y == firstBlock) {
                        int toArray = To1DArray(x, z, y);
                        //RE_WARN("ArrayIndex = {0}, x={1} z={2} y={3}", toArray, x, z, y);
                        m_Blocks[toArray].m_ID = BlockID::Dirt;

                        glm::vec3 position = { x + worldCoords.x, y, z + worldCoords.z };
                        std::array<glm::vec3, 24> vertexPositions;

                        //Front
                        vertexPositions[0] = (glm::vec3{ 1.0f, 1.0f, 0.0f } + position);
                        vertexPositions[1] = (glm::vec3{ 1.0f, 0.0f, 0.0f } + position);
                        vertexPositions[2] = (glm::vec3{ 0.0f, 0.0f, 0.0f } + position);
                        vertexPositions[3] = (glm::vec3{ 0.0f, 1.0f, 0.0f } + position);

                        //Back
                        vertexPositions[4] = (glm::vec3{ 1.0f, 1.0f, 1.0f } + position);
                        vertexPositions[5] = (glm::vec3{ 1.0f, 0.0f, 1.0f } + position);
                        vertexPositions[6] = (glm::vec3{ 0.0f, 0.0f, 1.0f } + position);
                        vertexPositions[7] = (glm::vec3{ 0.0f, 1.0f, 1.0f } + position);


                        //Left
                        vertexPositions[8] = (glm::vec3{ 1.0f, 1.0f, 0.0f } + position);
                        vertexPositions[9] = (glm::vec3{ 1.0f, 0.0f, 0.0f } + position);
                        vertexPositions[10] = (glm::vec3{ 1.0f, 0.0f, 1.0f } + position);
                        vertexPositions[11] = (glm::vec3{ 1.0f, 1.0f, 1.0f } + position);
                        //Right
                        vertexPositions[12] = (glm::vec3{ 0.0f, 1.0f, 0.0f } + position);
                        vertexPositions[13] = (glm::vec3{ 0.0f, 0.0f, 0.0f } + position);
                        vertexPositions[14] = (glm::vec3{ 0.0f, 0.0f, 1.0f } + position);
                        vertexPositions[15] = (glm::vec3{ 0.0f, 1.0f, 1.0f } + position);

                        //Top
                        vertexPositions[16] = (glm::vec3{ 1.0f, 1.0f, 0.0f } + position);
                        vertexPositions[17] = (glm::vec3{ 1.0f, 1.0f, 1.0f } + position);
                        vertexPositions[18] = (glm::vec3{ 0.0f, 1.0f, 1.0f } + position);
                        vertexPositions[19] = (glm::vec3{ 0.0f, 1.0f, 0.0f } + position);
                        //Bottom
                        vertexPositions[20] = (glm::vec3{ 1.0f, 0.0f, 0.0f } + position);
                        vertexPositions[21] = (glm::vec3{ 1.0f, 0.0f, 1.0f } + position);
                        vertexPositions[22] = (glm::vec3{ 0.0f, 0.0f, 1.0f } + position);
                        vertexPositions[23] = (glm::vec3{ 0.0f, 0.0f, 0.0f } + position);
                    
                        m_BlockPositions.push_back(vertexPositions);
                    }
                    else {
                        m_Blocks[To1DArray(x, z, y)].m_ID = BlockID::Air;
                    }
                }
            }
        }
    }

    void Render(PerlinNoise& noise) {
        const ChunkCoordinates worldCoords = ToChunkWorldCoordinates(m_ChunkCoordinates);
        
        
        for (const auto& vertexPositions : m_BlockPositions) {
            RealEngine::Renderer::DrawBlock(vertexPositions);
        }
        
        /*
        for (int x = 0; x < MAXCHUNK; x++) {
            for (int z = 0; z < MAXCHUNK; z++) {
                int y = noise.GetHeightValue( x + worldCoords.x, z + worldCoords.z) + MAXHEIGHT / 2;
                RealEngine::Renderer::DrawBlock({ x + worldCoords.x, y, z + worldCoords.z });
            }
        }
        */
    }

    ~Chunk() {
        //Since I use new
        m_BlockPositions.clear();
        delete[] m_Blocks;
    }

    static inline ChunkCoordinates ToChunkWorldCoordinates(ChunkCoordinates coords) {
        return coords * MAXCHUNK;
    }

    static inline int To1DArray(int x, int z, int y) {
        //Abs because it could be negative
        return (std::abs(x) * MAXCHUNK) + (std::abs(y) * MAXHEIGHT) + z;
    }
private:
    ChunkCoordinates m_ChunkCoordinates;

    Block* m_Blocks = new Block[MAXCHUNK * MAXCHUNK * MAXHEIGHT];
    std::vector<std::array<glm::vec3, 24>> m_BlockPositions;
};