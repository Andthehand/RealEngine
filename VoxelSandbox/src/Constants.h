#pragma once
#include <cstdint>

#include <RealEngine.h>

class Constants {
public:
	static const int CHUNK_SIZE = 64;
	//This is the amount of chunks that will be rendered in one direction around the camera
	static const int WORLD_HEIGHT = CHUNK_SIZE * 4;
	//All of the stuff to do with threads
	static const uint32_t NUM_MAX_THREADS = 8;

	static const uint32_t SEED = 5;
	inline static const float FREQUENCY = 0.01f;
	inline static const float THRESHOLD = 0.60f;

	inline static const glm::vec3 CAMERA_POSITION = { 0.0f, WORLD_HEIGHT / 2, 0.0f };
	inline static const float MOUSE_SENSITIVITY = 0.1f;
	inline static float CAMERA_SPEED = 100.0f;
	inline static const float NEAR_Clip = 0.1f;
	inline static const float FAR_Clip = 1000.0f;

	static const uint32_t JSON_FORMAT_VERSION = 1;
};
