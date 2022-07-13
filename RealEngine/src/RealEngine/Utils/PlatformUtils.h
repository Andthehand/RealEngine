#pragma once
#include <string>
#include <optional>

namespace RealEngine {
	class FileDialogs {
	public:
		//Returns empty string when canceled
		static std::optional<std::string> OpenFile(const char* filter);
		static std::optional<std::string> SaveFile(const char* filter);
	};
}
