#pragma once
#include <glad/glad.h>

#include "RealEngine/Renderer/Buffer.h"

namespace RealEngine {
	static GLenum BufferTargetTypeToOpenGLBaseType(BufferTarget type) {
		switch (type) {
		case BufferTarget::None: break;
		}

		RE_CORE_ASSERT(false, "BufferTargetTypeToOpenGLBaseType Has not been implemented yet");
		//RE_CORE_ASSERT(false, "Unknown BufferTarget");
		return 0;
	}

	static GLenum BufferUsageTypeToOpenGLBaseType(BufferUsage type) {
		switch (type) {
		case BufferUsage::None: break;
		case BufferUsage::StreamDraw:  return	GL_STREAM_DRAW;
		case BufferUsage::StreamRead:  return	GL_STREAM_READ;
		case BufferUsage::StreamCopy:  return	GL_STREAM_COPY;
		case BufferUsage::StaticDraw:  return	GL_STATIC_DRAW;
		case BufferUsage::StaticRead:  return	GL_STATIC_READ;
		case BufferUsage::StaticCopy:  return	GL_STATIC_COPY;
		case BufferUsage::DynamicDraw: return	GL_DYNAMIC_DRAW;
		case BufferUsage::DynamicRead: return	GL_DYNAMIC_READ;
		case BufferUsage::DynamicCopy: return	GL_DYNAMIC_COPY;
		}

		RE_CORE_ASSERT(false, "Unknown BufferUsage");
		return 0;
	}

	static GLenum BufferModeTypeToOpenGLBaseType(BufferMode type) {
		switch (type) {
		case BufferMode::None: break;
		case BufferMode::Points: return					GL_POINTS;
		case BufferMode::LineStrip: return				GL_LINE_STRIP;
		case BufferMode::LineLoop: return				GL_LINE_LOOP;
		case BufferMode::Lines: return					GL_LINES;
		case BufferMode::LineStripAdjacency: return		GL_LINE_STRIP_ADJACENCY;
		case BufferMode::LinesAdjacency: return			GL_LINES_ADJACENCY;
		case BufferMode::TriangleStrip: return			GL_TRIANGLE_STRIP;
		case BufferMode::TriangleFan: return			GL_TRIANGLE_FAN;
		case BufferMode::Triangles: return				GL_TRIANGLES;
		case BufferMode::TriangleStripAdjacency: return GL_TRIANGLE_STRIP_ADJACENCY;
		case BufferMode::TrianglesAdjacency: return		GL_TRIANGLES_ADJACENCY;
		case BufferMode::Patches: return				GL_PATCHES;
		}

		RE_CORE_ASSERT(false, "Unknown BufferMode");
		return 0;
	}

	static GLenum ColorFormatTypeToOpenGLBaseType(ColorFormat type) {
		switch (type) {
		case ColorFormat::None:  break;
		case ColorFormat::R8:	 return		GL_R8;
		case ColorFormat::R32F:	 return		GL_R32F;
		case ColorFormat::RG32F: return		GL_RG32F;
		}

		RE_CORE_ASSERT(false, "Unknown ColorFormat");
		return 0;
	}
}
