#include "ChunkManager.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_access.hpp>

#include "imgui/imgui.h"

ChunkManager::ChunkManager(glm::ivec3& cameraPos) : m_PreviousCameraPos(Vec3ToChunkCords((glm::vec3)cameraPos)* glm::vec3(Chunk::CHUNK_SIZE)) {
	m_ActiveChunks.reserve((2 * RENDER_DISTANCE) * (2 * RENDER_DISTANCE) * (2 * RENDER_DISTANCE));

	for (int x = -RENDER_DISTANCE; x < RENDER_DISTANCE; x++) {
		for (int y = -RENDER_DISTANCE; y < RENDER_DISTANCE; y++) {
			for (int z = -RENDER_DISTANCE; z < RENDER_DISTANCE; z++) {
				glm::vec3 chunkPos = (Vec3ToChunkCords((glm::vec3)cameraPos) * glm::vec3(Chunk::CHUNK_SIZE)) - glm::vec3((x * 16), (y * 16), (z * 16));
				m_ActiveChunks.insert({ chunkPos, Chunk(chunkPos) });
			}
		}
	}
}

void ExtractFrustum(glm::vec4* frustumPlanes, const glm::mat4& viewProjectionMatrix) {
	// Extract the right plane
	frustumPlanes[0] = glm::row(viewProjectionMatrix, 3) - glm::row(viewProjectionMatrix, 0);

	// Extract the left plane
	frustumPlanes[1] = glm::row(viewProjectionMatrix, 3) + glm::row(viewProjectionMatrix, 0);

	// Extract the top plane
	frustumPlanes[2] = glm::row(viewProjectionMatrix, 3) - glm::row(viewProjectionMatrix, 1);

	// Extract the bottom plane
	frustumPlanes[3] = glm::row(viewProjectionMatrix, 3) + glm::row(viewProjectionMatrix, 1);

	// Extract the near plane
	frustumPlanes[4] = glm::row(viewProjectionMatrix, 3) + glm::row(viewProjectionMatrix, 2);

	// Extract the far plane
	frustumPlanes[5] = glm::row(viewProjectionMatrix, 3) - glm::row(viewProjectionMatrix, 2);

	for (int i = 0; i < 6; i++) {
		frustumPlanes[i] = glm::normalize(frustumPlanes[i]);
	}
}

bool IntersectFrustum(const glm::vec4* frustumPlanes, const glm::vec3& min, const glm::vec3& max) {
	// Check if each corner of the bounding box is inside the frustum
	for (int i = 0; i < 6; i++) {
		if (glm::dot(frustumPlanes[i], glm::vec4(min, 1.0f)) < 0 &&
			glm::dot(frustumPlanes[i], glm::vec4(max, 1.0f)) < 0 &&
			glm::dot(frustumPlanes[i], glm::vec4(min.x, min.y, max.z, 1.0f)) < 0 &&
			glm::dot(frustumPlanes[i], glm::vec4(min.x, max.y, min.z, 1.0f)) < 0 &&
			glm::dot(frustumPlanes[i], glm::vec4(min.x, max.y, max.z, 1.0f)) < 0 &&
			glm::dot(frustumPlanes[i], glm::vec4(max.x, min.y, min.z, 1.0f)) < 0 &&
			glm::dot(frustumPlanes[i], glm::vec4(max.x, min.y, max.z, 1.0f)) < 0 &&
			glm::dot(frustumPlanes[i], glm::vec4(max.x, max.y, min.z, 1.0f)) < 0) {
			return false;
		}
	}
	return true;
}

glm::ivec3 currentCameraPos;
glm::vec3 distTemp;
void ChunkManager::Render(RealEngine::EditorCamera& editorCamera) {
	currentCameraPos = editorCamera.GetPosition();
	
	distTemp[0] = (float)std::abs(currentCameraPos.x - m_PreviousCameraPos.x);
	distTemp[1] = (float)std::abs(currentCameraPos.y - m_PreviousCameraPos.y);
	distTemp[2] = (float)std::abs(currentCameraPos.z - m_PreviousCameraPos.z);
	if (distTemp[0] >= Chunk::CHUNK_SIZE || 
		distTemp[1] >= Chunk::CHUNK_SIZE || 
		distTemp[2] >= Chunk::CHUNK_SIZE) {
		m_PreviousCameraPos = Vec3ToChunkCords((glm::vec3)currentCameraPos) * glm::vec3(Chunk::CHUNK_SIZE);
		UpdateChunkMap(currentCameraPos);
	}

	ExtractFrustum(frustumPlanes, editorCamera.GetViewProjection());
	
	for (auto& [pos, chunk] : m_ActiveChunks) {
		glm::vec3 min = (glm::vec3)pos;
		glm::vec3 max = (glm::vec3)pos + glm::vec3(Chunk::CHUNK_SIZE);
		
		if (IntersectFrustum(frustumPlanes, min, max)) {
			chunk.Render();
		}
	}
}

void ChunkManager::OnImGuiRender() {
	ImGui::Text("Previous Camera Pos: %f, %f, %f", m_PreviousCameraPos.x, m_PreviousCameraPos.y, m_PreviousCameraPos.z);
	ImGui::Text("Distance: %f, %f, %f", distTemp.x, distTemp.y, distTemp.z);
	ImGui::Text("Num Chunks %i", (uint32_t)m_ActiveChunks.size());
}

inline glm::vec3 ChunkManager::Vec3ToChunkCords(glm::vec3 cords) {
	float x = std::floor(cords.x / Chunk::CHUNK_SIZE);
	float y = std::floor(cords.y / Chunk::CHUNK_SIZE);
	float z = std::floor(cords.z / Chunk::CHUNK_SIZE);
	return glm::vec3(x, y, z);
}

void ChunkManager::UpdateChunkMap(glm::ivec3& cameraPos) {
	for (auto& chunk : m_ActiveChunks) {
		glm::ivec3 distVeryTemp;
		distVeryTemp[0] = std::abs(cameraPos.x - chunk.first.x);
		distVeryTemp[1] = std::abs(cameraPos.y - chunk.first.y);
		distVeryTemp[2] = std::abs(cameraPos.z - chunk.first.z);

		if (distVeryTemp[0] >= Chunk::CHUNK_SIZE * RENDER_DISTANCE || 
			distVeryTemp[1] >= Chunk::CHUNK_SIZE * RENDER_DISTANCE || 
			distVeryTemp[2] >= Chunk::CHUNK_SIZE * RENDER_DISTANCE) {
			m_ChunksToDelete.emplace(std::move(chunk));
			continue;
		}
	}

	//For some reason I can't delete in the for each loop above
	for (auto& chunk : m_ChunksToDelete) {
		m_ActiveChunks.erase(chunk.first);
	}

	for (int x = -RENDER_DISTANCE; x < RENDER_DISTANCE; x++) {
		for (int y = -RENDER_DISTANCE; y < RENDER_DISTANCE; y++) {
			for (int z = -RENDER_DISTANCE; z < RENDER_DISTANCE; z++) {
				glm::vec3 newChunkPos = (glm::vec3)m_PreviousCameraPos - glm::vec3((x * 16), (y * 16), (z * 16));
				if (m_ActiveChunks.find(newChunkPos) == m_ActiveChunks.end()) {
					// Load new chunk and add it to the unordered map
					m_ActiveChunks.insert({ newChunkPos, Chunk(newChunkPos) });
				}
			}
		}
	}
}
