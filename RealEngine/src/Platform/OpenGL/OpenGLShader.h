#pragma once

#include "RealEngine/Renderer/Shader.h"
#include <glm/glm.hpp>

//TODO: Remove
typedef unsigned int GLenum;

namespace RealEngine {
	class OpenGLShader : public Shader {
	public:
		OpenGLShader(const std::filesystem::path& filepath);
		OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
		virtual ~OpenGLShader();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual const std::string& GetName() const override { return m_Name; }
	private:
		std::string ReadFile(const std::filesystem::path& filepath);
		std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);

		void CompileOrGetVulkanBinaries(const std::unordered_map<GLenum, std::string>& shaderSources);
		void CompileOrGetOpenGLBinaries();
		void CreateProgram();
		void Reflect(GLenum stage, const std::vector<uint32_t>& shaderData);

	private:
		uint32_t m_RendererID;
		std::filesystem::path m_FilePath;
		std::string m_Name;

		//Don't recompile is used for making sure both Vulkan and Opengl bianaries are recompiled
		std::unordered_map<GLenum, bool> m_DontRecompile;
		std::unordered_map<GLenum, std::vector<uint32_t>> m_VulkanSPIRV;
		std::unordered_map<GLenum, std::vector<uint32_t>> m_OpenGLSPIRV;

		std::unordered_map<GLenum, std::string> m_OpenGLSourceCode;
	};
}
