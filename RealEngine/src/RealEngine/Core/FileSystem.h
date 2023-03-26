#pragma once
#include "RealEngine/Core/Buffer.h"

namespace RealEngine {
	class FileSystem {
	public:
		static Buffer ReadFileBinary(const std::filesystem::path& filepath);
	};
}
