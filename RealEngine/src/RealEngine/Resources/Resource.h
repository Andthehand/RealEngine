#pragma once
#include <unordered_map>
#include <filesystem>
#include <memory>

#include "RealEngine/Core/Core.h"

namespace RealEngine {
	template <typename T>
	class Resource {
	public:
		virtual ~Resource() = default;

		const std::filesystem::path& GetPath() const { return m_Path; }

		template<typename... Args>
		static Ref<T> GetInstance(const std::filesystem::path& path, Args&&... args) {
			auto it = s_Resources.find(path);
			if (it != s_Resources.end()) {
				if(!it->second.expired())
					// Resource already exists, return existing instance
					return std::dynamic_pointer_cast<T>(it->second.lock());
				else
					s_Resources.erase(it);
			}

			// Resource does not exist, create a new instance
			Ref<T> instance = CreateRef<T>(path, std::forward<Args>(args)...);
			s_Resources[path] = instance;
			return instance;
		}
	protected:
		Resource(const std::filesystem::path& path) : m_Path(path) {}
	private:
		const std::filesystem::path m_Path;

		//For each template type, there is a static map of weak pointers to the resource
		inline static std::unordered_map<std::filesystem::path, std::weak_ptr<Resource>> s_Resources;
	};
}
