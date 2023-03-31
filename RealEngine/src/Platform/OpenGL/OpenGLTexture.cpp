#include "repch.h"
#include "OpenGLTexture.h"

#include "stb_image.h"

namespace RealEngine {
	//------------------------------------
	// Texture2D -------------------------
	//------------------------------------
	OpenGLTexture2D::OpenGLTexture2D(const uint32_t width, const uint32_t height) : m_Width(width), m_Height(height) {
		RE_PROFILE_FUNCTION();

		m_InternalFormat = GL_RGBA8;
		m_DataFormat = GL_RGBA;

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	OpenGLTexture2D::OpenGLTexture2D(const std::filesystem::path& path) : m_Path(path) {
		RE_PROFILE_FUNCTION();

		int width, height, channels;
		stbi_uc* data = nullptr;
		stbi_set_flip_vertically_on_load(1);
		{
			RE_PROFILE_SCOPE("stbi_load - OpenGLTexture2D::OpenGLTexture2D(const std::string&)");
			data = stbi_load(path.string().c_str(), &width, &height, &channels, 0);
		}
		RE_CORE_ASSERT(data, "Failed to load image!");
		m_Width = width;
		m_Height = height;

		GLenum internalFormat = 0, dataFormat = 0;
		if (channels == 4) {
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
		}
		else if (channels == 3) {
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
		}

		m_InternalFormat = internalFormat;
		m_DataFormat = dataFormat;

		RE_CORE_ASSERT(internalFormat & dataFormat, "Format not supported!");

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, internalFormat, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
	}

	OpenGLTexture2D::~OpenGLTexture2D() {
		RE_PROFILE_FUNCTION();

		glDeleteTextures(1, &m_RendererID);
	}
	
	void OpenGLTexture2D::SetData(void* data, uint32_t size) {
		RE_PROFILE_FUNCTION();

		uint32_t bpp = m_DataFormat == GL_RGBA ? 4 : 3;
		RE_CORE_ASSERT(size == m_Width * m_Height * bpp, "Data must be entire texture!");
		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const {
		glBindTextureUnit(slot, m_RendererID);	
	}

	//------------------------------------
	// Texture2DArray --------------------
	//------------------------------------
	OpenGLTexture2DArray::OpenGLTexture2DArray(uint32_t width, uint32_t height, uint32_t numTextures, uint32_t mipLevels) : m_Width(width), m_Height(height) {
		RE_PROFILE_FUNCTION();

		m_InternalFormat = GL_RGBA8;
		m_DataFormat = GL_RGBA;

		glCreateTextures(GL_TEXTURE_2D_ARRAY, 1, &m_RendererID);
		glTextureStorage3D(GL_TEXTURE_2D_ARRAY, mipLevels, GL_RGBA8, width, height, numTextures);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}
	
	OpenGLTexture2DArray::OpenGLTexture2DArray(const std::initializer_list<std::filesystem::path> paths, uint32_t mipLevels) {
		RE_PROFILE_FUNCTION();

		uint32_t numTextures = (uint32_t)paths.size();
		RE_ASSERT(numTextures != 0, "Can't make Texuture without any textures");

		TextureData* textureData = new TextureData[numTextures];
		stbi_uc** data = new stbi_uc*[numTextures];

		int checkChannel = -1;
		stbi_set_flip_vertically_on_load(1);
		//Load all of the images into memory
		uint32_t i = 0;
		for (auto& path : paths) {
			RE_ASSERT(i >= numTextures, "I is outside the range of allocated memory");
			data[i] = stbi_load(path.string().c_str(), &textureData[i].Width, &textureData[i].Height, &textureData[i].Channels, 0);
			RE_CORE_ASSERT(data[i], "Failed to load image!");

			//Set data only on the first texture upload
			if (checkChannel == -1) {
				checkChannel = textureData[i].Channels;
				m_Width = textureData[i].Width;
				m_Height = textureData[i].Height;
			}

			//Make sure the channels, width, and height are the same
			RE_ASSERT(checkChannel == textureData[i].Channels &&
				m_Width == textureData[i].Width &&
				m_Height == textureData[i].Height, "Channel, Width, or height do not match while creating TextureArray");
			i++;
		}

		if (checkChannel == 4) {
			m_InternalFormat = GL_RGBA8;
			m_DataFormat = GL_RGBA;
		}
		else if (checkChannel == 3) {
			m_InternalFormat = GL_RGB8;
			m_DataFormat = GL_RGB;
		}
		RE_CORE_ASSERT(m_InternalFormat & m_DataFormat, "Format not supported!");

		//Create TextureArray and allocate memory
		glCreateTextures(GL_TEXTURE_2D_ARRAY, 1, &m_RendererID);
		glTextureStorage3D(m_RendererID, mipLevels, m_InternalFormat, m_Width, m_Height, numTextures);
		//Upload the images to the GPU
		for (uint32_t i = 0; i < numTextures; i++) {
			glTextureSubImage3D(m_RendererID, 0, 0, 0, i, m_Width, m_Height, 1, m_DataFormat, GL_UNSIGNED_BYTE, data[i]);
		}
		glGenerateTextureMipmap(m_RendererID);

		GLfloat max_aniso; 
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &max_aniso); 
		glTextureParameterf(m_RendererID, GL_TEXTURE_MAX_ANISOTROPY, max_aniso);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);


		//Delete rescources
		for (uint32_t i = 0; i < numTextures; i++) {
			stbi_image_free(data[i]);
		}
		delete[] data;
		delete[] textureData;
	}
	
	OpenGLTexture2DArray::~OpenGLTexture2DArray() {
		glDeleteTextures(1, &m_RendererID);
	}
	
	//TODO: Implement
	void OpenGLTexture2DArray::SetData(void* data, uint32_t size) {
		assert(false);
	}
	
	void OpenGLTexture2DArray::Bind(uint32_t slot) const {
		glBindTextureUnit(slot, m_RendererID);
	}
}
