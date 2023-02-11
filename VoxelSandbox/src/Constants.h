#pragma once
#include <cstdint>

class Constants {
public:
	static const int CHUNK_SIZE = 64;
	//This is the amount of chunks that will be rendered in one direction around the camera
	static const int WORLD_HEIGHT = 256;
	//All of the stuff to do with threads
	static const uint32_t NUM_MAX_THREADS = 1;

	static const uint32_t SEED = 5;
	inline static const float FREQUENCY = 0.005f;
};
