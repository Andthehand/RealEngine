#include "Chunk.h"

struct VoxelBuffer {
	glm::vec3 Position;
};

static const struct VerticesLookup {
	const std::array<glm::vec3, 4> Top = {{ 
		{ 0, 1, 0 },
		{ 1, 1, 0 },
		{ 0, 1, 1 },
		{ 1, 1, 1 } 
	}};
	const std::array<glm::vec3, 4> Bottom = { {
		{ 0, 0, 0 },
		{ 1, 0, 0 },
		{ 0, 0, 1 },
		{ 1, 0, 1 }
	}};
	const std::array<glm::vec3, 4> Left = { {
		{ 0, 1, 1 },
		{ 0, 1, 0 }, 
		{ 0, 0, 1 }, 
		{ 0, 0, 0 } 
	}};
	const std::array<glm::vec3, 4> Right = {{
		{ 1, 1, 1 },
		{ 1, 1, 0 },
		{ 1, 0, 1 },
		{ 1, 0, 0 }
	}};
	const std::array<glm::vec3, 4> Front = {{
		{ 0, 1, 1 },
		{ 1, 1, 1 },
		{ 0, 0, 1 },
		{ 1, 0, 1 }
	}};
	const std::array<glm::vec3, 4> Back = {{
		{ 0, 1, 0 },
		{ 1, 1, 0 },
		{ 0, 0, 0 },
		{ 1, 0, 0 }
	}};
} verticesLookup;

Chunk::~Chunk(){
	for (int i = 0; i < CHUNK_SIZE; ++i) {
		for (int j = 0; j < CHUNK_SIZE; ++j) {
			delete[] m_Voxels[i][j];
		}
		delete[] m_Voxels[i];
	}
	delete[] m_Voxels;
}

void Chunk::Init(glm::vec3 worldOffset) {
	m_WorldOffset = worldOffset;

	m_Voxels = new Voxel * *[CHUNK_SIZE];
	for (int x = 0; x < CHUNK_SIZE; ++x) {
		m_Voxels[x] = new Voxel * [CHUNK_SIZE];
		for (int y = 0; y < CHUNK_SIZE; ++y) {
			m_Voxels[x][y] = new Voxel[CHUNK_SIZE];
			for (int z = 0; z < CHUNK_SIZE; ++z) { // initialize the values to whatever you want the default to be
				//m_Voxels[x][y][z] = Voxel();
			}
		}
	}

	m_VertexArray = RealEngine::VertexArray::Create();
	CreateMesh();
}

void Chunk::UpdateMesh(float dt) {

}

void Chunk::CreateMesh() {
	std::array<glm::vec3, 4> tempVertice;
	for (int x = 0; x < CHUNK_SIZE; x++) {
		for (int y = 0; y < CHUNK_SIZE; y++) {
			for (int z = 0; z < CHUNK_SIZE; z++) {
				if (!m_Voxels[x][y][z].IsActive())
					continue;

				//Front
				for (size_t i = 0; i < 4; i++) {
					tempVertice[i] = glm::vec3(x, y, z) + m_WorldOffset + verticesLookup.Front[i];
				}
				m_Mesh.push_back(tempVertice);

				//Back
				for (size_t i = 0; i < 4; i++) {
					tempVertice[i] = glm::vec3(x, y, z) + m_WorldOffset + verticesLookup.Back[i];
				}
				m_Mesh.push_back(tempVertice);

				//Left
				for (size_t i = 0; i < 4; i++) {
					tempVertice[i] = glm::vec3(x, y, z) + m_WorldOffset + verticesLookup.Left[i];
				}
				m_Mesh.push_back(tempVertice);

				//Right
				for (size_t i = 0; i < 4; i++) {
					tempVertice[i] = glm::vec3(x, y, z) + m_WorldOffset + verticesLookup.Right[i];
				}
				m_Mesh.push_back(tempVertice);

				//Top
				for (size_t i = 0; i < 4; i++) {
					tempVertice[i] = glm::vec3(x, y, z) + m_WorldOffset + verticesLookup.Top[i];
				}
				m_Mesh.push_back(tempVertice);

				//Bottom
				for (size_t i = 0; i < 4; i++) {
					tempVertice[i] = glm::vec3(x, y, z) + m_WorldOffset + verticesLookup.Bottom[i];
				}
				m_Mesh.push_back(tempVertice);
			}
		}
	}

	uint32_t* quadIndices = new uint32_t[m_Mesh.size() * 6];
	uint32_t offset = 0;
	for (uint32_t i = 0; i < m_Mesh.size() * 6; i += 6) {
		quadIndices[i + 0] = offset + 0;
		quadIndices[i + 1] = offset + 1;
		quadIndices[i + 2] = offset + 2;

		quadIndices[i + 3] = offset + 2;
		quadIndices[i + 4] = offset + 3;
		quadIndices[i + 5] = offset + 1;

		offset += 4;
	}

													//I have to cast from a std::array<glm::vec3, 4>* to a float
	m_VertexBuffer = RealEngine::VertexBuffer::Create((float*)std::data(m_Mesh), (sizeof(VoxelBuffer) * 4) * m_Mesh.size());
	m_VertexBuffer->SetLayout({
		{ RealEngine::ShaderDataType::Float3, "a_Position" }
	});
	m_VertexArray->AddVertexBuffer(m_VertexBuffer);

 	RealEngine::Ref<RealEngine::IndexBuffer> indexBuffer = RealEngine::IndexBuffer::Create(quadIndices, m_Mesh.size() * 6);
	delete[] quadIndices;

	m_VertexArray->SetIndexBuffer(indexBuffer);
}

void Chunk::Render() {
	m_VertexArray->Bind();
	RealEngine::RenderCommand::DrawIndexed(m_VertexArray, m_Mesh.size() * 6);
}
