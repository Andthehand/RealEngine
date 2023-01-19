#pragma once
#include "Chunk.h"

#include <RealEngine/Utils/Threads/JobQueue.h>

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
	ChunkManager(glm::ivec3& cameraPos);
	~ChunkManager();
	void Render(RealEngine::EditorCamera& editorCamera);
	void OnImGuiRender();

	//Basic Statistics
	struct Statistics {
		glm::ivec3 CameraDist;
		uint32_t ChunksRendered = 0;
	};
	Statistics& GetStatistics() { return m_Statistics; }
	void ResetStatistics();

	std::shared_ptr<Chunk> GetChunk(glm::ivec3 chunkPos) { return (m_ActiveChunks.find(chunkPos) != m_ActiveChunks.end()) ? m_ActiveChunks.find(chunkPos)->second : nullptr; }

public:
	//This is the amount of chunks that will be rendered in one direction around the camera
	static const int WORLD_HEIGHT = 256;
	int m_RenderDistance = 5;
private:
	inline glm::ivec3 Vec3ToChunkCords(glm::ivec3 cords);
	inline glm::ivec3 ClampToNum(glm::ivec3& cords, int num);
	void UpdateChunkMap(glm::ivec3& cameraPos);

private:
	//This is global so I can freeze the frustum culling
	glm::vec4 m_FrustumPlanes[6];

	glm::ivec3 m_PreviousCameraPos = glm::vec3(0);
	std::unordered_map<glm::ivec3, std::shared_ptr<Chunk>> m_ActiveChunks;
	
	bool m_FrustumFrozen = false;
	Statistics m_Statistics;

	//All of the stuff to do with threads
	static const uint32_t NUM_MAX_THREADS = 6;
	RealEngine::JobQueue m_JobQueue;
};
