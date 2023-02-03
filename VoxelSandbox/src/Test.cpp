#include "Test.h"

struct VoxelBuffer {
	glm::vec3 Position;
};

glm::vec3 cube_vertices[] = {
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

const uint32_t numPoints = 5;

Test::Test() {
	glm::vec3 points[numPoints] = {
		{ 0.0f, 0.0f, 0.0f },
		{ 5.0f, 0.0f, 0.0f },
		{ 5.0f, 5.0f, 0.0f },
		{ 5.0f, 5.0f, 5.0f },
		{ 5.0f, 0.0f, 5.0f }
	};

	RealEngine::Ref<RealEngine::VertexBuffer> vertexBuffer, instanceBuffer;

	m_VertexArray = RealEngine::VertexArray::Create();
	
	vertexBuffer = RealEngine::VertexBuffer::Create((float*)cube_vertices, sizeof(VoxelBuffer) * 8);
	vertexBuffer->SetLayout({
		{ RealEngine::ShaderDataType::Float3, "a_Vert" },
	});
	m_VertexArray->AddVertexBuffer(vertexBuffer);

	instanceBuffer = RealEngine::VertexBuffer::Create((float*)points, sizeof(VoxelBuffer) * numPoints);
	instanceBuffer->SetLayout({
		{ RealEngine::ShaderDataType::Float3, "a_Position", 1 }
	});
	m_VertexArray->AddVertexBuffer(instanceBuffer);

	RealEngine::Ref<RealEngine::IndexBuffer> indexBuffer = RealEngine::IndexBuffer::Create(cube_elements, 36);

	m_VertexArray->SetIndexBuffer(indexBuffer);

	glm::vec2 coords = { 0.0f, 1.0f };
	m_TextureBuffer = RealEngine::TextureBuffer::Create(sizeof(glm::vec2), &coords, RealEngine::BufferUsage::StaticDraw, RealEngine::InternalFormat::R32F);
}

void Test::Render(const RealEngine::Ref<RealEngine::Shader> shader) {
	//0 for the mode means GL_POINTS
	m_TextureBuffer->Bind();
	RealEngine::RenderCommand::DrawIndexedInstanced(m_VertexArray, numPoints, 36);
}
