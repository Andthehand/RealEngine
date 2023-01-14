#pragma once
#include "Chunk.h"

class ChunkManager {
public:
	ChunkManager();
	~ChunkManager() = default;
	void Render(RealEngine::EditorCamera& editorCamera);

	static const int RENDER_DISTANCE = 5;
	static const int WORLD_HEIGHT = 256;

	glm::vec4 frustumPlanes[6];
private:
	std::unordered_map<glm::vec3, Chunk> m_Chunks;
};
