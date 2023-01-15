#include "Chunk.h"

#include "ChunkManager.h"

struct VoxelBuffer {
	glm::vec3 Position;
};

std::vector<std::shared_ptr<Chunk>> Chunk::MemoryPool;

Chunk::Chunk(glm::ivec3 worldOffset, ChunkManager& manager) : m_ChunkManager(manager) {
	m_Voxels = std::vector<std::vector<std::vector<Voxel>>>(CHUNK_SIZE, 
									 std::vector<std::vector<Voxel>>(CHUNK_SIZE, 
									 std::vector<Voxel>(CHUNK_SIZE)));

	//I need to make this better
	m_Indices.resize(18432);
	m_Vertices.resize(12288);

	for (int i = 0; i < CHUNK_SIZE; i++) {
		for (int j = 0; j < CHUNK_SIZE; j++) {
			for (int k = 0; k < CHUNK_SIZE; k++) {
				m_Voxels[i][j][k].SetActive(true);
				//if (k % 2 == 0 && j % 2 == 0 && i % 2 == 0) {
				//	m_Voxels[i][j][k].SetActive(true);
				//}
				//else {
				//	m_Voxels[i][j][k].SetActive(false);
				//}
			}
		}
	}
	CreateBuffers();
	ReuseChunk(worldOffset);
}

//This is used for so that I don't have to keep re allocating memory for new Chunks
void Chunk::ReuseChunk(glm::ivec3 worldOffset) {
	//Init variables
	m_WorldOffset = worldOffset;

	//Generate new Voxels
}

void Chunk::UpdateMesh(float dt) {

}

void Chunk::CreateMesh() {
	m_VertIndex = 0;
	m_IndicesIndex = 0;

	// Face culling algorithm
	std::shared_ptr<Chunk> neighborChunks[6] = {
		m_ChunkManager.GetChunk({ m_WorldOffset.x - CHUNK_SIZE, m_WorldOffset.y, m_WorldOffset.z }),
		m_ChunkManager.GetChunk({ m_WorldOffset.x + CHUNK_SIZE, m_WorldOffset.y, m_WorldOffset.z }),
		m_ChunkManager.GetChunk({ m_WorldOffset.x, m_WorldOffset.y - CHUNK_SIZE, m_WorldOffset.z }),
		m_ChunkManager.GetChunk({ m_WorldOffset.x, m_WorldOffset.y + CHUNK_SIZE, m_WorldOffset.z }),
		m_ChunkManager.GetChunk({ m_WorldOffset.x, m_WorldOffset.y, m_WorldOffset.z - CHUNK_SIZE }),
		m_ChunkManager.GetChunk({ m_WorldOffset.x, m_WorldOffset.y, m_WorldOffset.z + CHUNK_SIZE })
	};
	
	for (int x = 0; x < CHUNK_SIZE; x++) {
		for (int y = 0; y < CHUNK_SIZE; y++) {
			for (int z = 0; z < CHUNK_SIZE; z++) {
				if (!m_Voxels[x][y][z].IsActive()) continue;

				glm::ivec3 tempCords = { x + m_WorldOffset.x, y + m_WorldOffset.y, z + m_WorldOffset.z };

				// Check for inactive voxels around the current voxel
				if (x == 0) {
					if (neighborChunks[0] == nullptr || !neighborChunks[0]->GetVoxel({ 15, y, z }).IsActive()) {
						AddLeftFace(tempCords);
					}
				}
				else if (!m_Voxels[x - 1][y][z].IsActive()) {
					// Add a face to the vertices and indices for the left side
					AddLeftFace(tempCords);
				}

				if (x == CHUNK_SIZE - 1) {
					if (neighborChunks[1] == nullptr || !neighborChunks[1]->GetVoxel({ 15, y, z }).IsActive()) {
						AddRightFace(tempCords);
					}
				}
				else if (!m_Voxels[x + 1][y][z].IsActive()) {
					// Add a face to the vertices and indices for the right side
					AddRightFace(tempCords);
				}

				if (y == 0) {
					if (neighborChunks[2] == nullptr || !neighborChunks[2]->GetVoxel({ 15, y, z }).IsActive()) {
						AddBottomFace(tempCords);
					}
				}
				else if (!m_Voxels[x][y - 1][z].IsActive()) {
					// Add a face to the vertices and indices for the bottom side
					AddBottomFace(tempCords);
				}

				if (y == CHUNK_SIZE - 1) {
					if (neighborChunks[3] == nullptr || !neighborChunks[3]->GetVoxel({ 15, y, z }).IsActive()) {
						AddTopFace(tempCords);
					}
				}
				else if (!m_Voxels[x][y + 1][z].IsActive()) {
					// Add a face to the vertices and indices for the top side
					AddTopFace(tempCords);
				}

				if (z == 0) {
					if (neighborChunks[4] == nullptr || !neighborChunks[4]->GetVoxel({ 15, y, z }).IsActive()) {
						AddBackFace(tempCords);
					}
				}
				else if (!m_Voxels[x][y][z - 1].IsActive()) {
					// Add a face to the vertices and indices for the back side
					AddBackFace(tempCords);
				}
				
				if (z == CHUNK_SIZE - 1) {
					if (neighborChunks[5] == nullptr || !neighborChunks[5]->GetVoxel({ 15, y, z }).IsActive()) {
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

	//Update the data that's in the buffers
	UpdateBuffers();
}

void Chunk::CreateBuffers() {
	m_VertexArray = RealEngine::VertexArray::Create();
	m_VertexBuffer = RealEngine::VertexBuffer::Create(nullptr, sizeof(VoxelBuffer) * (uint32_t)m_Vertices.size());
	m_VertexBuffer->SetLayout({
		{ RealEngine::ShaderDataType::Float3, "a_Position" }
	});
	m_VertexArray->AddVertexBuffer(m_VertexBuffer);

	RealEngine::Ref<RealEngine::IndexBuffer> indexBuffer = RealEngine::IndexBuffer::Create(nullptr, (uint32_t)m_Indices.size());

	m_VertexArray->SetIndexBuffer(indexBuffer);
}

void Chunk::UpdateBuffers() {
	m_VertexArray->GetVertexBuffers()[0]->SetData((void*)std::data(m_Vertices), sizeof(glm::vec3) * m_VertIndex);
	m_VertexArray->GetIndexBuffer()->SetData(std::data(m_Indices), m_IndicesIndex);
}

void Chunk::Render() {
	m_VertexArray->Bind();
	RealEngine::RenderCommand::DrawIndexed(m_VertexArray, m_IndicesIndex);
}

// Helper function to add a face to the left side of the current voxel
void Chunk::AddLeftFace(glm::ivec3& pos) {
	m_Vertices[m_VertIndex++] = pos;
	m_Vertices[m_VertIndex++] = glm::vec3(pos.x,pos. y, pos.z + 1);
	m_Vertices[m_VertIndex++] = glm::vec3(pos.x ,pos. y + 1, pos.z + 1);;
	m_Vertices[m_VertIndex++] = glm::vec3(pos.x ,pos. y + 1, pos.z);;
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
	m_Vertices[m_VertIndex++] = glm::vec3(pos.x + 1,pos. y, pos.z);
	m_Vertices[m_VertIndex++] = glm::vec3(pos.x + 1,pos. y + 1, pos.z);;
	m_Vertices[m_VertIndex++] = glm::vec3(pos.x  + 1,pos. y + 1, pos.z + 1);;
	m_Vertices[m_VertIndex++] = glm::vec3(pos.x + 1,pos. y, pos.z + 1);
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
	m_Vertices[m_VertIndex++] = pos;
	m_Vertices[m_VertIndex++] = glm::vec3(pos.x + 1,pos. y, pos.z);
	m_Vertices[m_VertIndex++] = glm::vec3(pos.x + 1,pos. y, pos.z + 1);
	m_Vertices[m_VertIndex++] = glm::vec3(pos.x,pos. y, pos.z + 1);
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
	m_Vertices[m_VertIndex++] = glm::vec3(pos.x ,pos. y + 1, pos.z);;
	m_Vertices[m_VertIndex++] = glm::vec3(pos.x ,pos. y + 1, pos.z + 1);;
	m_Vertices[m_VertIndex++] = glm::vec3(pos.x  + 1,pos. y + 1, pos.z + 1);;
	m_Vertices[m_VertIndex++] = glm::vec3(pos.x + 1,pos. y + 1, pos.z);;
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
	m_Vertices[m_VertIndex++] = pos;
	m_Vertices[m_VertIndex++] = glm::vec3(pos.x ,pos. y + 1, pos.z);;
	m_Vertices[m_VertIndex++] = glm::vec3(pos.x + 1,pos. y + 1, pos.z);;
	m_Vertices[m_VertIndex++] = glm::vec3(pos.x + 1,pos. y, pos.z);
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
	m_Vertices[m_VertIndex++] = glm::vec3(pos.x,pos. y, pos.z + 1);
	m_Vertices[m_VertIndex++] = glm::vec3(pos.x + 1,pos. y, pos.z + 1);
	m_Vertices[m_VertIndex++] = glm::vec3(pos.x  + 1,pos. y + 1, pos.z + 1);;
	m_Vertices[m_VertIndex++] = glm::vec3(pos.x ,pos. y + 1, pos.z + 1);;
	// Add the indices for the vertices of the front face
	m_Indices[m_IndicesIndex++] = m_VertIndex - 4;
	m_Indices[m_IndicesIndex++] = m_VertIndex - 3;
	m_Indices[m_IndicesIndex++] = m_VertIndex - 2;
	m_Indices[m_IndicesIndex++] = m_VertIndex - 4;
	m_Indices[m_IndicesIndex++] = m_VertIndex - 2;
	m_Indices[m_IndicesIndex++] = m_VertIndex - 1;
}
