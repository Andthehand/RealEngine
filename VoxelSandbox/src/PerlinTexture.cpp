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
	static FastNoise::SmartNode<> m_PerlinNoise = FastNoise::NewFromEncodedNodeTree("FwAAAIC/AACAPwAAAAAAAIA/BwA=");
	m_Texture = RealEngine::Texture2D::Create(width, height);
	
	float* noiseOutput = new float[height * width];

	m_PerlinNoise->GenUniformGrid2D(noiseOutput, 0, 0, width, height, noiseStep, Constants::SEED);

	Color* color = new Color[width * height];
	for (uint32_t x = 0; x < width; x++) {
		for (uint32_t z = 0; z < height; z++) {
			uint32_t index = x + (width * z);
			color[index].r = color[index].g = color[index].b = uint8_t(0xff * noiseOutput[x + width * z]);
		}
	}
	m_Texture->SetData(color, sizeof(Color) * (width * height));
	delete[] color;
	delete[] noiseOutput;
}
