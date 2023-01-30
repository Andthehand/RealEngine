#include "Voxel.h"

Voxel::VoxelTextureCord Voxel::s_VoxelTextureCord;

void Voxel::CreateTextureCords(RealEngine::Ref<RealEngine::Texture2D> textureAtlas) {
	Voxel::s_VoxelTextureCord.Dirt_Cords = RealEngine::SubTexture2D::CreateFromCoords(textureAtlas, { 0, 2 }, { 16, 16 });
	Voxel::s_VoxelTextureCord.Sand_Cords = RealEngine::SubTexture2D::CreateFromCoords(textureAtlas, { 2, 0 }, { 16, 16 });
	Voxel::s_VoxelTextureCord.Stone_Cords = RealEngine::SubTexture2D::CreateFromCoords(textureAtlas, { 3, 0 }, { 16, 16 });
	Voxel::s_VoxelTextureCord.Water_Cords = RealEngine::SubTexture2D::CreateFromCoords(textureAtlas, { 4, 0 }, { 16, 16 });

	Voxel::s_VoxelTextureCord.Grass_Cords[VoxelSide::Side] =	RealEngine::SubTexture2D::CreateFromCoords(textureAtlas, { 0, 2 }, { 16, 16 });
	Voxel::s_VoxelTextureCord.Grass_Cords[VoxelSide::Top] =		RealEngine::SubTexture2D::CreateFromCoords(textureAtlas, { 0, 1 }, { 16, 16 });
	Voxel::s_VoxelTextureCord.Grass_Cords[VoxelSide::Bottom] =	RealEngine::SubTexture2D::CreateFromCoords(textureAtlas, { 0, 0 }, { 16, 16 });

	Voxel::s_VoxelTextureCord.Wood_Cords[VoxelSide::Side] =		RealEngine::SubTexture2D::CreateFromCoords(textureAtlas, { 5, 0 }, { 16, 16 });
	Voxel::s_VoxelTextureCord.Wood_Cords[VoxelSide::Top] =		RealEngine::SubTexture2D::CreateFromCoords(textureAtlas, { 5, 0 }, { 16, 16 });
	Voxel::s_VoxelTextureCord.Wood_Cords[VoxelSide::Bottom] =	RealEngine::SubTexture2D::CreateFromCoords(textureAtlas, { 5, 0 }, { 16, 16 });
}
