#pragma once

#include <glad/glad.h>

#include "RealEngine/Renderer/Texture.h"

namespace RealEngine {
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
}