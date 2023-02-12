#pragma once

#include <glad/glad.h>

#include "RealEngine/Renderer/Texture.h"

namespace RealEngine {
	//------------------------------------
	// Texture2D -------------------------
	//------------------------------------
	class OpenGLTexture2D : public Texture2D{
	public:
		OpenGLTexture2D(const uint32_t width, const uint32_t height);
		OpenGLTexture2D(const std::string& path);
		virtual ~OpenGLTexture2D();

		virtual uint32_t GetWidth() const override { return m_Width; }
		virtual uint32_t GetHeight() const override { return m_Height; }
		virtual uint32_t GetRendererID() const override { return m_RendererID; }

		virtual void SetData(void* data, uint32_t size) override;

		virtual void Bind(uint32_t slot = 0) const override;
	
		virtual bool operator==(const Texture& other) const override {
			return m_RendererID == ((OpenGLTexture2D&)other).m_RendererID;
		}
	private:
		std::string m_Path;
		uint32_t m_Width, m_Height;

		uint32_t m_RendererID;
		GLenum m_InternalFormat, m_DataFormat;
	};

	//------------------------------------
	// Texture2DArray --------------------
	//------------------------------------
	struct TextureData {
		int Width = 0;
		int Height = 0;
		int Channels = 0;
	};

	class OpenGLTexture2DArray : public Texture2DArray {
	public:
		//Not Tested
		OpenGLTexture2DArray(uint32_t width, uint32_t height, uint32_t numTextures, uint32_t mipLevels = 1);
		OpenGLTexture2DArray(const std::string* path, uint32_t numTextures, uint32_t mipLevels = 1);
		virtual ~OpenGLTexture2DArray();

		virtual uint32_t GetWidth() const override { return m_Width; }
		virtual uint32_t GetHeight() const override { return m_Height; }
		virtual uint32_t GetRendererID() const override { return m_RendererID; }

		virtual void SetData(void* data, uint32_t size) override;

		virtual void Bind(uint32_t slot = 0) const override;

		virtual bool operator==(const Texture& other) const override {
			return m_RendererID == ((OpenGLTexture2DArray&)other).m_RendererID;
		}
	private:
		std::string m_Path;
		uint32_t m_Width, m_Height;

		uint32_t m_RendererID;
		GLenum m_InternalFormat, m_DataFormat;
	};
}
