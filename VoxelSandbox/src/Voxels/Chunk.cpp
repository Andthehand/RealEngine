#include "Chunk.h"

struct VoxelBuffer {
	glm::vec3 Position;
};

std::vector<std::shared_ptr<Chunk>> Chunk::MemoryPool;

//TODO: Give the chunk a way to see other chunks
Chunk::Chunk(glm::ivec3 worldOffset) {
	m_Voxels = std::vector<std::vector<std::vector<Voxel>>>(CHUNK_SIZE, 
									 std::vector<std::vector<Voxel>>(CHUNK_SIZE, 
									 std::vector<Voxel>(CHUNK_SIZE)));

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
	m_VertIndex = 0;
	m_IndicesIndex = 0;

	CreateMesh();
	UpdateBuffers();
}

void Chunk::UpdateMesh(float dt) {

}

// Helper function to add a face to the left side of the current voxel
void Chunk::AddLeftFace(int x, int y, int z) {
	m_Vertices[m_VertIndex++] = glm::vec3(x, y, z);
	m_Vertices[m_VertIndex++] = glm::vec3(x, y, z + 1);
	m_Vertices[m_VertIndex++] = glm::vec3(x, y + 1, z + 1);
	m_Vertices[m_VertIndex++] = glm::vec3(x, y + 1, z);
	// Add the indices for the vertices of the left face
	m_Indices[m_IndicesIndex++] = m_VertIndex - 4;
	m_Indices[m_IndicesIndex++] = m_VertIndex - 3;
	m_Indices[m_IndicesIndex++] = m_VertIndex - 2;
	m_Indices[m_IndicesIndex++] = m_VertIndex - 4;
	m_Indices[m_IndicesIndex++] = m_VertIndex - 2;
	m_Indices[m_IndicesIndex++] = m_VertIndex - 1;
}

// Helper function to add a face to the right side of the current voxel
void Chunk::AddRightFace(int x, int y, int z) {
	m_Vertices[m_VertIndex++] = glm::vec3(x + 1, y, z);
	m_Vertices[m_VertIndex++] = glm::vec3(x + 1, y + 1, z);
	m_Vertices[m_VertIndex++] = glm::vec3(x + 1, y + 1, z + 1);
	m_Vertices[m_VertIndex++] = glm::vec3(x + 1, y, z + 1);
	// Add the indices for the vertices of the right face
	m_Indices[m_IndicesIndex++] = m_VertIndex - 4;
	m_Indices[m_IndicesIndex++] = m_VertIndex - 3;
	m_Indices[m_IndicesIndex++] = m_VertIndex - 2;
	m_Indices[m_IndicesIndex++] = m_VertIndex - 4;
	m_Indices[m_IndicesIndex++] = m_VertIndex - 2;
	m_Indices[m_IndicesIndex++] = m_VertIndex - 1;
}

// Helper function to add a face to the bottom side of the current voxel
void Chunk::AddBottomFace(int x, int y, int z) {
	m_Vertices[m_VertIndex++] = glm::vec3(x, y, z);
	m_Vertices[m_VertIndex++] = glm::vec3(x + 1, y, z);
	m_Vertices[m_VertIndex++] = glm::vec3(x + 1, y, z + 1);
	m_Vertices[m_VertIndex++] = glm::vec3(x, y, z + 1);
	// Add the indices for the vertices of the bottom face
	m_Indices[m_IndicesIndex++] = m_VertIndex - 4;
	m_Indices[m_IndicesIndex++] = m_VertIndex - 3;
	m_Indices[m_IndicesIndex++] = m_VertIndex - 2;
	m_Indices[m_IndicesIndex++] = m_VertIndex - 4;
	m_Indices[m_IndicesIndex++] = m_VertIndex - 2;
	m_Indices[m_IndicesIndex++] = m_VertIndex - 1;
}

// Helper function to add a face to the top side of the current voxel
void Chunk::AddTopFace(int x, int y, int z) {
	m_Vertices[m_VertIndex++] = glm::vec3(x, y + 1, z);
	m_Vertices[m_VertIndex++] = glm::vec3(x, y + 1, z + 1);
	m_Vertices[m_VertIndex++] = glm::vec3(x + 1, y + 1, z + 1);
	m_Vertices[m_VertIndex++] = glm::vec3(x + 1, y + 1, z);
	// Add the indices for the vertices of the top face
	m_Indices[m_IndicesIndex++] = m_VertIndex - 4;
	m_Indices[m_IndicesIndex++] = m_VertIndex - 3;
	m_Indices[m_IndicesIndex++] = m_VertIndex - 2;
	m_Indices[m_IndicesIndex++] = m_VertIndex - 4;
	m_Indices[m_IndicesIndex++] = m_VertIndex - 2;
	m_Indices[m_IndicesIndex++] = m_VertIndex - 1;
}

// Helper function to add a face to the back side of the current voxel
void Chunk::AddBackFace(int x, int y, int z) {
	m_Vertices[m_VertIndex++] = glm::vec3(x, y, z);
	m_Vertices[m_VertIndex++] = glm::vec3(x, y + 1, z);
	m_Vertices[m_VertIndex++] = glm::vec3(x + 1, y + 1, z);
	m_Vertices[m_VertIndex++] = glm::vec3(x + 1, y, z);
	// Add the indices for the vertices of the back face
	m_Indices[m_IndicesIndex++] = m_VertIndex - 4;
	m_Indices[m_IndicesIndex++] = m_VertIndex - 3;
	m_Indices[m_IndicesIndex++] = m_VertIndex - 2;
	m_Indices[m_IndicesIndex++] = m_VertIndex - 4;
	m_Indices[m_IndicesIndex++] = m_VertIndex - 2;
	m_Indices[m_IndicesIndex++] = m_VertIndex - 1;
}

// Helper function to add a face to the front side of the current voxel
void Chunk::AddFrontFace(int x, int y, int z) {
	m_Vertices[m_VertIndex++] = glm::vec3(x, y, z + 1);
	m_Vertices[m_VertIndex++] = glm::vec3(x + 1, y, z + 1);
	m_Vertices[m_VertIndex++] = glm::vec3(x + 1, y + 1, z + 1);
	m_Vertices[m_VertIndex++] = glm::vec3(x, y + 1, z + 1);
	// Add the indices for the vertices of the front face
	m_Indices[m_IndicesIndex++] = m_VertIndex - 4;
	m_Indices[m_IndicesIndex++] = m_VertIndex - 3;
	m_Indices[m_IndicesIndex++] = m_VertIndex - 2;
	m_Indices[m_IndicesIndex++] = m_VertIndex - 4;
	m_Indices[m_IndicesIndex++] = m_VertIndex - 2;
	m_Indices[m_IndicesIndex++] = m_VertIndex - 1;
}

void Chunk::CreateMesh() {
	// Face culling algorithm
	for (int x = 0; x < CHUNK_SIZE; x++) {
		for (int y = 0; y < CHUNK_SIZE; y++) {
			for (int z = 0; z < CHUNK_SIZE; z++) {
				if (!m_Voxels[x][y][z].IsActive()) continue;

				int tempX = x + m_WorldOffset.x;
				int tempY = y + m_WorldOffset.y;
				int tempZ = z + m_WorldOffset.z;

				// Check for inactive voxels around the current voxel
				if (x == 0 || !m_Voxels[x - 1][y][z].IsActive()) {
					// Add a face to the vertices and indices for the left side
					AddLeftFace(tempX, tempY, tempZ);
				}
				if (x == CHUNK_SIZE - 1 || !m_Voxels[x + 1][y][z].IsActive()) {
					// Add a face to the vertices and indices for the right side
					AddRightFace(tempX, tempY, tempZ);
				}
				if (y == 0 || !m_Voxels[x][y - 1][z].IsActive()) {
					// Add a face to the vertices and indices for the bottom side
					AddBottomFace(tempX, tempY, tempZ);
				}
				if (y == CHUNK_SIZE - 1 || !m_Voxels[x][y + 1][z].IsActive()) {
					// Add a face to the vertices and indices for the top side
					AddTopFace(tempX, tempY, tempZ);
				}
				if (z == 0 || !m_Voxels[x][y][z - 1].IsActive()) {
					// Add a face to the vertices and indices for the back side
					AddBackFace(tempX, tempY, tempZ);
				}
				if (z == CHUNK_SIZE - 1 || !m_Voxels[x][y][z + 1].IsActive()) {
					// Add a face to the vertices and indices for the front side
					AddFrontFace(tempX, tempY, tempZ);
				}
			}
		}
	}
}

void Chunk::CreateBuffers() {
	//I have to cast from a std::array<glm::vec3, 4>* to a float
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
