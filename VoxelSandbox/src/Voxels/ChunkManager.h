#pragma once
#include "Chunk.h"

class ChunkManager {
public:
	ChunkManager();
	~ChunkManager();
	void Render();

	static const int RENDER_DISTANCE = 1;
	static const int WORLD_HEIGHT = 256;
private:
	Chunk*** m_Chunks;
};
