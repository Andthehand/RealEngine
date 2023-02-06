#include "PerlinTexture.h"

#include "Voxels/TerrainGenerator.h"
#include "Constants.h"

struct Color {
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a = 0xff;
};

PerlinTexture::PerlinTexture(uint32_t width, uint32_t height) {
	Regenerate(width, height, Constants::FREQUENCY);
}

void PerlinTexture::Regenerate(uint32_t width, uint32_t height, float noiseStep) {
	m_Texture = RealEngine::Texture2D::Create(width, height);
	
	//RealEngine::BasicPerlinNoise<float> noise = TerrainGenerator::GetNoiseObject();

	Color* color = new Color[width * height];
	for (uint32_t x = 0; x < width; x++) {
		for (uint32_t y = 0; y < height; y++) {
			uint32_t index = x + (width * y);
			//color[index].r = color[index].g = color[index].b = uint8_t(0xffff * noise.noise2D_01((float)x * noiseStep, (float)y * noiseStep));
		}
	}
	m_Texture->SetData(color, sizeof(Color) * (width * height));
	delete[] color;
}
