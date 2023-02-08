#pragma once
#include <RealEngine.h>

#include "Constants.h"
#include "Voxel.h"

class ChunkManager;

class Chunk {
public:
	Chunk(glm::ivec3 worldOffset, ChunkManager& manager);
	~Chunk() = default;

	void LoadVoxels();

	void CreateMesh();
	void CreateBuffers();
	void Render();

	const glm::ivec3& GetPostition() const { return m_WorldOffset; }
	void SetPostition(glm::ivec3& position) { m_WorldOffset = position; }

	inline Voxel& GetVoxel(glm::ivec3 pos) { return m_Voxels[pos.x][pos.y][pos.z]; }
public:
	//This is used by the Chunk manager for better memory manigment
	static std::vector<std::shared_ptr<Chunk>> MemoryPool;

	enum Status {
		//Load all of the Voxel data
		Load,
		//There is no Data to render
		NoData,
		Renderable,
		//Mesh needs to be recreated
		UpdateMesh,
		//Only the main thread can upload stuff to gpu
		UploadBuffers,	
		Proccessing
	};
	Status m_Status = Status::Load;
private:
	void UpdateBuffers();

private:
	ChunkManager& m_ChunkManager;

	glm::ivec3 m_WorldOffset;

	// 3D array of voxels
	Voxel m_Voxels[Constants::CHUNK_SIZE][Constants::CHUNK_SIZE][Constants::CHUNK_SIZE];

	// vectors to hold vertex and index data for rendering
	struct VoxelBuffer {
		glm::vec3 Position;
		uint32_t Data;
	};
	std::vector<VoxelBuffer> m_Vertices;
	std::vector<uint32_t> m_Indices;

	RealEngine::Ref<RealEngine::VertexArray> m_VertexArray;
	RealEngine::Ref<RealEngine::VertexBuffer> m_VertexBuffer;
};
