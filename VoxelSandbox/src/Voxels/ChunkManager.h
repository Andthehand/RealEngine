#pragma once
#include "Chunk.h"

class ChunkManager {
public:
	ChunkManager(glm::ivec3& cameraPos);
	~ChunkManager() = default;
	void Render(RealEngine::EditorCamera& editorCamera);
	void OnImGuiRender();

	static const int RENDER_DISTANCE = 6;
	static const int WORLD_HEIGHT = 256;

	glm::vec4 frustumPlanes[6];
private:
	inline glm::ivec3 Vec3ToChunkCords(glm::ivec3 cords);
	inline glm::ivec3 ClampToNum(glm::ivec3& cords, int num);
	void UpdateChunkMap(glm::ivec3& cameraPos);

	glm::ivec3 m_PreviousCameraPos = glm::vec3(0);
	std::unordered_map<glm::ivec3, std::shared_ptr<Chunk>> m_ActiveChunks;
	std::unordered_map<glm::ivec3, std::shared_ptr<Chunk>> m_ChunksToDelete;
};
