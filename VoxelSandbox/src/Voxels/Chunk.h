#pragma once
#include <RealEngine.h>

#include "Voxel.h"

class Chunk {
public:
	Chunk() = default;
	~Chunk();

	void Init(glm::vec3 worldOffset);


	void UpdateMesh(float dt);
	void CreateMesh();
	void Render();

	static const int CHUNK_SIZE = 16;
private:
	glm::vec3 m_WorldOffset;
	Voxel*** m_Voxels;
	
	std::vector<std::array<glm::vec3, 4>> m_Mesh;

	RealEngine::Ref<RealEngine::VertexArray> m_VertexArray;
	RealEngine::Ref<RealEngine::VertexBuffer> m_VertexBuffer;
};
