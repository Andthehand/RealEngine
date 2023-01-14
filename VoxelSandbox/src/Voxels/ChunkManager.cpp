#include "ChunkManager.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_access.hpp>

#include "imgui/imgui.h"

ChunkManager::ChunkManager(glm::ivec3& cameraPos) : m_PreviousCameraPos(ClampToNum(cameraPos, Chunk::CHUNK_SIZE)) {
	m_ActiveChunks.reserve((2 * RENDER_DISTANCE) * (2 * RENDER_DISTANCE) * (2 * RENDER_DISTANCE));

	//TODO: Move this to a function?
	//Populate m_ActiveChunks with actaul chunks
	for (int x = -RENDER_DISTANCE; x < RENDER_DISTANCE; x++) {
		for (int y = -RENDER_DISTANCE; y < RENDER_DISTANCE; y++) {
			for (int z = -RENDER_DISTANCE; z < RENDER_DISTANCE; z++) {
				glm::vec3 chunkPos = (ClampToNum(cameraPos, Chunk::CHUNK_SIZE)) - (glm::ivec3(x, y, z) * 16);
				m_ActiveChunks.insert({ chunkPos, std::make_shared<Chunk>(chunkPos) });
			}
		}
	}
}

//This is used to get the frustum from the camera for frustum culling
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

//This is to check if the bounding boxes for the chunks are in the frustum
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
	ResetStatistics();

	glm::ivec3 currentCameraPos	= editorCamera.GetPosition();
	
	glm::ivec3 cameraDist;
	cameraDist.x = std::abs(currentCameraPos.x - m_PreviousCameraPos.x);
	cameraDist.y = std::abs(currentCameraPos.y - m_PreviousCameraPos.y);
	cameraDist.z = std::abs(currentCameraPos.z - m_PreviousCameraPos.z);
	m_Statistics.CameraDist = cameraDist;
	if (cameraDist.x >= Chunk::CHUNK_SIZE || 
		cameraDist.y >= Chunk::CHUNK_SIZE || 
		cameraDist.z >= Chunk::CHUNK_SIZE) {
		m_PreviousCameraPos = ClampToNum(currentCameraPos, Chunk::CHUNK_SIZE);
		UpdateChunkMap(currentCameraPos);
	}

	//This is used for the frustum culling
	ExtractFrustum(frustumPlanes, editorCamera.GetViewProjection());
	for (auto& [pos, chunk] : m_ActiveChunks) {
		//Calcualte the boudning box of the chunk
		glm::vec3 min = (glm::vec3)pos;
		glm::vec3 max = (glm::vec3)pos + glm::vec3(Chunk::CHUNK_SIZE);
		
		//This is the frustum culling
		if (IntersectFrustum(frustumPlanes, min, max)) {
			chunk->Render();
			m_Statistics.ChunksRendered++;
		}
	}
}

void ChunkManager::OnImGuiRender() {
	ImGui::Text("Previous Camera Pos: %f, %f, %f", m_PreviousCameraPos.x, m_PreviousCameraPos.y, m_PreviousCameraPos.z);
	ImGui::Text("Distance: %i, %i, %i", m_Statistics.CameraDist.x, m_Statistics.CameraDist.y, m_Statistics.CameraDist.z);
	ImGui::Text("Num Chunks Rendered %i", m_Statistics.ChunksRendered);
	ImGui::Text("Num Chunks %i", m_ActiveChunks.size());
}

void ChunkManager::ResetStatistics() {
	m_Statistics.ChunksRendered = 0;
}

//Formats { 15, 32, -16 } to { 0, 2, -1 }
inline glm::ivec3 ChunkManager::Vec3ToChunkCords(glm::ivec3 cords) {
	//Floor only returns a float and cast to float so that the divide is a float divide not an int divide
	int x = (int)std::floor((float)cords.x / Chunk::CHUNK_SIZE);
	int y = (int)std::floor((float)cords.y / Chunk::CHUNK_SIZE);
	int z = (int)std::floor((float)cords.z / Chunk::CHUNK_SIZE);
	return glm::ivec3(x, y, z);
}

//This formats the cords to chunk cords then multipiles it by the num
inline glm::ivec3 ChunkManager::ClampToNum(glm::ivec3& cords, int num) {
	return Vec3ToChunkCords(cords) * glm::ivec3(num);
}

//This checks the m_ActiveChunks to discard chunks to far away and add chunks that are in render distance
void ChunkManager::UpdateChunkMap(glm::ivec3& cameraPos) {
	glm::ivec3 chunkCameraDist;
	for (auto& chunk : m_ActiveChunks) {
		chunkCameraDist.x = std::abs(cameraPos.x - chunk.first.x);
		chunkCameraDist.y = std::abs(cameraPos.y - chunk.first.y);
		chunkCameraDist.z = std::abs(cameraPos.z - chunk.first.z);

		//Is the current chunk within the render distance
		if (chunkCameraDist.x >= Chunk::CHUNK_SIZE * RENDER_DISTANCE || 
			chunkCameraDist.y >= Chunk::CHUNK_SIZE * RENDER_DISTANCE || 
			chunkCameraDist.z >= Chunk::CHUNK_SIZE * RENDER_DISTANCE) {
			//If not put in que to delete
			m_ChunksToDelete.emplace(chunk);
			continue;
		}
	}

	//For some reason I can't delete in the for each loop above
	//Which kinda makes sense
	for (auto& chunk : m_ChunksToDelete) {
		m_ActiveChunks.erase(chunk.first);
	}

	//TODO: Do this on another thread
	m_ChunksToDelete.clear();

	//TODO: Do this on another thread
	for (int x = -RENDER_DISTANCE; x < RENDER_DISTANCE; x++) {
		for (int y = -RENDER_DISTANCE; y < RENDER_DISTANCE; y++) {
			for (int z = -RENDER_DISTANCE; z < RENDER_DISTANCE; z++) {
				glm::ivec3 newChunkPos = m_PreviousCameraPos - (glm::ivec3(x, y, z) * 16);
				//Check if the chunkpos is already in the unordered_map
				if (m_ActiveChunks.find(newChunkPos) == m_ActiveChunks.end()) {
					// Load new chunk and add it to the unordered map
					m_ActiveChunks.insert({ newChunkPos, std::make_shared<Chunk>(newChunkPos) });
				}
			}
		}
	}
}
