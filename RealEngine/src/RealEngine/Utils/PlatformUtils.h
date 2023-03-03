#pragma once
#include <string>

namespace RealEngine {
	class FileDialogs {
	public:
		//Returns empty string when canceled
		static std::string OpenFile(const char* filter);
		static std::string SaveFile(const char* filter);
	};
}
