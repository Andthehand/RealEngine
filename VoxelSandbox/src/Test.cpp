#include "Test.h"

struct VoxelBuffer {
	glm::vec3 Position;
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
