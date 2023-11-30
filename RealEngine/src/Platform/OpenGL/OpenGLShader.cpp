#include "repch.h"
#include "OpenGLShader.h"

#include <fstream>
#include <string>
#include <unordered_map>

#include <glm/gtc/type_ptr.hpp>

#include <shaderc/shaderc.hpp>
#include <spirv_cross/spirv_cross.hpp>
#include <spirv_cross/spirv_glsl.hpp>

#include "RealEngine/Core/Timer.h"

namespace RealEngine {
	namespace Utils {
		static GLenum ShaderTypeFromString(const std::string& type) {
			if (type == "vertex")
				return GL_VERTEX_SHADER;
			if (type == "fragment" || type == "pixel")
				return GL_FRAGMENT_SHADER;
			if (type == "geometry" || type == "geo")
				return GL_GEOMETRY_SHADER;
			if (type == "compute")
				return GL_COMPUTE_SHADER;

			RE_CORE_ASSERT(false, "Unknown shader type!");
			return 0;
		}

		static shaderc_shader_kind GLShaderStageToShaderC(GLenum stage) {
			switch (stage) {
				case GL_VERTEX_SHADER:   return shaderc_glsl_vertex_shader;
				case GL_FRAGMENT_SHADER: return shaderc_glsl_fragment_shader;
				case GL_GEOMETRY_SHADER: return shaderc_glsl_geometry_shader;
				case GL_COMPUTE_SHADER:  return shaderc_glsl_compute_shader;
			}
			RE_CORE_ASSERT(false);
			return (shaderc_shader_kind)0;
		}

		static const char* GLShaderStageToString(GLenum stage) {
			switch (stage) {
				case GL_VERTEX_SHADER:   return "GL_VERTEX_SHADER";
				case GL_FRAGMENT_SHADER: return "GL_FRAGMENT_SHADER";
				case GL_GEOMETRY_SHADER: return "GL_GEOMETRY_SHADER";
				case GL_COMPUTE_SHADER: return "GL_COMPUTE_SHADER";
			}
			RE_CORE_ASSERT(false);
			return nullptr;
		}

		static const char* GetCacheDirectory() {
			return "assets/cache/shader/opengl";
		}

		static void CreateCacheDirectoryIfNeeded() {
			std::string cacheDirectory = GetCacheDirectory();
			if (!std::filesystem::exists(cacheDirectory))
				std::filesystem::create_directories(cacheDirectory);
		}

		//Returns false if the file doesn't exist or if the hash is different
		static const bool CompareFileHashes(const std::filesystem::path& filepath, uint32_t stage, const std::string& source) {
			RE_PROFILE_FUNCTION();
			
			size_t hashCompare = std::hash<std::string>{}(source);
			
			std::ifstream in(filepath, std::ios::in | std::ios::binary);
			if (in.is_open()) {
				in.seekg(0, std::ios::end);
				auto size = in.tellg();
				in.seekg(0, std::ios::beg);

				size_t hashRead = 0;
				in.read((char*)&hashRead, size);

				if (hashRead == hashCompare)
					return true;
				else {
					std::ofstream out(filepath, std::ios::out | std::ios::binary);
					if (out.is_open()) {
						out.write((char*)&hashCompare, sizeof(hashCompare));
						out.flush();
						out.close();
					}
					return false;
				}
			}
			else {
				std::ofstream out(filepath, std::ios::out | std::ios::binary);
				if (out.is_open()) {
					out.write((char*)&hashCompare, sizeof(hashCompare));
					out.flush();
					out.close();
				}
				return false;
			}
		}

		static const std::filesystem::path GLShaderStageCachedOpenGLFileExtension(uint32_t stage) {
			switch (stage) {
				case GL_VERTEX_SHADER:    return ".cached_opengl.vert";
				case GL_FRAGMENT_SHADER:  return ".cached_opengl.frag";
				case GL_GEOMETRY_SHADER:  return ".cached_opengl.geo";
				case GL_COMPUTE_SHADER:   return ".cached_opengl.comp";
			}
			RE_CORE_ASSERT(false);
			return "";
		}

		static const std::filesystem::path GLShaderStageCachedVulkanFileExtension(uint32_t stage) {
			switch (stage) {
				case GL_VERTEX_SHADER:    return ".cached_vulkan.vert";
				case GL_FRAGMENT_SHADER:  return ".cached_vulkan.frag";
				case GL_GEOMETRY_SHADER:  return ".cached_vulkan.geo";
				case GL_COMPUTE_SHADER:   return ".cached_vulkan.comp";
			}
			RE_CORE_ASSERT(false);
			return "";
		}

		static const std::filesystem::path GLShaderStageCachedStringHashFileExtension(uint32_t stage) {
			switch (stage) {
				case GL_VERTEX_SHADER:    return ".string_hash.vert";
				case GL_FRAGMENT_SHADER:  return ".string_hash.frag";
				case GL_GEOMETRY_SHADER:  return ".string_hash.geo";
				case GL_COMPUTE_SHADER:   return ".string_hash.comp";
			}
			RE_CORE_ASSERT(false);
			return "";
		}
	}

	OpenGLShader::OpenGLShader(const std::filesystem::path& filepath)
		: m_FilePath(filepath) {
		RE_PROFILE_FUNCTION();

		m_Name = filepath.stem().string();
		Utils::CreateCacheDirectoryIfNeeded();

		std::string source = ReadFile(filepath);
		if (source.empty()) {
			RE_CORE_WARN("Failed to create shader: {0}", m_Name);
			return;
		}

		auto shaderSources = PreProcess(source);

		{
			Timer timer;
			CompileOrGetVulkanBinaries(shaderSources);
			CompileOrGetOpenGLBinaries();
			CreateProgram();
			RE_CORE_WARN("Shader {0} took {1} ms to create", filepath.stem().string(), timer.ElapsedMillis());
		}

	}

	OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
		: m_Name(name), m_FilePath(name) {
		RE_PROFILE_FUNCTION();

		std::unordered_map<GLenum, std::string> sources;
		sources[GL_VERTEX_SHADER] = vertexSrc;
		sources[GL_FRAGMENT_SHADER] = fragmentSrc;
		

		{
			Timer timer;
			CompileOrGetVulkanBinaries(sources);
			CompileOrGetOpenGLBinaries();
			CreateProgram();
			RE_CORE_WARN("Shader {0} took {1} ms to create", name, timer.ElapsedMillis());
		}
	}

	OpenGLShader::~OpenGLShader() {
		RE_PROFILE_FUNCTION();

		glDeleteProgram(m_RendererID);
	}

	std::string OpenGLShader::ReadFile(const std::filesystem::path& filepath) {
		RE_PROFILE_FUNCTION();

		std::string result;
		std::ifstream in(filepath, std::ios::in | std::ios::binary); // ifstream closes itself due to RAII
		if (in) {
			in.seekg(0, std::ios::end);
			size_t size = in.tellg();
			if (size != -1) {
				result.resize(size);
				in.seekg(0, std::ios::beg);
				in.read(&result[0], size);
			}
			else {
				RE_CORE_ERROR("Could not read from file '{0}'", filepath);
			}
		}
		else {
			RE_CORE_ERROR("Could not open file '{0}'", filepath);
		}

		return result;
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source) {
		RE_PROFILE_FUNCTION();

		std::unordered_map<GLenum, std::string> shaderSources;

		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0); //Start of shader type declaration line
		while (pos != std::string::npos) {
			size_t eol = source.find_first_of("\r\n", pos); //End of shader type declaration line
			RE_CORE_ASSERT(eol != std::string::npos, "Syntax error");
			size_t begin = pos + typeTokenLength + 1; //Start of shader type name (after "#type " keyword)
			std::string type = source.substr(begin, eol - begin);
			RE_CORE_ASSERT(Utils::ShaderTypeFromString(type), "Invalid shader type specified");

			size_t nextLinePos = source.find_first_not_of("\r\n", eol); //Start of shader code after shader type declaration line
			RE_CORE_ASSERT(nextLinePos != std::string::npos, "Syntax error");
			pos = source.find(typeToken, nextLinePos); //Start of next shader type declaration line

			shaderSources[Utils::ShaderTypeFromString(type)] = (pos == std::string::npos) ? source.substr(nextLinePos) : source.substr(nextLinePos, pos - nextLinePos);
		}

		return shaderSources;
	}

	void OpenGLShader::CompileOrGetVulkanBinaries(const std::unordered_map<GLenum, std::string>& shaderSources) {
		RE_PROFILE_FUNCTION();
		
		GLuint program = glCreateProgram();

		shaderc::Compiler compiler;
		shaderc::CompileOptions options;
		options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_2);
		const bool optimize = true;
		if (optimize)
			options.SetOptimizationLevel(shaderc_optimization_level_performance);

		options.SetAutoMapLocations(true);
		options.SetAutoBindUniforms(true);

		std::filesystem::path cacheDirectory = Utils::GetCacheDirectory();

		auto& shaderData = m_VulkanSPIRV;
		shaderData.clear();
		for (auto&& [stage, source] : shaderSources) {
			std::filesystem::path shaderFilePath = m_FilePath;
			std::filesystem::path stringHashPath = cacheDirectory / (shaderFilePath.stem() += Utils::GLShaderStageCachedStringHashFileExtension(stage));
			std::filesystem::path cachedPath = cacheDirectory / (shaderFilePath.stem() += Utils::GLShaderStageCachedVulkanFileExtension(stage));

			//Check if the bianary already exists or if the file changed
			m_DontRecompile[stage] = Utils::CompareFileHashes(stringHashPath, stage, source);
			std::ifstream in(cachedPath, std::ios::in | std::ios::binary);
			if (m_DontRecompile[stage] && in.is_open()) {
				in.seekg(0, std::ios::end);
				auto size = in.tellg();
				in.seekg(0, std::ios::beg);

				auto& data = shaderData[stage];
				data.resize(size / sizeof(uint32_t));
				in.read((char*)data.data(), size);
			}
			else {
				shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(source, Utils::GLShaderStageToShaderC(stage), m_Name.c_str(), options);
				if (module.GetCompilationStatus() != shaderc_compilation_status_success) {
					RE_CORE_ERROR(module.GetErrorMessage());
					RE_CORE_ASSERT(false);
				}

				shaderData[stage] = std::vector<uint32_t>(module.cbegin(), module.cend());

				std::ofstream out(cachedPath, std::ios::out | std::ios::binary | std::ios::trunc);
				if (out.is_open()) {
					auto& data = shaderData[stage];
					out.write((char*)data.data(), data.size() * sizeof(uint32_t));
					out.flush();
					out.close();
				}
			}
		}

		//for (auto&& [stage, data] : shaderData)
		//	Reflect(stage, data);
	}

	void OpenGLShader::CompileOrGetOpenGLBinaries() {
		RE_PROFILE_FUNCTION();
		
		auto& shaderData = m_OpenGLSPIRV;

		shaderc::Compiler compiler;
		shaderc::CompileOptions options;
		options.SetTargetEnvironment(shaderc_target_env_opengl, shaderc_env_version_opengl_4_5);
		const bool optimize = true;
		if (optimize)
			options.SetOptimizationLevel(shaderc_optimization_level_performance);

		std::filesystem::path cacheDirectory = Utils::GetCacheDirectory();

		shaderData.clear();
		m_OpenGLSourceCode.clear();
		for (auto&& [stage, spirv] : m_VulkanSPIRV) {
			std::filesystem::path shaderFilePath = m_FilePath;
			std::filesystem::path cachedPath = cacheDirectory / shaderFilePath.stem() += Utils::GLShaderStageCachedOpenGLFileExtension(stage);

			std::ifstream in(cachedPath, std::ios::in | std::ios::binary);
			if (m_DontRecompile[stage] && in.is_open()) {
				in.seekg(0, std::ios::end);
				auto size = in.tellg();
				in.seekg(0, std::ios::beg);

				auto& data = shaderData[stage];
				data.resize(size / sizeof(uint32_t));
				in.read((char*)data.data(), size);
			}
			else {
				spirv_cross::CompilerGLSL glslCompiler(spirv);
				m_OpenGLSourceCode[stage] = glslCompiler.compile();
				auto& source = m_OpenGLSourceCode[stage];

				shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(source, Utils::GLShaderStageToShaderC(stage), m_FilePath.string().c_str());
				if (module.GetCompilationStatus() != shaderc_compilation_status_success) {
					RE_CORE_ERROR(module.GetErrorMessage());
					RE_CORE_ASSERT(false);
				}

				shaderData[stage] = std::vector<uint32_t>(module.cbegin(), module.cend());

				std::ofstream out(cachedPath, std::ios::out | std::ios::binary| std::ios::trunc);
				if (out.is_open()) {
					auto& data = shaderData[stage];
					out.write((char*)data.data(), data.size() * sizeof(uint32_t));
					out.flush();
					out.close();
				}
			}
		}
	}

	void OpenGLShader::CreateProgram() {
		RE_PROFILE_FUNCTION();
	
		GLuint program = glCreateProgram();

		std::vector<GLuint> shaderIDs;
		for (auto&& [stage, spirv] : m_OpenGLSPIRV) {
			GLint ExtensionCount = 0;
			glGetIntegerv(GL_NUM_PROGRAM_BINARY_FORMATS, &ExtensionCount);

			GLuint shaderID = shaderIDs.emplace_back(glCreateShader(stage));
			glShaderBinary(1, &shaderID, GL_SHADER_BINARY_FORMAT_SPIR_V, spirv.data(), (GLsizei)(spirv.size() * sizeof(uint32_t)));
			glSpecializeShader(shaderID, "main", 0, nullptr, nullptr);
			glAttachShader(program, shaderID);
		}

		glLinkProgram(program);

		GLint isLinked;
		glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
		if (isLinked == GL_FALSE) {
			GLint maxLength;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, infoLog.data());
			RE_CORE_ERROR("Shader linking failed ({0}):\n{1}", m_FilePath, infoLog.data());

			glDeleteProgram(program);

			for (auto id : shaderIDs)
				glDeleteShader(id);
		}

		for (auto id : shaderIDs) {
			glDetachShader(program, id);
			glDeleteShader(id);
		}

		m_RendererID = program;

		//Cleanup
		m_DontRecompile.clear();
		m_VulkanSPIRV.clear();
		m_OpenGLSPIRV.clear();
		m_OpenGLSourceCode.clear();
	}

	void OpenGLShader::Reflect(GLenum stage, const std::vector<uint32_t>& shaderData) {
		RE_PROFILE_FUNCTION();
		
		spirv_cross::Compiler compiler(shaderData);
		spirv_cross::ShaderResources resources = compiler.get_shader_resources();

		RE_CORE_TRACE("OpenGLShader::Reflect - {0} {1}", Utils::GLShaderStageToString(stage), m_FilePath);
		RE_CORE_TRACE("    {0} uniform buffers", resources.uniform_buffers.size());
		RE_CORE_TRACE("    {0} resources", resources.sampled_images.size());

		RE_CORE_TRACE("Uniform buffers:");
		for (const auto& resource : resources.uniform_buffers) {
			const auto& bufferType = compiler.get_type(resource.base_type_id);
			uint32_t bufferSize = (uint32_t)compiler.get_declared_struct_size(bufferType);
			uint32_t binding = compiler.get_decoration(resource.id, spv::DecorationBinding);
			int memberCount = (int)bufferType.member_types.size();

			RE_CORE_TRACE("  {0}", resource.name);
			RE_CORE_TRACE("    Size = {0}", bufferSize);
			RE_CORE_TRACE("    Binding = {0}", binding);
			RE_CORE_TRACE("    Members = {0}", memberCount);
		}
	}

	void OpenGLShader::Bind() const {
		glUseProgram(m_RendererID);
	}

	void OpenGLShader::Unbind() const {
		glUseProgram(0);
	}
}
