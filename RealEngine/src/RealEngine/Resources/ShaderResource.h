#pragma once
#include "RealEngine/Resources/Resource.h"
#include <filesystem>

namespace RealEngine {
	class ShaderResource : public Resource<ShaderResource> {
	public:
		ShaderResource(const std::filesystem::path& path) : Resource(path) {}
	};
}
