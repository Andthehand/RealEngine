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

uint32_t cube_elements[] = {
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

Test::Test() {
	const uint32_t numPoints = 2;
	glm::vec3 points[numPoints] = {
		{ 0.0f, 0.0f, 0.0f },
		{ 2.0f, 0.0f, 0.0f }
	};

	m_VertexArray = RealEngine::VertexArray::Create();
	m_VertexBuffer = RealEngine::VertexBuffer::Create((float*)points, sizeof(VoxelBuffer) * numPoints);
	m_VertexBuffer->SetLayout({
		{ RealEngine::ShaderDataType::Float3, "a_Position" }
	});
	m_VertexArray->AddVertexBuffer(m_VertexBuffer);
}

void Test::Render() {
	//0 for the mode means GL_POINTS
	RealEngine::RenderCommand::DrawArrays(m_VertexArray, 0, 2);
}
