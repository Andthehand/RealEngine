#pragma once
#include "Texture.h"

namespace RealEngine {
	enum class ShaderDataType {
		None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, UInt, Int, Int2, Int3, Int4, Bool
	};

	static uint32_t ShaderDataTypeSize(ShaderDataType type) {
		switch (type) {
			case ShaderDataType::Float:	  return 4;
			case ShaderDataType::Float2:  return 4 * 2;
			case ShaderDataType::Float3:  return 4 * 3;
			case ShaderDataType::Float4:  return 4 * 4;
			case ShaderDataType::Mat3:	  return 4 * 3 * 3;
			case ShaderDataType::Mat4:	  return 4 * 4 * 4;
			case ShaderDataType::UInt:	  return 4;
			case ShaderDataType::Int:	  return 4;
			case ShaderDataType::Int2:	  return 4 * 2;
			case ShaderDataType::Int3:	  return 4 * 3;
			case ShaderDataType::Int4:	  return 4 * 4;
			case ShaderDataType::Bool:	  return 1;
		}
		RE_CORE_ASSERT(false, "Unknown ShaderDataType!");

		return 0;
	}

	enum class BufferUsage {
		None = 0, StreamDraw, StreamRead, StreamCopy, StaticDraw, StaticRead, StaticCopy, DynamicDraw, DynamicRead, DynamicCopy
	};

	enum class BufferMode {
		None = 0, Points, LineStrip, LineLoop, Lines, LineStripAdjacency, LinesAdjacency, TriangleStrip, TriangleFan, Triangles, TriangleStripAdjacency, TrianglesAdjacency, Patches
	};

	enum class BufferTarget {
		None = 0
	};

	struct BufferElement {
		std::string Name;
		ShaderDataType Type;
		uint32_t Size;
		size_t Offset;
		uint32_t InstancedDivisor;
		bool Normalized;

		BufferElement() = default;

		BufferElement(ShaderDataType type, const std::string& name, uint32_t instancedDivisor = 0, bool normalized = false)
			: Name(name), Type(type), Size(ShaderDataTypeSize(type)), Offset(0), InstancedDivisor(instancedDivisor), Normalized(normalized) { }
	
		uint32_t GetComponentCount() const {
			switch (Type) {
				case ShaderDataType::Float:  return 1;
				case ShaderDataType::Float2: return 2;
				case ShaderDataType::Float3: return 3;
				case ShaderDataType::Float4: return 4;
				case ShaderDataType::Mat3:   return 3; // 3* float3
				case ShaderDataType::Mat4:   return 4; // 4* float4
				case ShaderDataType::UInt:	 return 1;
				case ShaderDataType::Int:	 return 1;
				case ShaderDataType::Int2:	 return 2;
				case ShaderDataType::Int3:	 return 3;
				case ShaderDataType::Int4:	 return 4;
				case ShaderDataType::Bool:	 return 1;
			}

			RE_CORE_ASSERT(false, "Unknown ShaderDataType!");
			return 0;
		}
	};

	class BufferLayout {
	public:
		BufferLayout() = default;

		BufferLayout(std::initializer_list<BufferElement> elements) : m_Elements(elements) {
			CalculateOffsetsAndStride();
		}

		inline uint32_t GetStride() const { return m_Stride; }
		inline const std::vector<BufferElement>& GetElements() const { return m_Elements; }
	
		std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
		std::vector<BufferElement>::iterator end() { return m_Elements.end(); }
		std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }
	private:
		//Loops through all of the elemets that are passed into this class and calculates the offset and stride automatically
		void CalculateOffsetsAndStride() {
			size_t offset = 0;
			m_Stride = 0;
			for (auto& element : m_Elements) {
				element.Offset = offset;
				offset += element.Size;
				m_Stride += element.Size;
			}
		}

		std::vector<BufferElement> m_Elements;
		uint32_t m_Stride = 0;
	};

	//Not Tested
	class Buffer {
	public:
		virtual ~Buffer() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetData(const void* data, uint32_t size) = 0;

		static Ref<Buffer> Create(BufferTarget target, uint32_t size, BufferUsage usage);
		static Ref<Buffer> Create(BufferTarget target, uint32_t size, const void* data, BufferUsage usage);
	};

	class TextureBuffer {
	public:
		virtual ~TextureBuffer() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual uint32_t GetRenderID() const = 0;

		static Ref<TextureBuffer> Create(uint32_t size, const void* data, BufferUsage usage, ColorFormat colorFormat);
	};

	class VertexBuffer {
	public:
		virtual ~VertexBuffer() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetData(const void* data, uint32_t size) = 0;

		virtual const BufferLayout& GetLayout() const = 0;
		virtual void SetLayout(const BufferLayout& layout) = 0;
		
		static Ref<VertexBuffer> Create(uint32_t size);
		static Ref<VertexBuffer> Create(const float* verticies, uint32_t size);
	};

	class IndexBuffer {
	public:
		virtual ~IndexBuffer() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetData(const uint32_t* data, uint32_t count) = 0;

		virtual uint32_t GetCount() const = 0;

		static Ref<IndexBuffer> Create(uint32_t count);
		static Ref<IndexBuffer> Create(const uint32_t* indices, uint32_t count);
	};
}
