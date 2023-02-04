#include "Voxel.h"

RealEngine::Ref<RealEngine::TextureBuffer> Voxel::s_TextureBuffer;

inline std::array<glm::vec2, 4> CreateTextureCoords(const RealEngine::Ref<RealEngine::Texture2D> texture, const glm::vec2& coords, const glm::vec2& cellSize) {
	glm::vec2 min = { (coords.x * cellSize.x) / texture->GetWidth(), (coords.y * cellSize.y) / texture->GetHeight() };
	glm::vec2 max = { ((coords.x + 1) * cellSize.x) / texture->GetWidth(), ((coords.y + 1) * cellSize.y) / texture->GetHeight() };
	
	std::array<glm::vec2, 4> texCoords = { glm::vec2{ min.x, min.y }, glm::vec2{ max.x, min.y }, glm::vec2{ max.x, max.y }, glm::vec2{ min.x, max.y } };
	return texCoords;
}

void Voxel::UploadTextureCords(RealEngine::Ref<RealEngine::Texture2D> textureAtlas) {
	std::array<std::array<glm::vec2, 4>, 10> Coords;

	//Dirt
	Coords[Grass_Cords_Top] = CreateTextureCoords(textureAtlas,		{ 0, 1 }, { 16, 16 });
	Coords[Grass_Cords_Bottom] = CreateTextureCoords(textureAtlas,	{ 0, 0 }, { 16, 16 });
	Coords[Grass_Cords_Side] = CreateTextureCoords(textureAtlas,	{ 0, 2 }, { 16, 16 });
	Coords[Dirt_Cords] = CreateTextureCoords(textureAtlas,			{ 0, 0 }, { 16, 16 });
	Coords[Water_Cords] = CreateTextureCoords(textureAtlas,			{ 0, 0 }, { 16, 16 });
	Coords[Stone_Cords] = CreateTextureCoords(textureAtlas,			{ 0, 0 }, { 16, 16 });
	Coords[Wood_Cords_Top] = CreateTextureCoords(textureAtlas,		{ 0, 0 }, { 16, 16 });
	Coords[Wood_Cords_Bottom] = CreateTextureCoords(textureAtlas,	{ 0, 0 }, { 16, 16 });
	Coords[Wood_Cords_Side] = CreateTextureCoords(textureAtlas,		{ 0, 0 }, { 16, 16 });
	Coords[Sand_Cords] = CreateTextureCoords(textureAtlas,			{ 0, 0 }, { 16, 16 });

	Voxel::s_TextureBuffer = RealEngine::TextureBuffer::Create(sizeof(Coords), Coords.data()->data(), RealEngine::BufferUsage::StaticDraw, RealEngine::ColorFormat::RG32F);
	Voxel::s_TextureBuffer->Bind();
}
