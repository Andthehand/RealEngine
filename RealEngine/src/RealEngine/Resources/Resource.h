#pragma once
#include <unordered_map>
#include <filesystem>
#include <memory>

#include "RealEngine/Core/Core.h"

namespace RealEngine {
	template <typename T>
	class Resource {
	public:
		virtual ~Resource() {
			s_Resources.erase(m_Path);
		}

		std::filesystem::path& GetPath() const { return m_Path; }

		static Ref<T> GetInstance(const std::filesystem::path& path) {
			auto it = s_Resources.find(path);
			if (it != s_Resources.end()) {
				// Resource already exists, return existing instance
				return std::dynamic_pointer_cast<T>(std::shared_ptr<Resource>(it->second));
			}

			// Resource does not exist, create a new instance
			auto instance = CreateRef<T>(path);
			s_Resources[path] = instance;
			return instance;
		}
	protected:
		Resource(const std::filesystem::path& path) : m_Path(path) {}
	private:
		std::filesystem::path m_Path;

		inline static std::unordered_map<std::filesystem::path, std::weak_ptr<Resource>> s_Resources;
	};
}
