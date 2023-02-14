#pragma once
#include <RealEngine.h>

#include <glm/gtx/string_cast.hpp>

#include "Constants.h"
#include "Voxel.h"
#include "Utils/ScopeTimer.h"

class ChunkManager;

class ChunkReadWrite {
public:
	static inline void SaveVoxels(Voxel m_Voxels[Constants::CHUNK_SIZE][Constants::CHUNK_SIZE][Constants::CHUNK_SIZE], const glm::ivec3& worldPos) {
		std::ofstream chunkFile(Format(worldPos), std::ios::trunc | std::ios::binary);
		
		chunkFile.write((char*)m_Voxels, sizeof(Voxel) * Constants::CHUNK_SIZE * Constants::CHUNK_SIZE * Constants::CHUNK_SIZE);
		chunkFile.close();
	}

	//Returns if file exists and is read correctly
	static inline bool ReadVoxels(Voxel m_Voxels[Constants::CHUNK_SIZE][Constants::CHUNK_SIZE][Constants::CHUNK_SIZE], const glm::ivec3& worldPos) {
		if (!std::filesystem::is_directory("assets/chunks") || !std::filesystem::exists("assets/chunks")) { // Check if src folder exists
			std::filesystem::create_directory("assets/chunks"); // create src folder
			return false;
		}

		size_t size = sizeof(m_Voxels);

		//Check if the file exists
		struct stat buffer;
		if (stat(Format(worldPos).c_str(), &buffer) == 0) {
			std::ifstream chunkFile(Format(worldPos), std::ios::binary);
			
			chunkFile.read((char*)m_Voxels, sizeof(Voxel) * Constants::CHUNK_SIZE * Constants::CHUNK_SIZE * Constants::CHUNK_SIZE);

			return true;
		}
		return false;
	}

private:
	static inline std::string Format(const glm::ivec3& worldPos) {
		return "assets/chunks/" + glm::to_string(worldPos) + ".chunk";
	}

	inline const glm::ivec3 ToChunkCoords(const glm::vec3& worldCoordinates) {
		return glm::ivec3{
			glm::floor(worldCoordinates.x / (float)Constants::CHUNK_SIZE),
			glm::floor(worldCoordinates.y / (float)Constants::CHUNK_SIZE),
			glm::floor(worldCoordinates.z / (float)Constants::CHUNK_SIZE)
		};
	}
};

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
		glm::vec3 m_Position;
		uint32_t Data;
	};
	std::vector<VoxelBuffer> m_Vertices;
	std::vector<uint32_t> m_Indices;

	RealEngine::Ref<RealEngine::VertexArray> m_VertexArray;
	RealEngine::Ref<RealEngine::VertexBuffer> m_VertexBuffer;
};
