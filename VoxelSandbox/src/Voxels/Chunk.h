#pragma once
#include <RealEngine.h>

#include "Voxel.h"

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
	Chunk(glm::ivec3 worldOffset);
	~Chunk();

	void UpdateMesh(float dt);
	void CreateMesh();
	
	void Render();

	static const int CHUNK_SIZE = 16;
private:
	void CreateBuffers();

	void AddLeftFace(int x, int y, int z);
	void AddRightFace(int x, int y, int z);
	void AddBottomFace(int x, int y, int z);
	void AddTopFace(int x, int y, int z);
	void AddBackFace(int x, int y, int z);
	void AddFrontFace(int x, int y, int z);

	glm::ivec3 m_WorldOffset;

	// 3D array of voxels
	std::vector<std::vector<std::vector<Voxel>>> m_Voxels;

	// vectors to hold vertex and index data for rendering
	std::vector<glm::vec3> m_Vertices;
	std::vector<uint32_t> m_Indices;

	RealEngine::Ref<RealEngine::VertexArray> m_VertexArray;
	RealEngine::Ref<RealEngine::VertexBuffer> m_VertexBuffer;
};
