#pragma once
#include <cstdint>

#include <RealEngine.h>

class Constants {
public:
	static constexpr int FRAME_TIME_ARRAY_SIZE = 50;

	static constexpr int CHUNK_SIZE = 64;
	//This is the amount of chunks that will be rendered in one direction around the camera
	static constexpr int WORLD_HEIGHT = CHUNK_SIZE * 4;
	//All of the stuff to do with threads
	static constexpr uint32_t NUM_MAX_THREADS = 8;

	static constexpr uint32_t SEED = 5;
	static constexpr float FREQUENCY = 0.01f;
	static constexpr float THRESHOLD = 0.00f;

	static constexpr glm::vec3 CAMERA_POSITION = { 0.0f, 0.0f, 0.0f };
	static constexpr float MOUSE_SENSITIVITY = 0.1f;
	inline static float CAMERA_SPEED = 100.0f;
	static constexpr float NEAR_Clip = 0.1f;
	static constexpr float FAR_Clip = 1000.0f;

	static constexpr uint32_t JSON_FORMAT_VERSION = 1;
};
