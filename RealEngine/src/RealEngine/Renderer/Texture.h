#pragma once

#include <string>

#include "RealEngine/Core/Core.h"

namespace RealEngine {
	enum class ColorFormat {
		None = 0, R8, R32F, RG32F, RGB32F
	};

	class Texture {
	public:
		virtual ~Texture() = default;
		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual uint32_t GetRendererID() const = 0;

		virtual void SetData(void* data, uint32_t size) = 0;
		
		virtual void Bind(uint32_t slot = 0) const = 0;

		virtual bool operator==(const Texture& other) const = 0;
	};

	class Texture2D : public Texture {
	public:
		static Ref<Texture2D> Create(const uint32_t width, const uint32_t height);
		static Ref<Texture2D> Create(const std::filesystem::path& path);
	};

	class Texture2DArray : public Texture {
	public:
		static Ref<Texture2DArray> Create(uint32_t width, uint32_t height, uint32_t numTextures, uint32_t mipLevels = 1);
		static Ref<Texture2DArray> Create(const std::string* path, uint32_t numTextures, uint32_t mipLevels = 1);
	};
}
