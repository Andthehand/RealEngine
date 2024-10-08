#pragma once
#include "glm/glm.hpp"

namespace RealEngine {
	class Variant {
	public:
		Variant(bool p_bool);
		Variant(int p_int);
		Variant(float p_float);
		Variant(const std::string& p_string);

		Variant(const glm::vec2& p_vec2);
		Variant(const glm::ivec2& p_vec2i);
		Variant(const glm::vec3& p_vec3);
		Variant(const glm::ivec3& p_vec3i);
		Variant(const glm::vec4& p_vec4);
		Variant(const glm::ivec4& p_vec4i);

		Variant(const Variant& p_other);

		~Variant();

		Variant& operator=(const Variant& p_other);

		explicit operator bool() const;
		explicit operator int() const;
		explicit operator float() const;
		explicit operator float*();
		operator std::string() const;
		explicit operator std::string*();

		explicit operator glm::vec2() const;
		explicit operator glm::ivec2() const;
		explicit operator glm::vec3() const;
		explicit operator glm::ivec3() const;
		explicit operator glm::vec4() const;
		explicit operator glm::ivec4() const;
	public:
		enum Type {
			NIL,

			// atomic types
			BOOL,
			INT,
			FLOAT,
			STRING,

			// math types
			VECTOR2,
			VECTOR2I,
			RECT2,
			RECT2I,
			VECTOR3,
			VECTOR3I,
			VECTOR4,
			VECTOR4I
		};
		Type m_Type = NIL;

	private:
		union {
			bool _bool;
			int64_t _int;
			float _float;
			std::string* _string;

			glm::vec2* _vec2;
			glm::ivec2* _vec2i;
			glm::vec3* _vec3;
			glm::ivec3* _vec3i;
			glm::vec4* _vec4;
			glm::ivec4* _vec4i;
		} m_Data;
	};
}
