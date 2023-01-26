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

void Chunk::CreateMesh() {
	m_VertIndex = 0;
	m_IndicesIndex = 0;

	//Get the chunks around this chunk
	std::shared_ptr<Chunk> neighborChunks[6] = {
		m_ChunkManager.GetChunk({ m_WorldOffset.x - Constants::CHUNK_SIZE, m_WorldOffset.y, m_WorldOffset.z }),
		m_ChunkManager.GetChunk({ m_WorldOffset.x + Constants::CHUNK_SIZE, m_WorldOffset.y, m_WorldOffset.z }),
		m_ChunkManager.GetChunk({ m_WorldOffset.x, m_WorldOffset.y - Constants::CHUNK_SIZE, m_WorldOffset.z }),
		m_ChunkManager.GetChunk({ m_WorldOffset.x, m_WorldOffset.y + Constants::CHUNK_SIZE, m_WorldOffset.z }),
		m_ChunkManager.GetChunk({ m_WorldOffset.x, m_WorldOffset.y, m_WorldOffset.z - Constants::CHUNK_SIZE }),
		m_ChunkManager.GetChunk({ m_WorldOffset.x, m_WorldOffset.y, m_WorldOffset.z + Constants::CHUNK_SIZE })
	};

	for (int x = 0; x < Constants::CHUNK_SIZE; x++) {
		for (int y = 0; y < Constants::CHUNK_SIZE; y++) {
			for (int z = 0; z < Constants::CHUNK_SIZE; z++) {
				if (!m_Voxels[x][y][z].IsActive()) continue;

				if (m_VertIndex + 24 > m_Vertices.size()) {
					m_Vertices.resize(m_VertIndex + 24 * 2);
				}
				if (m_IndicesIndex + 36 > m_Indices.size()) {
					m_Indices.resize(m_IndicesIndex + 36 * 2);
				}

				//AddLeftFace(glm::ivec3{ 0, 0, 0 });

				glm::ivec3 tempCords = { x + m_WorldOffset.x, y + m_WorldOffset.y, z + m_WorldOffset.z };

				/// Check for inactive voxels around the current voxel
				if (x == 0) {
																	//TODO: Fix this GetVoxel function call
					if (neighborChunks[0] == nullptr || !neighborChunks[0]->GetVoxel({ 15, y, z }).IsActive()) {
						AddLeftFace(tempCords);
					}
				}
				else if (!m_Voxels[x - 1][y][z].IsActive()) {
					// Add a face to the vertices and indices for the left side
					AddLeftFace(tempCords);
				}

				if (x == Constants::CHUNK_SIZE - 1) {
					if (neighborChunks[1] == nullptr || !neighborChunks[1]->GetVoxel({ 0, y, z }).IsActive()) {
						AddRightFace(tempCords);
					}
				}
				else if (!m_Voxels[x + 1][y][z].IsActive()) {
					// Add a face to the vertices and indices for the right side
					AddRightFace(tempCords);
				}

				if (y == 0) {
					if (neighborChunks[2] == nullptr || !neighborChunks[2]->GetVoxel({ x, 15, z }).IsActive()) {
						AddBottomFace(tempCords);
					}
				}
				else if (!m_Voxels[x][y - 1][z].IsActive()) {
					// Add a face to the vertices and indices for the bottom side
					AddBottomFace(tempCords);
				}

				if (y == Constants::CHUNK_SIZE - 1) {
					if (neighborChunks[3] == nullptr || !neighborChunks[3]->GetVoxel({ x, 0, z }).IsActive()) {
						AddTopFace(tempCords);
					}
				}
				else if (!m_Voxels[x][y + 1][z].IsActive()) {
					// Add a face to the vertices and indices for the top side
					AddTopFace(tempCords);
				}

				if (z == 0) {
					if (neighborChunks[4] == nullptr || !neighborChunks[4]->GetVoxel({ x, y, 15 }).IsActive()) {
						AddBackFace(tempCords);
					}
				}
				else if (!m_Voxels[x][y][z - 1].IsActive()) {
					// Add a face to the vertices and indices for the back side
					AddBackFace(tempCords);
				}
				
				if (z == Constants::CHUNK_SIZE - 1) {
					if (neighborChunks[5] == nullptr || !neighborChunks[5]->GetVoxel({ x , y, 0 }).IsActive()) {
						AddFrontFace(tempCords);
					}
				}
				else if (!m_Voxels[x][y][z + 1].IsActive()) {
					// Add a face to the vertices and indices for the front side
					AddFrontFace(tempCords);
				}
			}
		}
	}

	m_Status = Status::NoData;
	//Why try and render if there is nothing to render
	if (m_VertIndex != 0) {
		//Tells the main thread to upload the buffers
		m_Status = Status::UploadBuffers;
	}
	else {
		m_Status = Status::NoData;
		//Set the vectors to no size or capacity to save memory
		std::vector<VoxelBuffer>().swap(m_Vertices);
		std::vector<uint32_t>().swap(m_Indices);
	}
}

void Chunk::CreateBuffers() {
	m_VertexArray = RealEngine::VertexArray::Create();
	m_VertexBuffer = RealEngine::VertexBuffer::Create(sizeof(VoxelBuffer) * m_VertIndex);
	m_VertexBuffer->SetLayout({
		{ RealEngine::ShaderDataType::Float3, "a_Position" },
		{ RealEngine::ShaderDataType::Float2, "a_TexCoord" }
	});
	m_VertexArray->AddVertexBuffer(m_VertexBuffer);

	RealEngine::Ref<RealEngine::IndexBuffer> indexBuffer = RealEngine::IndexBuffer::Create(m_IndicesIndex);

	m_VertexArray->SetIndexBuffer(indexBuffer);

	UpdateBuffers();
}

void Chunk::UpdateBuffers() {
	m_VertexArray->GetVertexBuffers()[0]->SetData(std::data(m_Vertices), sizeof(VoxelBuffer) * m_VertIndex);
	m_VertexArray->GetIndexBuffer()->SetData(std::data(m_Indices), m_IndicesIndex);

	m_Status = Status::Renderable;
}

void Chunk::Render() {
	m_VertexArray->Bind();
	RealEngine::RenderCommand::DrawIndexed(m_VertexArray, m_IndicesIndex);
}

// Helper function to add a face to the left side of the current voxel
void Chunk::AddLeftFace(glm::ivec3& pos) {
	m_Vertices[m_VertIndex++] = { glm::vec3(pos), { 0.0f, 0.0f } };
	m_Vertices[m_VertIndex++] = { glm::vec3(pos.x,pos.y, pos.z + 1), { 1.0f, 0.0f } };
	m_Vertices[m_VertIndex++] = { glm::vec3(pos.x ,pos.y + 1, pos.z + 1), { 1.0f, 1.0f } };
	m_Vertices[m_VertIndex++] = { glm::vec3(pos.x ,pos.y + 1, pos.z), { 0.0f, 1.0f } };
	// Add the indices for the vertices of the left face
	m_Indices[m_IndicesIndex++] = m_VertIndex - 4;
	m_Indices[m_IndicesIndex++] = m_VertIndex - 3;
	m_Indices[m_IndicesIndex++] = m_VertIndex - 2;
	m_Indices[m_IndicesIndex++] = m_VertIndex - 4;
	m_Indices[m_IndicesIndex++] = m_VertIndex - 2;
	m_Indices[m_IndicesIndex++] = m_VertIndex - 1;
}

// Helper function to add a face to the right side of the current voxel
void Chunk::AddRightFace(glm::ivec3& pos) {
	m_Vertices[m_VertIndex++] = { glm::vec3(pos.x + 1,pos.y, pos.z), { 1.0f, 0.0f } };
	m_Vertices[m_VertIndex++] = { glm::vec3(pos.x + 1,pos.y + 1, pos.z), { 1.0f, 1.0f } };
	m_Vertices[m_VertIndex++] = { glm::vec3(pos.x + 1,pos. y + 1, pos.z + 1), { 0.0f, 1.0f } };
	m_Vertices[m_VertIndex++] = { glm::vec3(pos.x + 1,pos. y, pos.z + 1), { 0.0f, 0.0f } };
	// Add the indices for the vertices of the right face
	m_Indices[m_IndicesIndex++] = m_VertIndex - 4;
	m_Indices[m_IndicesIndex++] = m_VertIndex - 3;
	m_Indices[m_IndicesIndex++] = m_VertIndex - 2;
	m_Indices[m_IndicesIndex++] = m_VertIndex - 4;
	m_Indices[m_IndicesIndex++] = m_VertIndex - 2;
	m_Indices[m_IndicesIndex++] = m_VertIndex - 1;
}

// Helper function to add a face to the bottom side of the current voxel
void Chunk::AddBottomFace(glm::ivec3& pos) {
	m_Vertices[m_VertIndex++] = { glm::vec3(pos), { 1.0f, 0.0f } };
	m_Vertices[m_VertIndex++] = { glm::vec3(pos.x + 1, pos.y, pos.z), { 1.0f, 1.0f } };
	m_Vertices[m_VertIndex++] = { glm::vec3(pos.x + 1, pos.y, pos.z + 1), { 0.0f, 1.0f } };
	m_Vertices[m_VertIndex++] = { glm::vec3(pos.x, pos.y, pos.z + 1), { 0.0f, 0.0f } };
	// Add the indices for the vertices of the bottom face
	m_Indices[m_IndicesIndex++] = m_VertIndex - 4;
	m_Indices[m_IndicesIndex++] = m_VertIndex - 3;
	m_Indices[m_IndicesIndex++] = m_VertIndex - 2;
	m_Indices[m_IndicesIndex++] = m_VertIndex - 4;
	m_Indices[m_IndicesIndex++] = m_VertIndex - 2;
	m_Indices[m_IndicesIndex++] = m_VertIndex - 1;
}

// Helper function to add a face to the top side of the current voxel
void Chunk::AddTopFace(glm::ivec3& pos) {
	m_Vertices[m_VertIndex++] = { glm::vec3(pos.x , pos.y + 1, pos.z), { 0.0f, 0.0f } };
	m_Vertices[m_VertIndex++] = { glm::vec3(pos.x , pos.y + 1, pos.z + 1), { 1.0f, 0.0f } };
	m_Vertices[m_VertIndex++] = { glm::vec3(pos.x + 1, pos.y + 1, pos.z + 1), { 1.0f, 1.0f } };
	m_Vertices[m_VertIndex++] = { glm::vec3(pos.x + 1, pos.y + 1, pos.z), { 0.0f, 1.0f } };
	// Add the indices for the vertices of the top face
	m_Indices[m_IndicesIndex++] = m_VertIndex - 4;
	m_Indices[m_IndicesIndex++] = m_VertIndex - 3;
	m_Indices[m_IndicesIndex++] = m_VertIndex - 2;
	m_Indices[m_IndicesIndex++] = m_VertIndex - 4;
	m_Indices[m_IndicesIndex++] = m_VertIndex - 2;
	m_Indices[m_IndicesIndex++] = m_VertIndex - 1;
}

// Helper function to add a face to the back side of the current voxel
void Chunk::AddBackFace(glm::ivec3& pos) {
	m_Vertices[m_VertIndex++] = { glm::vec3(pos), { 1.0f, 0.0f } };
	m_Vertices[m_VertIndex++] = { glm::vec3(pos.x ,pos.y + 1, pos.z), { 1.0f, 1.0f } };
	m_Vertices[m_VertIndex++] = { glm::vec3(pos.x + 1, pos.y + 1, pos.z), { 0.0f, 1.0f } };
	m_Vertices[m_VertIndex++] = { glm::vec3(pos.x + 1, pos.y, pos.z), { 0.0f, 0.0f } };
	// Add the indices for the vertices of the back face
	m_Indices[m_IndicesIndex++] = m_VertIndex - 4;
	m_Indices[m_IndicesIndex++] = m_VertIndex - 3;
	m_Indices[m_IndicesIndex++] = m_VertIndex - 2;
	m_Indices[m_IndicesIndex++] = m_VertIndex - 4;
	m_Indices[m_IndicesIndex++] = m_VertIndex - 2;
	m_Indices[m_IndicesIndex++] = m_VertIndex - 1;
}

// Helper function to add a face to the front side of the current voxel
void Chunk::AddFrontFace(glm::ivec3& pos) {
	
	m_Vertices[m_VertIndex++] = { glm::vec3(pos.x + 1, pos.y + 1, pos.z + 1), { 1.0f, 1.0f } };
	m_Vertices[m_VertIndex++] = { glm::vec3(pos.x , pos.y + 1, pos.z + 1), { 0.0f, 1.0f } };
	m_Vertices[m_VertIndex++] = { glm::vec3(pos.x, pos.y, pos.z + 1), { 0.0f, 0.0f } };
	m_Vertices[m_VertIndex++] = { glm::vec3(pos.x + 1, pos.y, pos.z + 1), { 1.0f, 0.0f } };
	
	// Add the indices for the vertices of the front face
	m_Indices[m_IndicesIndex++] = m_VertIndex - 4;
	m_Indices[m_IndicesIndex++] = m_VertIndex - 3;
	m_Indices[m_IndicesIndex++] = m_VertIndex - 2;
	m_Indices[m_IndicesIndex++] = m_VertIndex - 4;
	m_Indices[m_IndicesIndex++] = m_VertIndex - 2;
	m_Indices[m_IndicesIndex++] = m_VertIndex - 1;
}
