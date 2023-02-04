#include "Test.h"

#define Tex_Coord_ID_BITMASK uint32_t(0x000000FF)
#define UV_ID_BITMASK uint32_t(0x00000300)

struct VoxelBuffer {
	glm::vec3 Vert;
	uint32_t Data;
};

glm::vec3 cube_vertices[8] = {
	// front
	{ -1.0, -1.0,  1.0 },
	{  1.0, -1.0,  1.0 },
	{  1.0,  1.0,  1.0 },
	{ -1.0,  1.0,  1.0 },
	// back
	{ -1.0, -1.0, -1.0 },
	{  1.0, -1.0, -1.0 },
	{  1.0,  1.0, -1.0 },
	{ -1.0,  1.0, -1.0 }
};

uint32_t cube_elements[36] = {
	// front
	0, 1, 2,
	2, 3, 0,
	// right
	1, 5, 6,
	6, 2, 1,
	// back
	7, 6, 5,
	5, 4, 7,
	// left
	4, 0, 3,
	3, 7, 4,
	// bottom
	4, 5, 1,
	1, 0, 4,
	// top
	3, 2, 6,
	6, 7, 3
};

const uint32_t numPoints = 8;

Test::Test() {
	VoxelBuffer points[numPoints] = {
		{ cube_vertices[0], 0 },
		{ cube_vertices[1], 0 },
		{ cube_vertices[2], 1 },
		{ cube_vertices[3], 1 },
		{ cube_vertices[4], 2 },
		{ cube_vertices[5], 2 },
		{ cube_vertices[6], 3 },
		{ cube_vertices[7], 3 }
	};

	RealEngine::Ref<RealEngine::VertexBuffer> vertexBuffer;

	m_VertexArray = RealEngine::VertexArray::Create();
	
	vertexBuffer = RealEngine::VertexBuffer::Create(sizeof(VoxelBuffer) * numPoints);
	vertexBuffer->SetData(points, sizeof(VoxelBuffer) * numPoints);
	vertexBuffer->SetLayout({
		{ RealEngine::ShaderDataType::Float3, "a_Position" },
		{ RealEngine::ShaderDataType::UInt, "a_Data" }
	});
	m_VertexArray->AddVertexBuffer(vertexBuffer);

	RealEngine::Ref<RealEngine::IndexBuffer> indexBuffer = RealEngine::IndexBuffer::Create(cube_elements, 36);

	m_VertexArray->SetIndexBuffer(indexBuffer);

	glm::vec2 coords[4] = {
		{ 0.0f, 1.0f },
		{ 1.0f, 0.0f },
		{ 1.0f, 1.0f },
		{ 0.0f, 0.0f }
	};
	m_TextureBuffer = RealEngine::TextureBuffer::Create(sizeof(glm::vec2) * 2, &coords, RealEngine::BufferUsage::StaticDraw, RealEngine::ColorFormat::RG32F);
	m_TextureBuffer->Bind();
}

void Test::Render(const RealEngine::Ref<RealEngine::Shader> shader) {
	//0 for the mode means GL_POINTS
	//m_TextureBuffer->Bind();
	RealEngine::RenderCommand::DrawIndexed(m_VertexArray, 36);
}
