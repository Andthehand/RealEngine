#pragma once
#include <RealEngine.h>

#include <glm/gtx/string_cast.hpp>

#include "Core/Constants.h"
#include "Voxels/Voxel.h"
#include "Utils/ScopeTimer.h"

class ChunkManager;

class ChunkReadWrite {
public:
	static inline void SaveVoxels(Voxel m_Voxels[Constants::CHUNK_SIZE][Constants::CHUNK_SIZE][Constants::CHUNK_SIZE], const glm::ivec3& worldPos) {
		//Conpress to Run line encoding
		std::vector<std::pair<uint32_t, VoxelTypeIDs>> RLE;
		VoxelTypeIDs currentType = (VoxelTypeIDs)1111111;
		uint32_t currentOffset = 1;
		for (uint32_t z = 0; z < Constants::CHUNK_SIZE; z++) {
			for (uint32_t y = 0; y < Constants::CHUNK_SIZE; y++) {
				for (uint32_t x = 0; x < Constants::CHUNK_SIZE; x++) {
					if (m_Voxels[x][y][z] == currentType) {
						RLE.back().first++;
						currentOffset++;
					}
					else {
						currentType = m_Voxels[x][y][z].GetBlockType();
						RLE.push_back({ currentOffset++, currentType });
					}
				}
			}
		}

		std::ofstream chunkFile(Format(worldPos), std::ios::trunc | std::ios::binary);
		chunkFile.write((char*)RLE.data(), sizeof(std::pair<uint32_t, VoxelTypeIDs>) * RLE.size());
		chunkFile.close();
	}

	//Returns if file exists and is read correctly
	static inline bool ReadVoxels(Voxel m_Voxels[Constants::CHUNK_SIZE][Constants::CHUNK_SIZE][Constants::CHUNK_SIZE], const glm::ivec3& worldPos) {
		if (!std::filesystem::is_directory("assets/chunks") || !std::filesystem::exists("assets/chunks")) { // Check if src folder exists
			std::filesystem::create_directory("assets/chunks"); // create src folder
			return false;
		}

		//Check if the file exists
		struct stat buffer;
		if (stat(Format(worldPos).c_str(), &buffer) == 0) {
			uint32_t size = (uint32_t)Filesize(Format(worldPos).c_str()) / sizeof(std::pair < uint32_t, VoxelTypeIDs>);
			std::pair<uint32_t, VoxelTypeIDs>* RLE = new std::pair<uint32_t, VoxelTypeIDs>[size];

			std::ifstream chunkFile(Format(worldPos), std::ios::binary);
			chunkFile.read((char*)RLE, Filesize(Format(worldPos).c_str()));

			//Uncompress RLE
			uint32_t lastOffset = 0;
			for (uint32_t i = 0; i < size; i++) {
				uint32_t offset = RLE[i].first;
				VoxelTypeIDs type = RLE[i].second;
				for (uint32_t n = lastOffset; n < offset; n++) {
					uint32_t z = (n / (Constants::CHUNK_SIZE * Constants::CHUNK_SIZE)) % Constants::CHUNK_SIZE;
					uint32_t y = (n / Constants::CHUNK_SIZE) % Constants::CHUNK_SIZE;
					uint32_t x = n % Constants::CHUNK_SIZE;
					m_Voxels[x][y][z] = type;
				}
				lastOffset = offset;
			}
			delete[] RLE;

			return true;
		}
		return false;
	}

private:
	static inline std::ifstream::pos_type Filesize(const char* filename) {
		std::ifstream in(filename, std::ifstream::ate | std::ifstream::binary);
		return in.tellg();
	}

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

//This is mainly used for statistics
class ChunkRenderer {
public:
	static inline void Render(RealEngine::Ref<RealEngine::VertexArray> vertexArray) {
		m_VerticeCount += vertexArray->GetIndexBuffer()->GetCount();
		
		vertexArray->Bind();
		RealEngine::RenderCommand::DrawIndexed(vertexArray);
	}

	static inline void ResetStatistics() { m_VerticeCount = 0; }

	static inline uint32_t GetVerticeCount() { return m_VerticeCount; }
	static inline uint32_t GetTriangleCount() { return m_VerticeCount / 2; }
	static inline uint32_t GetQuadCount() { return m_VerticeCount / 4; }
	static inline uint32_t GetIndiceCount() { return m_VerticeCount / 4 * 6; }
private:
	static uint32_t m_VerticeCount;
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
