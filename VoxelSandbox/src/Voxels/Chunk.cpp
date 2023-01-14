#include "Chunk.h"

struct VoxelBuffer {
	glm::vec3 Position;
};

Chunk::~Chunk(){

}

Chunk::Chunk(glm::ivec3 worldOffset) : m_WorldOffset(worldOffset) {
	m_Voxels = std::vector<std::vector<std::vector<Voxel>>>(CHUNK_SIZE, 
									 std::vector<std::vector<Voxel>>(CHUNK_SIZE, 
									 std::vector<Voxel>(CHUNK_SIZE)));

	for (int i = 0; i < CHUNK_SIZE; i++) {
		for (int j = 0; j < CHUNK_SIZE; j++) {
			for (int k = 0; k < CHUNK_SIZE; k++) {
				m_Voxels[i][j][k].SetActive(true);
				/*if (k % 2 == 0 && j % 2 == 0 && i % 2 == 0) {
					m_Voxels[i][j][k].SetActive(true);
				}
				else {
					m_Voxels[i][j][k].SetActive(false);
				}*/
			}
		}
	}

	CreateMesh();
	CreateBuffers();
}

void Chunk::UpdateMesh(float dt) {

}

// Helper function to add a face to the left side of the current voxel
void Chunk::AddLeftFace(int x, int y, int z) {
	m_Vertices.push_back(glm::vec3(x, y, z));
	m_Vertices.push_back(glm::vec3(x, y, z + 1));
	m_Vertices.push_back(glm::vec3(x, y + 1, z + 1));
	m_Vertices.push_back(glm::vec3(x, y + 1, z));
	// Add the indices for the vertices of the left face
	int index = (int)m_Vertices.size() - 4;
	m_Indices.push_back(index);
	m_Indices.push_back(index + 1);
	m_Indices.push_back(index + 2);
	m_Indices.push_back(index);
	m_Indices.push_back(index + 2);
	m_Indices.push_back(index + 3);
}

// Helper function to add a face to the right side of the current voxel
void Chunk::AddRightFace(int x, int y, int z) {
	m_Vertices.push_back(glm::vec3(x + 1, y, z));
	m_Vertices.push_back(glm::vec3(x + 1, y + 1, z));
	m_Vertices.push_back(glm::vec3(x + 1, y + 1, z + 1));
	m_Vertices.push_back(glm::vec3(x + 1, y, z + 1));
	// Add the indices for the vertices of the right face
	int index = (int)m_Vertices.size() - 4;
	m_Indices.push_back(index);
	m_Indices.push_back(index + 1);
	m_Indices.push_back(index + 2);
	m_Indices.push_back(index);
	m_Indices.push_back(index + 2);
	m_Indices.push_back(index + 3);
}

// Helper function to add a face to the bottom side of the current voxel
void Chunk::AddBottomFace(int x, int y, int z) {
	m_Vertices.push_back(glm::vec3(x, y, z));
	m_Vertices.push_back(glm::vec3(x + 1, y, z));
	m_Vertices.push_back(glm::vec3(x + 1, y, z + 1));
	m_Vertices.push_back(glm::vec3(x, y, z + 1));
	// Add the indices for the vertices of the bottom face
	int index = (int)m_Vertices.size() - 4;
	m_Indices.push_back(index);
	m_Indices.push_back(index + 1);
	m_Indices.push_back(index + 2);
	m_Indices.push_back(index);
	m_Indices.push_back(index + 2);
	m_Indices.push_back(index + 3);
}

// Helper function to add a face to the top side of the current voxel
void Chunk::AddTopFace(int x, int y, int z) {
	m_Vertices.push_back(glm::vec3(x, y + 1, z));
	m_Vertices.push_back(glm::vec3(x, y + 1, z + 1));
	m_Vertices.push_back(glm::vec3(x + 1, y + 1, z + 1));
	m_Vertices.push_back(glm::vec3(x + 1, y + 1, z));
	// Add the indices for the vertices of the top face
	int index = (int)m_Vertices.size() - 4;
	m_Indices.push_back(index);
	m_Indices.push_back(index + 1);
	m_Indices.push_back(index + 2);
	m_Indices.push_back(index);
	m_Indices.push_back(index + 2);
	m_Indices.push_back(index + 3);
}

// Helper function to add a face to the back side of the current voxel
void Chunk::AddBackFace(int x, int y, int z) {
	m_Vertices.push_back(glm::vec3(x, y, z));
	m_Vertices.push_back(glm::vec3(x, y + 1, z));
	m_Vertices.push_back(glm::vec3(x + 1, y + 1, z));
	m_Vertices.push_back(glm::vec3(x + 1, y, z));
	// Add the indices for the vertices of the back face
	int index = (int)m_Vertices.size() - 4;
	m_Indices.push_back(index);
	m_Indices.push_back(index + 1);
	m_Indices.push_back(index + 2);
	m_Indices.push_back(index);
	m_Indices.push_back(index + 2);
	m_Indices.push_back(index + 3);
}

// Helper function to add a face to the front side of the current voxel
void Chunk::AddFrontFace(int x, int y, int z) {
	m_Vertices.push_back(glm::vec3(x, y, z + 1));
	m_Vertices.push_back(glm::vec3(x + 1, y, z + 1));
	m_Vertices.push_back(glm::vec3(x + 1, y + 1, z + 1));
	m_Vertices.push_back(glm::vec3(x, y + 1, z + 1));
	// Add the indices for the vertices of the front face
	int index = (int)m_Vertices.size() - 4;
	m_Indices.push_back(index);
	m_Indices.push_back(index + 1);
	m_Indices.push_back(index + 2);
	m_Indices.push_back(index);
	m_Indices.push_back(index + 2);
	m_Indices.push_back(index + 3);
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
	m_VertexBuffer = RealEngine::VertexBuffer::Create((float*)std::data(m_Vertices), sizeof(VoxelBuffer) * (uint32_t)m_Vertices.size());
	m_VertexBuffer->SetLayout({
		{ RealEngine::ShaderDataType::Float3, "a_Position" }
		});
	m_VertexArray->AddVertexBuffer(m_VertexBuffer);

	RealEngine::Ref<RealEngine::IndexBuffer> indexBuffer = RealEngine::IndexBuffer::Create(std::data(m_Indices), (uint32_t)m_Indices.size());

	m_VertexArray->SetIndexBuffer(indexBuffer);
}

void Chunk::Render() {
	m_VertexArray->Bind();
	RealEngine::RenderCommand::DrawIndexed(m_VertexArray, (uint32_t)m_Indices.size());
}
