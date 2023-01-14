#include "ChunkManager.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_access.hpp>

ChunkManager::ChunkManager() {
	m_Chunks.reserve(RENDER_DISTANCE * RENDER_DISTANCE * RENDER_DISTANCE);

	for (int x = 0; x < RENDER_DISTANCE; x++) {
		for (int y = 0; y < RENDER_DISTANCE; y++) {
			for (int z = 0; z < RENDER_DISTANCE; z++) {
				glm::vec3 tempChunkPos = { x * Chunk::CHUNK_SIZE, y * Chunk::CHUNK_SIZE, z * Chunk::CHUNK_SIZE };
				m_Chunks.insert({ tempChunkPos, Chunk(tempChunkPos) });
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

void ChunkManager::Render(RealEngine::EditorCamera& editorCamera) {
	ExtractFrustum(frustumPlanes, editorCamera.GetViewProjection());
	
	for each (auto chunk in m_Chunks) {
		glm::vec3 min = glm::vec3(chunk.first);
		glm::vec3 max = glm::vec3(chunk.first + glm::vec3(Chunk::CHUNK_SIZE));
		
		if (IntersectFrustum(frustumPlanes, min, max)) {
			chunk.second.Render();
		}
	}
}
