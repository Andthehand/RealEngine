#include "Chunk.h"

#include "ChunkManager.h"
#include "TerrainGenerator.h"
#include "Constants.h"

std::vector<std::shared_ptr<Chunk>> Chunk::MemoryPool;

Chunk::Chunk(glm::ivec3 worldOffset, ChunkManager& manager) 
	: m_WorldOffset(worldOffset), m_ChunkManager(manager) { }

//This is used for so that I don't have to keep re allocating memory for new Chunks
void Chunk::LoadVoxels() {
	//Generate new Voxels
	TerrainGenerator::CreateTerrain(m_Voxels, m_WorldOffset);

	m_Status = Status::UpdateMesh;
}

inline uint32_t CompressTextureCoords(uint8_t uv, uint8_t side, uint32_t texCoordID, uint32_t width, uint32_t height) {
	uint8_t UV = uv					  & 0x00000003;
	uint8_t Side = (side << 2)		  & 0x0000000C;
	uint16_t ID = (texCoordID << 4)   &	0x00003FF0;
	uint32_t XOffset = (width << 14)  &	0x003FC000;
	uint32_t YOffset = (height << 22) &	0x3FC00000;
	return UV | Side| ID | XOffset | YOffset;
}

inline int f(glm::ivec3& dims, int i, int j, int k) {
	return i + dims[0] * (j + dims[1] * k);
}

void Chunk::CreateMesh() {
	//Get the chunks around this chunk
	std::shared_ptr<Chunk> neighborChunks[6] = {
		m_ChunkManager.GetChunk({ m_WorldOffset.x - Constants::CHUNK_SIZE, m_WorldOffset.y, m_WorldOffset.z }),
		m_ChunkManager.GetChunk({ m_WorldOffset.x + Constants::CHUNK_SIZE, m_WorldOffset.y, m_WorldOffset.z }),
		m_ChunkManager.GetChunk({ m_WorldOffset.x, m_WorldOffset.y - Constants::CHUNK_SIZE, m_WorldOffset.z }),
		m_ChunkManager.GetChunk({ m_WorldOffset.x, m_WorldOffset.y + Constants::CHUNK_SIZE, m_WorldOffset.z }),
		m_ChunkManager.GetChunk({ m_WorldOffset.x, m_WorldOffset.y, m_WorldOffset.z - Constants::CHUNK_SIZE }),
		m_ChunkManager.GetChunk({ m_WorldOffset.x, m_WorldOffset.y, m_WorldOffset.z + Constants::CHUNK_SIZE })
	};

	// Sweep over each axis (X, Y and Z)
	for (int d = 0; d < 3; ++d) {
		int i, j, k, l, w, h;
		int u = (d + 1) % 3;
		int v = (d + 2) % 3;
		glm::ivec3 x = glm::ivec3(0);
		glm::ivec3 q = glm::ivec3(0);

		Voxel mask[Constants::CHUNK_SIZE * Constants::CHUNK_SIZE];
		q[d] = 1;

		// Check each slice of the chunk one at a time
		for (x[d] = -1; x[d] < Constants::CHUNK_SIZE;) {
			// Compute the mask
			int n = 0;
			for (x[v] = 0; x[v] < Constants::CHUNK_SIZE; ++x[v]) {
				for (x[u] = 0; x[u] < Constants::CHUNK_SIZE; ++x[u], n++) {
					// q determines the direction (X, Y or Z) that we are searching
					// m.IsBlockAt(x,y,z) takes global map positions and returns true if a block exists there
					Voxel blockCurrent = 0 <= x[d] ? m_Voxels[x[0]][x[1]][x[2]].GetBlockType() : Voxel(VoxelType::BlockType_Air);
					Voxel blockCompare = x[d] < Constants::CHUNK_SIZE - 1 ? m_Voxels[x[0] + q[0]][x[1] + q[1]][x[2] + q[2]].GetBlockType() : Voxel(VoxelType::BlockType_Air);

					if (blockCurrent.IsAir() == blockCompare.IsAir()) {
						mask[n] = Voxel(VoxelType::BlockType_Air);
					}
					else if (blockCurrent) {
						mask[n] = blockCurrent;
					}
					else {
						mask[n] = blockCompare;
					}
				}
			}

			++x[d];

			n = 0;

			// Generate a mesh from the mask using lexicographic ordering,      
			//   by looping over each block in this slice of the chunk
			for (j = 0; j < Constants::CHUNK_SIZE; ++j) {
				for (i = 0; i < Constants::CHUNK_SIZE;) {
					Voxel currentType = mask[n];
					if (currentType) {
						// Compute the width of this quad and store it in w                        
						//   This is done by searching along the current axis until mask[n + w] is false
						for (w = 1; i + w < Constants::CHUNK_SIZE && mask[n + w] == currentType; w++) {}

						// Compute the height of this quad and store it in h                        
						//   This is done by checking if every block next to this row (range 0 to w) is also part of the mask.
						//   For example, if w is 5 we currently have a quad of dimensions 1 x 5. To reduce triangle count,
						//   greedy meshing will attempt to expand this quad out to Constants::CHUNK_SIZE x 5, but will stop if it reaches a hole in the mask

						bool done = false;
						for (h = 1; j + h < Constants::CHUNK_SIZE; h++) {
							// Check each block next to this quad
							for (k = 0; k < w; ++k) {
								// If there's a hole in the mask, exit
								if (currentType != mask[n + k + h * Constants::CHUNK_SIZE]) {
									done = true;
									break;
								}
							}

							if (done)
								break;
						}

						x[u] = i;
						x[v] = j;

						// du and dv determine the size and orientation of this face
						glm::ivec3 du = glm::ivec3(0), dv = glm::ivec3(0);
						du[u] = w;
						dv[v] = h;

						uint32_t ID = currentType.GetTexCordID(VoxelSide::Side);
						uint8_t UV = 0;

						//Jank
						uint32_t side;
						int width = w, height = h;
						if (u != 1) {
							side = 0;
							width = w;
							height = h;
						}
						else {
							side = 1;
							height = w;
							width = h;
						}

						// Create a quad for this face. Colour, normal or textures are not stored in this block vertex format.
						m_Vertices.push_back({ glm::ivec3{x[0] + du[0], x[1] + du[1], x[2] + du[2] } + m_WorldOffset, CompressTextureCoords(UV++, side, ID, width, height) });        // Top right vertice position
						m_Vertices.push_back({ glm::ivec3{x[0], x[1], x[2] } + m_WorldOffset, CompressTextureCoords(UV++, side, ID, width, height) });                 // Top-left vertice position
						m_Vertices.push_back({ glm::ivec3{x[0] + dv[0], x[1] + dv[1], x[2] + dv[2] } + m_WorldOffset, CompressTextureCoords(UV++, side, ID, width, height) });       // Bottom left vertice position
						m_Vertices.push_back({ glm::ivec3{x[0] + du[0] + dv[0], x[1] + du[1] + dv[1], x[2] + du[2] + dv[2]} + m_WorldOffset, CompressTextureCoords(UV, side, ID, width, height) });  // Bottom right vertice position

						// Clear this part of the mask, so we don't add duplicate faces
						for (l = 0; l < h; ++l)
							for (k = 0; k < w; ++k)
								mask[n + k + l * Constants::CHUNK_SIZE].SetBlockType(VoxelType::BlockType_Air);

						// Increment counters and continue
						i += w;
						n += w;
					}
					else {
						i++;
						n++;
					}
				}
			}
		}
	}

	uint32_t offset = 0;
	for (uint32_t i = 0; i < (m_Vertices.size() / 4) * 6; i += 6) {
		m_Indices.push_back(offset + 0);
		m_Indices.push_back(offset + 1);
		m_Indices.push_back(offset + 2);

		m_Indices.push_back(offset + 2);
		m_Indices.push_back(offset + 3);
		m_Indices.push_back(offset + 0);

		offset += 4;
	}

	//Why try and render if there is nothing to render
	if (m_Vertices.size() != 0) {
		//Tells the main thread to upload the buffers
		m_Status = Status::UploadBuffers;
	}
	else {
		m_Status = Status::NoData;
		//Set the vectors to no size or capacity to save memory
		std::vector<VoxelBuffer>().swap(m_Vertices);
	}
}

void Chunk::CreateBuffers() {
	m_VertexArray = RealEngine::VertexArray::Create();
	m_VertexBuffer = RealEngine::VertexBuffer::Create(sizeof(VoxelBuffer) * (uint32_t)m_Vertices.size());
	m_VertexBuffer->SetLayout({
		{ RealEngine::ShaderDataType::Float3, "a_Position" },
		{ RealEngine::ShaderDataType::UInt, "a_Data" }
	});
	m_VertexArray->AddVertexBuffer(m_VertexBuffer);

	RealEngine::Ref<RealEngine::IndexBuffer> indexBuffer = RealEngine::IndexBuffer::Create((uint32_t)m_Indices.size());

	m_VertexArray->SetIndexBuffer(indexBuffer);

	UpdateBuffers();
}

void Chunk::UpdateBuffers() {
	m_VertexArray->GetVertexBuffers()[0]->SetData(std::data(m_Vertices), sizeof(VoxelBuffer) * (uint32_t)m_Vertices.size());
	m_VertexArray->GetIndexBuffer()->SetData(std::data(m_Indices), (uint32_t)m_Indices.size());

	std::vector<VoxelBuffer>().swap(m_Vertices);
	std::vector<uint32_t>().swap(m_Indices);

	m_Status = Status::Renderable;
}

void Chunk::Render() {
	m_VertexArray->Bind();
	RealEngine::RenderCommand::DrawIndexed(m_VertexArray, ((uint32_t)m_Vertices.size() / 4) * 6);
}
