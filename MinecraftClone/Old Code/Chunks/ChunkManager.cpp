#include "ChunkManager.h"
#include <cmath>

ChunkManager::ChunkManager(int renderDistance)
	: m_Noise(5983), m_RenderDistance(renderDistance) {
	//This doesnt work rn if renderDistance is even
	RE_ASSERT(m_RenderDistance % 2 != 0, "RenderDistance is Even");

	int distanceFromCenter = (int)std::floorf(m_RenderDistance / 2.0f);
	m_Chunks.reserve(renderDistance * renderDistance);
	for (int x = -distanceFromCenter; x <= distanceFromCenter; x++) {
		for (int z = -distanceFromCenter; z <= distanceFromCenter; z++) {
			ChunkCoordinates newChunkCoords = ChunkCoordinates{ x, z };
			m_Chunks.emplace(newChunkCoords, std::make_unique<Chunk>(m_Noise, newChunkCoords));
		}
	}
}


std::vector<std::pair<ChunkCoordinates, ChunkCoordinates>> newChunks;
void ChunkManager::OnUpdate(const glm::vec3& playerPosition) {
	ChunkCoordinates currentChunk = PositionToChunkPosition(glm::vec2(playerPosition.x, playerPosition.z));

	if (currentChunk != m_CurrentChunk) {
		m_CurrentChunk = currentChunk;

		int maxDistance = (int)std::floorf(m_RenderDistance / 2.0f);
		for (auto& [coord, chunk] : m_Chunks) {
			//Check distance to see if it needs to be destroyed
			ChunkCoordinates newChunk = coord;
			if (std::abs(m_CurrentChunk.x - coord.x) > maxDistance) {
				//TODO: Move Chunk Reset to a list to destroy later
				if (coord.x < m_CurrentChunk.x) {
					newChunk.x += m_RenderDistance;
				}
				else if (coord.x > m_CurrentChunk.x) {
					newChunk.x -= m_RenderDistance;
				}
			}

			if (std::abs(m_CurrentChunk.z - coord.z) > maxDistance) {
				if (coord.z < m_CurrentChunk.z) {
					newChunk.z += m_RenderDistance;
				}
				else if (coord.z > m_CurrentChunk.z) {
					newChunk.z -= m_RenderDistance;
				}
			}

			if(newChunk != coord)
				newChunks.push_back(std::make_pair(coord, newChunk));
		}

		for (const auto& newChunk : newChunks) {
			//Change the key
			auto chunkExtract = m_Chunks.extract(newChunk.first);
			RE_ASSERT(chunkExtract, "Failed to extract chunk");
			chunkExtract.key() = newChunk.second;
			bool inserted = m_Chunks.insert(move(chunkExtract)).inserted;
			RE_ASSERT(inserted, "Failed to insert chunk");

			//Modify the pointer
			m_Chunks.find(newChunk.second)->second.reset(new Chunk(m_Noise, newChunk.second));
			//chunk.reset(new Chunk(m_Noise, newChunk.x * MAXWIDTH, newChunk.z * MAXLENGTH));
		}
		newChunks.clear();
	}
}

void ChunkManager::RenderAll(const RealEngine::Camera& camera) {
	ChunkCoordinates currentChunk = PositionToChunkPosition({ camera.GetPosition().x, camera.GetPosition().z });

	for (const auto& [coord, chunk] : m_Chunks) {
		//TODO: Take away +2 and -2
		switch (camera.GetFacingDirection()) {
		case Facing::North:
			if(coord.x + 2 >= currentChunk.x)
				chunk->Render(m_Noise);
			break;
		case Facing::South:
			if (coord.x - 2 <= currentChunk.x)
				chunk->Render(m_Noise);
			break;
		case Facing::East:
			if (coord.z + 2 >= currentChunk.z)
				chunk->Render(m_Noise);
			break;
		case Facing::West:
			if (coord.z - 2 <= currentChunk.z)
				chunk->Render(m_Noise);
			break;
		}
		
		//chunk->Render(m_Noise);
	}
}
