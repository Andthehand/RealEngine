#include "Voxel.h"

RealEngine::Ref<RealEngine::TextureBuffer> Voxel::s_TextureBuffer;

void Voxel::UploadTextureCords() {
	std::array<float, 10> texID;

	//Dirt
	texID[Grass_Cords_Side] =	0.0f;
	texID[Grass_Cords_Top] =	1.0f;
	texID[Grass_Cords_Bottom] =	2.0f;
	texID[Sand_Cords] =	4.0f;

	Voxel::s_TextureBuffer = RealEngine::TextureBuffer::Create(sizeof(texID), texID.data(), RealEngine::BufferUsage::StaticDraw, RealEngine::ColorFormat::R32F);
	Voxel::s_TextureBuffer->Bind();
}
