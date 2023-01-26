#pragma once
#include <RealEngine.h>

class PerlinTexture {
public:
	PerlinTexture(uint32_t width, uint32_t height);

	void Regenerate(uint32_t width, uint32_t height, float noiseStep);

	inline RealEngine::Ref<RealEngine::Texture2D> GetTexture() { return m_Texture; }
private:
	RealEngine::Ref<RealEngine::Texture2D> m_Texture;
};
