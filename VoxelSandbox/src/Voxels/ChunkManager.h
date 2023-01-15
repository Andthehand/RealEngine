#pragma once
#include "Chunk.h"

class ChunkManager {
public:
	//Base functions
	ChunkManager(glm::ivec3& cameraPos);
	~ChunkManager() = default;
	void Render(RealEngine::EditorCamera& editorCamera);
	void OnImGuiRender();

	//Basic Statistics
	struct Statistics {
		glm::ivec3 CameraDist;
		uint32_t ChunksRendered = 0;
	};
	Statistics GetStatistics() { return m_Statistics; }
	void ResetStatistics();

	std::shared_ptr<Chunk> GetChunk(glm::ivec3 chunkPos) { return (m_ActiveChunks.find(chunkPos) != m_ActiveChunks.end()) ? m_ActiveChunks.find(chunkPos)->second : nullptr; }

public:
	//This is the amount of chunks that will be rendered in one direction around the camera
	static const int WORLD_HEIGHT = 256;
	int m_RenderDistance = 4;

	glm::vec4 frustumPlanes[6];
private:
	inline glm::ivec3 Vec3ToChunkCords(glm::ivec3 cords);
	inline glm::ivec3 ClampToNum(glm::ivec3& cords, int num);
	void UpdateChunkMap(glm::ivec3& cameraPos);

	glm::ivec3 m_PreviousCameraPos = glm::vec3(0);
	std::unordered_map<glm::ivec3, std::shared_ptr<Chunk>> m_ActiveChunks;

	Statistics m_Statistics;
};
