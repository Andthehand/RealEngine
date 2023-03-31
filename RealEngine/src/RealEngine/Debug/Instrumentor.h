// Basic instrumentation profiler by The Cherno
#pragma once

#include <optick.h>

//chrome://tracing to open the json files
#define RE_PROFILE 1
//https://www.youtube.com/watch?v=qiD39bB7DvA&list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT&index=56&ab_channel=TheCherno
#if RE_PROFILE
    #define RE_PROFILE_BEGIN_SESSION(...) ::Optick::StartCapture(__VA_ARGS__);
	#define RE_PROFILE_END_SESSION(...) ::Optick::StopCapture(__VA_ARGS__);
	#define RE_PROFILE_SAVE_SESSION(name) ::Optick::SaveCapture(name);

	#define RE_PROFILE_FRAME(FRAME_NAME, ...) static ::Optick::ThreadScope mainThreadScope(FRAME_NAME);		\
										OPTICK_UNUSED(mainThreadScope);									\
										::Optick::EndFrame(__VA_ARGS__);								\
										::Optick::Update();												\
										uint32_t frameNumber = ::Optick::BeginFrame(__VA_ARGS__);		\
										::Optick::Event OPTICK_CONCAT(autogen_event_, __LINE__)(*::Optick::GetFrameDescription(__VA_ARGS__)); \
										OPTICK_TAG("Frame", frameNumber);

	#define RE_PROFILE_SCOPE(...) static ::Optick::EventDescription* OPTICK_CONCAT(autogen_description_, __LINE__) = nullptr; \
							 if (OPTICK_CONCAT(autogen_description_, __LINE__) == nullptr) OPTICK_CONCAT(autogen_description_, __LINE__) = ::Optick::CreateDescription(OPTICK_FUNC, __FILE__, __LINE__, ##__VA_ARGS__); \
							 ::Optick::Event OPTICK_CONCAT(autogen_event_, __LINE__)( *(OPTICK_CONCAT(autogen_description_, __LINE__)) ); 
    #define RE_PROFILE_FUNCTION(...) static ::Optick::EventDescription* OPTICK_CONCAT(autogen_description_, __LINE__) = nullptr; \
							 if (OPTICK_CONCAT(autogen_description_, __LINE__) == nullptr) OPTICK_CONCAT(autogen_description_, __LINE__) = ::Optick::CreateDescription(OPTICK_FUNC, __FILE__, __LINE__, nullptr); \
							 ::Optick::Event OPTICK_CONCAT(autogen_event_, __LINE__)( *(OPTICK_CONCAT(autogen_description_, __LINE__)) ); 
#else
    #define RE_PROFILE_BEGIN_SESSION(...)
    #define RE_PROFILE_END_SESSION(...)
	#define RE_PROFILE_SAVE_SESSION(name)

	#define RE_PROFILE_SCOPE(...)
    #define RE_PROFILE_FUNCTION()
#endif
