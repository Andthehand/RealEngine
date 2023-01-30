#pragma once
#include <RealEngine.h>

class Test {
public:
	Test();
	~Test() = default;

	void Render();
private:
	RealEngine::Ref<RealEngine::VertexArray> m_VertexArray;
};
