#pragma once
#include "Chunk.h"

#include <RealEngine/Utils/Threads/JobQueue.h>
#include <shared_mutex>

//This is for the unordered map
namespace std {
	template<> struct hash<glm::ivec3> {
		size_t operator()(const glm::ivec3& v) const {
			// Use the built-in hash function for the individual components
			size_t h1 = std::hash<int>()(v.x);
			size_t h2 = std::hash<int>()(v.y);
			size_t h3 = std::hash<int>()(v.z);
			// Combine the hash values using a combination of bitwise operators
			return h1 ^ (h2 << 1) ^ (h3 << 2);
		}
	};
}

class ChunkManager {
public:
	//Base functions
	ChunkManager(const glm::vec3& cameraPos);
	~ChunkManager();

	void Render(const RealEngine::EditorCamera& editorCamera);
	void OnImGuiRender();

	//Basic Statistics
	struct Statistics {
		uint32_t ChunksRendered = 0;
		float CameraDist = 0.0f;
	};
	Statistics& GetStatistics() { return m_Statistics; }
	void ResetStatistics();

	//If you can't find the chunk then return a nullptr
	const std::shared_ptr<Chunk> GetChunk(glm::ivec3 chunkPos) { 
		std::shared_lock lock(m_ChunkMutex);
		return m_ActiveChunks.find(chunkPos) != m_ActiveChunks.end() ? m_ActiveChunks.find(chunkPos)->second : nullptr; 
	}

	bool SetChunk(glm::ivec3& chunkPos, std::shared_ptr<Chunk> chunk) {
		std::unique_lock lock(m_ChunkMutex);
		return m_ActiveChunks.insert({ chunkPos, chunk }).second;
	}
private:
	void UpdateChunks();

	inline const glm::ivec3 ToChunkCoords(const glm::vec3& worldCoordinates) {
		return glm::ivec3{
			glm::floor(worldCoordinates.x / (float)Constants::CHUNK_SIZE),
			glm::floor(worldCoordinates.y / (float)Constants::CHUNK_SIZE),
			glm::floor(worldCoordinates.z / (float)Constants::CHUNK_SIZE)
		};
	}
private:
	//TODO: Make a Seperate load distance that should be at least 1 larger than the rendender distance
	int m_RenderDistance = 1;
	
	//This is global so I can freeze the frustum culling
	glm::vec4 m_FrustumPlanes[6];

	glm::ivec3 m_LastCameraChunkPosition;

	std::shared_mutex m_ChunkMutex;
	std::unordered_map<glm::ivec3, std::shared_ptr<Chunk>> m_ActiveChunks;
	
	bool m_FrustumFrozen = false;
	bool m_FreezePos = true;
	Statistics m_Statistics;

	RealEngine::JobQueue m_JobQueue;
};
