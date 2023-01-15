#pragma once
#include <RealEngine.h>

#include "Voxel.h"

class ChunkManager;

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

class Chunk {
public:
	static std::vector<std::shared_ptr<Chunk>> MemoryPool;

	Chunk(glm::ivec3 worldOffset, ChunkManager& manager);
	~Chunk() = default;

	void ReuseChunk(glm::ivec3 worldOffset);

	void UpdateMesh(float dt);
	void CreateMesh();
	
	void Render();

	inline Voxel& GetVoxel(glm::ivec3 pos) { return m_Voxels[pos.x][pos.y][pos.z]; }

	static const int CHUNK_SIZE = 16;
private:
	void CreateBuffers();
	void UpdateBuffers();

	void AddLeftFace(glm::ivec3& pos);
	void AddRightFace(glm::ivec3& pos);
	void AddBottomFace(glm::ivec3& pos);
	void AddTopFace(glm::ivec3& pos);
	void AddBackFace(glm::ivec3& pos);
	void AddFrontFace(glm::ivec3& pos);

	ChunkManager& m_ChunkManager;

	glm::ivec3 m_WorldOffset;

	// 3D array of voxels
	std::vector<std::vector<std::vector<Voxel>>> m_Voxels;

	// vectors to hold vertex and index data for rendering
	int m_VertIndex = 0;
	std::vector<glm::vec3> m_Vertices;
	int m_IndicesIndex = 0;
	std::vector<uint32_t> m_Indices;

	RealEngine::Ref<RealEngine::VertexArray> m_VertexArray;
	RealEngine::Ref<RealEngine::VertexBuffer> m_VertexBuffer;
};
