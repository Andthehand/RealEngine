#pragma once
#include <RealEngine.h>

class ScopeTimer
{
public:
	ScopeTimer(std::string func) : m_Funtion(func) {
		m_Start = std::chrono::high_resolution_clock::now();
	}

	~ScopeTimer() {
		auto stop = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - m_Start);
		RE_INFO("The Function {0} took: {1}us", m_Funtion, duration.count());
	}

private:
	std::string m_Funtion;
	std::chrono::steady_clock::time_point m_Start;
};

#define VOXEL_TIMER() ScopeTimer timer(__FUNCTION__);
