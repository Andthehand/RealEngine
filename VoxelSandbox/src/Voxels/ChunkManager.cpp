#include "ChunkManager.h"

#include <functional>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_access.hpp>

#include "imgui/imgui.h"

ChunkManager::ChunkManager(glm::ivec3& cameraPos) : m_PreviousCameraPos(ClampToNum(cameraPos, Chunk::CHUNK_SIZE)), m_JobQueue(NUM_MAX_THREADS) {
	m_ActiveChunks.reserve((2 * m_RenderDistance + 1) ^ 3);
	Chunk::MemoryPool.reserve((2 * m_RenderDistance + 1) ^ 3);

	//TODO: Move this to a seperate  function?
	//Populate m_ActiveChunks with actaul chunks
	for (int x = -m_RenderDistance; x <= m_RenderDistance; x++) {
		for (int y = -m_RenderDistance; y <= m_RenderDistance; y++) {
			for (int z = -m_RenderDistance; z <= m_RenderDistance; z++) {
				glm::vec3 chunkPos = (ClampToNum(cameraPos, Chunk::CHUNK_SIZE)) - (glm::ivec3(x, y, z) * Chunk::CHUNK_SIZE);
				m_ActiveChunks.insert({ chunkPos, std::make_shared<Chunk>(chunkPos, *this) });
			}
		}
	}
}

ChunkManager::~ChunkManager() {
	m_JobQueue.Stop();
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

glm::ivec3 currentCameraPos;
void ChunkManager::Render(RealEngine::EditorCamera& editorCamera) {
	ResetStatistics();

	currentCameraPos = editorCamera.GetPosition();
	
	glm::ivec3 cameraDist;
	cameraDist.x = std::abs(currentCameraPos.x - m_PreviousCameraPos.x);
	cameraDist.y = std::abs(currentCameraPos.y - m_PreviousCameraPos.y);
	cameraDist.z = std::abs(currentCameraPos.z - m_PreviousCameraPos.z);
	m_Statistics.CameraDist = cameraDist;
	if (cameraDist.x >= Chunk::CHUNK_SIZE || 
		cameraDist.y >= Chunk::CHUNK_SIZE || 
		cameraDist.z >= Chunk::CHUNK_SIZE) {
		m_PreviousCameraPos = ClampToNum(currentCameraPos, Chunk::CHUNK_SIZE);
		//m_JobQueue.Push(std::bind(&ChunkManager::UpdateChunks, this));
		UpdateChunks();
	}

	//This is used for the frustum culling
	if(!m_FrustumFrozen) ExtractFrustum(m_FrustumPlanes, editorCamera.GetViewProjection());
	for (auto& [pos, chunk] : m_ActiveChunks) {
		//Calcualte the boudning box of the chunk
		glm::vec3 min = (glm::vec3)pos;
		glm::vec3 max = (glm::vec3)pos + glm::vec3(Chunk::CHUNK_SIZE);
		
		switch (chunk->m_Status) {
			case Chunk::Status::Load:
				chunk->m_Status = Chunk::Status::Proccessing;
				m_JobQueue.Push(std::bind(&Chunk::LoadVoxels, chunk.get()));
				break;
			
			case Chunk::Status::UpdateMesh:
				//Setting the Status to Proccessing so that it doesn't get added to the JobQueue agains
				chunk->m_Status = Chunk::Status::Proccessing;
				m_JobQueue.Push(std::bind(&Chunk::CreateMesh, chunk.get()));
				break;
			
			case Chunk::Status::UploadBuffers:
				chunk->CreateBuffers();
				break;

			case Chunk::Status::Renderable:
				//This is the frustum culling
				if(IntersectFrustum(m_FrustumPlanes, min, max))
				//TODO: Put this into a display list
				chunk->Render();
				m_Statistics.ChunksRendered++;
				break;
		}
	}
}

void ChunkManager::OnImGuiRender() {
	ImGui::Text("Previous Camera Pos: %i, %i, %i", m_PreviousCameraPos.x, m_PreviousCameraPos.y, m_PreviousCameraPos.z);
	glm::ivec3 temp = Vec3ToChunkCords(m_PreviousCameraPos);
	ImGui::Text("Previous Chunk Pos: %i, %i, %i", temp.x, temp.y, temp.z);
	ImGui::Text("Distance: %i, %i, %i", m_Statistics.CameraDist.x, m_Statistics.CameraDist.y, m_Statistics.CameraDist.z);
	ImGui::Text("Num Chunks Rendered %i", m_Statistics.ChunksRendered);
	ImGui::Text("Num Chunks %i", m_ActiveChunks.size());
	if (ImGui::SliderInt("Render Distance", &m_RenderDistance, 1, 20)) {
		for (auto& [key, chunk] : m_ActiveChunks) {
			chunk->m_Status = Chunk::Status::UpdateMesh;
		}
		m_JobQueue.Clear();
		//m_JobQueue.Push(std::bind(&ChunkManager::UpdateChunks, this));
		UpdateChunks();
	}
	if (ImGui::Button("Freeze Frustum")) m_FrustumFrozen = !m_FrustumFrozen;
	if (ImGui::Button("Add Job")) m_JobQueue.Push([] { RE_INFO("Hello Thread!"); });
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
void ChunkManager::UpdateChunks() {
	static std::vector<glm::ivec3> tempCords;
	for (auto& chunk : m_ActiveChunks) {
		glm::ivec3 chunkCameraDist;
		chunkCameraDist.x = std::abs(m_PreviousCameraPos.x - chunk.first.x);
		chunkCameraDist.y = std::abs(m_PreviousCameraPos.y - chunk.first.y);
		chunkCameraDist.z = std::abs(m_PreviousCameraPos.z - chunk.first.z);

		//Is the current chunk within the render distance
		if (chunkCameraDist.x > Chunk::CHUNK_SIZE * m_RenderDistance ||
			chunkCameraDist.y > Chunk::CHUNK_SIZE * m_RenderDistance ||
			chunkCameraDist.z > Chunk::CHUNK_SIZE * m_RenderDistance) {
			Chunk::MemoryPool.push_back(chunk.second);
			tempCords.push_back(chunk.first);
		}
	}

	//Can't figure out how to erase inside of the top for each loop
	for (auto& cords : tempCords) {
		m_ActiveChunks.erase(cords);
	}

	for(auto& cords : tempCords) {
		//This bool is because a deleted chunk shouldn't have more than one neighbor maybe
		for (int i = -1; i < 2; i += 2) {
			std::shared_ptr<Chunk> chunk = GetChunk({ cords.x + (Chunk::CHUNK_SIZE * i), cords.y, cords.z });
			if (chunk != nullptr) {
				chunk->m_Status = Chunk::Status::UpdateMesh;
			}

			chunk = GetChunk({ cords.x, cords.y + (Chunk::CHUNK_SIZE * i), cords.z });
			if (chunk != nullptr) {
				chunk->m_Status = Chunk::Status::UpdateMesh;
			}

			chunk = GetChunk({ cords.x, cords.y, cords.z + (Chunk::CHUNK_SIZE * i) });
			if (chunk != nullptr) {
				chunk->m_Status = Chunk::Status::UpdateMesh;
			}
		}
	}
	tempCords.clear();

	//TODO: Do this on another thread
	for (int x = -m_RenderDistance; x <= m_RenderDistance; x++) {
		for (int y = -m_RenderDistance; y <= m_RenderDistance; y++) {
			for (int z = -m_RenderDistance; z <= m_RenderDistance; z++) {
				glm::ivec3 newChunkPos = m_PreviousCameraPos - (glm::ivec3(x, y, z) * Chunk::CHUNK_SIZE);
				//Check if the chunkpos is already in the unordered_map
				if (m_ActiveChunks.find(newChunkPos) == m_ActiveChunks.end()) {
					// See if the MemoryPool already has a chunk or else loads a new one
					if (Chunk::MemoryPool.empty()) {
						//m_JobQueue.Push([&] {m_ActiveChunks.insert({ newChunkPos, std::make_shared<Chunk>(newChunkPos, *this) }); });
						m_ActiveChunks.insert({ newChunkPos, std::make_shared<Chunk>(newChunkPos, *this) });
					}
					else {
						m_ActiveChunks.insert({ newChunkPos, Chunk::MemoryPool.back()}).first->second->SetPostition(newChunkPos);
						Chunk::MemoryPool.pop_back();

						tempCords.push_back(newChunkPos);
					}
				}
			}
		}
	}

	//TODO: Make this way better
	for (auto& cords : tempCords) {
		for (int i = -1; i < 2; i += 2) {
			std::shared_ptr<Chunk> chunk = GetChunk({ cords.x + (Chunk::CHUNK_SIZE * i), cords.y, cords.z });
			if (chunk != nullptr) {
				chunk->m_Status = Chunk::Status::UpdateMesh;
			}

			chunk = GetChunk({ cords.x, cords.y + (Chunk::CHUNK_SIZE * i), cords.z });
			if (chunk != nullptr) {
				chunk->m_Status = Chunk::Status::UpdateMesh;
			}

			chunk = GetChunk({ cords.x, cords.y, cords.z + (Chunk::CHUNK_SIZE * i) });
			if (chunk != nullptr) {
				chunk->m_Status = Chunk::Status::UpdateMesh;
			}
		}
	}

	for (auto& cords : tempCords) {
		std::shared_ptr<Chunk> chunk = GetChunk(cords);
		chunk->m_Status = Chunk::Status::Load;
	}

	tempCords.clear();
}
