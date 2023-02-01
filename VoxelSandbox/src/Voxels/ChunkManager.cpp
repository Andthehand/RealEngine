#include "ChunkManager.h"

#include <functional>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_access.hpp>

#include "imgui/imgui.h"

#include "Constants.h"

ChunkManager::ChunkManager(const glm::vec3& cameraPos) 
	: m_LastCameraChunkPosition(ToChunkCoords(cameraPos)), m_JobQueue(Constants::NUM_MAX_THREADS) {
	m_ActiveChunks.reserve((2 * m_RenderDistance + 1) ^ 3);
	Chunk::MemoryPool.reserve((2 * m_RenderDistance + 1) ^ 3);

	//m_Texture = RealEngine::Texture2D::Create("assets/textures/awesomeface.png");
	m_Texture = RealEngine::Texture2D::Create("assets/textures/Spritesheet.png");
	Voxel::CreateTextureCords(m_Texture);

	//Populate m_ActiveChunks with actaul chunks
	UpdateChunks();
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

void ChunkManager::Render(const RealEngine::EditorCamera& editorCamera) {
	ResetStatistics(); 

	static glm::vec3 currentCameraPos = { 0, 0, 0 };
	if(!m_FreezePos)
		currentCameraPos = editorCamera.GetPosition();
	
	static glm::vec3 lastPlayerLoadPosition = currentCameraPos;

	m_Statistics.CameraDist = glm::distance2(currentCameraPos, lastPlayerLoadPosition);
	if (m_Statistics.CameraDist > Constants::CHUNK_SIZE * Constants::CHUNK_SIZE) {
		lastPlayerLoadPosition = currentCameraPos;
		m_LastCameraChunkPosition = ToChunkCoords(currentCameraPos);

		//m_JobQueue.Push(std::bind(&ChunkManager::UpdateChunks, this));
		UpdateChunks();
	}

	//This is used for the frustum culling
	if(!m_FrustumFrozen) ExtractFrustum(m_FrustumPlanes, editorCamera.GetViewProjection());
	for (auto& [pos, chunk] : m_ActiveChunks) {		
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
				//Calcualte the boudning box of the chunk
				glm::vec3 min = (glm::vec3)pos;
				glm::vec3 max = (glm::vec3)pos + glm::vec3(Constants::CHUNK_SIZE);

				//Check if the chunk is intersecting
				if (IntersectFrustum(m_FrustumPlanes, min, max)) {
					//TODO: Put this into a display list
					m_Texture->Bind();
					chunk->Render();
					m_Statistics.ChunksRendered++;
				}
				break;
		}
	}
}

void ChunkManager::OnImGuiRender() {
	ImGui::Text("Previous Camera Pos: %i, %i, %i", m_LastCameraChunkPosition.x, m_LastCameraChunkPosition.y, m_LastCameraChunkPosition.z);
	//glm::vec3 temp = Vec3ToChunkCords(m_LastCameraChunkPosition);
	//ImGui::Text("Previous Chunk Pos: %i, %i, %i", temp.x, temp.y, temp.z);
	ImGui::Text("Distance: %f", std::sqrtf(m_Statistics.CameraDist));
	ImGui::Text("Num Chunks Rendered %i", m_Statistics.ChunksRendered);
	ImGui::Text("Num Chunks %i", m_ActiveChunks.size());
	if (ImGui::SliderInt("Render Distance", &m_RenderDistance, 1, 20)) {
		for (auto& [key, chunk] : m_ActiveChunks) {
			chunk->m_Status = Chunk::Status::UpdateMesh;
		}
		UpdateChunks();
	}
	if (ImGui::Button("Regenerate Terrain")) {
		for (auto& [key, chunk] : m_ActiveChunks) {
			chunk->m_Status = Chunk::Status::UpdateMesh;
		}
	}
	if (ImGui::Button("Freeze Frustum")) m_FrustumFrozen = !m_FrustumFrozen;
	if (ImGui::Button("Freeze Position")) m_FreezePos = !m_FreezePos;
}

void ChunkManager::ResetStatistics() {
	m_Statistics.ChunksRendered = 0;
}

//This checks the m_ActiveChunks to discard chunks to far away and add chunks that are in render distance
void ChunkManager::UpdateChunks() {
	//Remove Chunks that are too far away from the camera
	for (auto chunk = m_ActiveChunks.begin(); chunk != m_ActiveChunks.end();) {
		//Covert chunk position to chunk cords
		const glm::ivec3 localChunkPos = ToChunkCoords(chunk->first) - m_LastCameraChunkPosition;
		bool inRangeOfPlayer =
			(localChunkPos.x * localChunkPos.x) + (localChunkPos.y * localChunkPos.y) + (localChunkPos.z * localChunkPos.z) <=
			(m_RenderDistance * m_RenderDistance) + (m_RenderDistance * m_RenderDistance);
		if (!inRangeOfPlayer) {
			Chunk::MemoryPool.push_back(chunk->second);
			chunk = m_ActiveChunks.erase(chunk);
		}
		else
			chunk++;
	}

	for (int y = m_LastCameraChunkPosition.y - m_RenderDistance; y <= m_LastCameraChunkPosition.y + m_RenderDistance; y++) {
		for (int x = m_LastCameraChunkPosition.x - m_RenderDistance; x <= m_LastCameraChunkPosition.x + m_RenderDistance; x++) {
			for (int z = m_LastCameraChunkPosition.z - m_RenderDistance; z <= m_LastCameraChunkPosition.z + m_RenderDistance; z++) {
				glm::ivec3 position(x, y, z);
				glm::ivec3 localPos = m_LastCameraChunkPosition - position;
				if ((localPos.x * localPos.x) + (localPos.y * localPos.y) + (localPos.z * localPos.z) <= (m_RenderDistance * m_RenderDistance)) {
					if (Chunk::MemoryPool.empty()) {
						//m_JobQueue.Push([&] {m_ActiveChunks.insert({ newChunkPos, std::make_shared<Chunk>(newChunkPos, *this) }); });
						std::shared_ptr<Chunk> tempChunk = std::make_shared<Chunk>(position * Constants::CHUNK_SIZE, *this);
						tempChunk->m_Status = Chunk::Status::Load;
						m_ActiveChunks.insert({ position * Constants::CHUNK_SIZE, std::make_shared<Chunk>(position * Constants::CHUNK_SIZE, *this) });
					}
					else {
						std::shared_ptr<Chunk> tempChunk = Chunk::MemoryPool.back();
						tempChunk->SetPostition(position * Constants::CHUNK_SIZE);
						tempChunk->m_Status = Chunk::Status::Load;
						m_ActiveChunks.insert({ position * Constants::CHUNK_SIZE, tempChunk});
						Chunk::MemoryPool.pop_back();
					}
				}
			}
		}
	}
}
