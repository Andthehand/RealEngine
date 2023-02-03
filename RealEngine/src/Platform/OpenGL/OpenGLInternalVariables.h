#pragma once
#include <glad/glad.h>

namespace RealEngine {
	enum BufferUsage : GLenum {
		StreamData = GL_STREAM_DRAW,
		StreamRead = GL_STREAM_READ,
		StreamCopy = GL_STREAM_COPY,
		StaticDraw = GL_STATIC_DRAW,
		StaticRead = GL_STATIC_READ,
		StaticCopy = GL_STATIC_COPY,
		DynamicDraw = GL_DYNAMIC_DRAW,
		DynamicRead = GL_DYNAMIC_READ,
		DynamicCopy = GL_DYNAMIC_COPY
	};

	enum BufferTarget : GLenum {

	};

	enum InternalFormat : GLenum {
		R8 = GL_R8,
		R32F = GL_R32F
	};
}
