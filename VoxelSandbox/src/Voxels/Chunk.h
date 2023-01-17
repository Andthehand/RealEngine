#pragma once
#include <RealEngine.h>

#include "Voxel.h"

class ChunkManager;

class Chunk {
public:
	Chunk(glm::ivec3 worldOffset, ChunkManager& manager);
	~Chunk() = default;

	void ReuseChunk(glm::ivec3 worldOffset);

	void CreateMesh();
	void CreateBuffers();
	void Render();

	inline Voxel& GetVoxel(glm::ivec3 pos) { return m_Voxels[pos.x][pos.y][pos.z]; }
public:
	static const int CHUNK_SIZE = 16;
	static std::vector<std::shared_ptr<Chunk>> MemoryPool;

	enum Status {
		NoData,
		Renderable,
		UpdateMesh,
		UploadBuffers,
		Proccessing
	};
	Status m_Status;
private:
	void UpdateBuffers();

	void AddLeftFace(glm::ivec3& pos);
	void AddRightFace(glm::ivec3& pos);
	void AddBottomFace(glm::ivec3& pos);
	void AddTopFace(glm::ivec3& pos);
	void AddBackFace(glm::ivec3& pos);
	void AddFrontFace(glm::ivec3& pos);

private:
	ChunkManager& m_ChunkManager;

	glm::ivec3 m_WorldOffset;

	// 3D array of voxels
	Voxel m_Voxels[16][16][16];

	// vectors to hold vertex and index data for rendering
	int m_VertIndex = 0;
	std::vector<glm::vec3> m_Vertices;
	int m_IndicesIndex = 0;
	std::vector<uint32_t> m_Indices;

	RealEngine::Ref<RealEngine::VertexArray> m_VertexArray;
	RealEngine::Ref<RealEngine::VertexBuffer> m_VertexBuffer;
};
