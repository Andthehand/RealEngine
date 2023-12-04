#pragma once

#include "RealEngine/Renderer/Shader.h"
#include <glad/glad.h>

namespace RealEngine {
	class OpenGLShader : public Shader {
	public:
		OpenGLShader(const std::filesystem::path& filepath);
		OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc, ShaderReflect* reflect);
		virtual ~OpenGLShader();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetInt(const std::string& name, int value) override;
		virtual void SetIntArray(const std::string& name, int* values, uint32_t count) override;

		virtual void SetFloat(const std::string& name, const float value) override;
		virtual void SetFloat2(const std::string& name, const glm::vec2& value) override;
		virtual void SetFloat3(const std::string& name, const glm::vec3& value) override;
		virtual void SetFloat4(const std::string& name, const glm::vec4& value) override;
		virtual void SetMat4(const std::string& name, const glm::mat4& value) override;

		virtual const std::string& GetName() const override { return m_Name; }
	private:
		std::string ReadFile(const std::filesystem::path& filepath);
		std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);

		void CompileOrGetOpenGLBinaries(const std::unordered_map<GLenum, std::string>& shaderSources);
		void CreateProgram();

		void GenerateReflectData(ShaderReflect* reflect);
		void DebugReflect(GLenum stage, const std::vector<uint32_t>& shaderData);

	private:
		uint32_t m_RendererID;
		std::filesystem::path m_FilePath;
		//TODO: Remove this and get the name from the filepath
		std::string m_Name;

		//Don't recompile is used for making sure both Vulkan and Opengl bianaries are recompiled
		std::unordered_map<GLenum, bool> m_DontRecompile;
		std::unordered_map<GLenum, std::vector<uint32_t>> m_OpenGLSPIRV;
	};
}
