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
		RE_INFO("The Function {0} took: {1}us ({2}ms)", m_Funtion, duration.count(), duration.count() / 1000.0f);
	}

private:
	std::string m_Funtion;
	std::chrono::steady_clock::time_point m_Start;
};

//#ifdef RE_DEBUG
	#define VOXEL_TIMER() ScopeTimer timer(__FUNCTION__);
//#else
//	#define VOXEL_TIMER();
//#endif

