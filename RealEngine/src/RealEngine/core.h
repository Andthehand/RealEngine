#pragma once

#ifdef RE_PLATFORM_WINDOWS
	#ifdef RE_BUILD_DLL
		#define RealEngine_API __declspec(dllexport)
	#else
		#define RealEngine_API __declspec(dllimport)
	#endif
#else 
	#error RealEngine only supports Windows!
#endif 
