#pragma once
#include <RealEngine.h>

class Test {
public:
	Test();
	~Test() = default;

	void Render(const RealEngine::Ref<RealEngine::Shader> shader);
private:
	RealEngine::Ref<RealEngine::VertexArray> m_VertexArray;
	RealEngine::Ref<RealEngine::TextureBuffer> m_TextureBuffer;
};
