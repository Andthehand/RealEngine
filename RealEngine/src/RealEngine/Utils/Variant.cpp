#include "repch.h"
#include "Variant.h"

namespace RealEngine {
	Variant::Variant(bool p_bool) {
		m_Data._bool = p_bool;
		m_Type = BOOL;
	}

	Variant::Variant(int p_int) {
		m_Data._int = p_int;
		m_Type = INT;
	}

	Variant::Variant(float p_float) {
		m_Data._float = p_float;
		m_Type = FLOAT;
	}

	Variant::Variant(const std::string& p_string) {
		m_Data._string = new std::string(p_string);
		m_Type = STRING;
	}

	Variant::Variant(const glm::vec2& p_vec2) {
		m_Data._vec2 = new glm::vec2(p_vec2);
		m_Type = VECTOR2;
	}

	Variant::Variant(const glm::ivec2& p_vec2i) {
		m_Data._vec2i = new glm::ivec2(p_vec2i);
		m_Type = VECTOR2I;
	}

	Variant::Variant(const glm::vec3& p_vec3) {
		m_Data._vec3 = new glm::vec3(p_vec3);
		m_Type = VECTOR3;
	}

	Variant::Variant(const glm::ivec3& p_vec3i) {
		m_Data._vec3i = new glm::ivec3(p_vec3i);
		m_Type = VECTOR3I;
	}

	Variant::Variant(const glm::vec4& p_vec4) {
		m_Data._vec4 = new glm::vec4(p_vec4);
		m_Type = VECTOR4;
	}

	Variant::Variant(const glm::ivec4& p_vec4i) {
		m_Data._vec4i = new glm::ivec4(p_vec4i);
		m_Type = VECTOR4I;
	}

	Variant::Variant(const Variant& p_other) {
		m_Type = p_other.m_Type;
		switch (m_Type) {
			case STRING:
				m_Data._string = new std::string(*p_other.m_Data._string);
				break;
			case VECTOR2:
				m_Data._vec2 = new glm::vec2(*p_other.m_Data._vec2);
				break;
			case VECTOR2I:
				m_Data._vec2i = new glm::ivec2(*p_other.m_Data._vec2i);
				break;
			case VECTOR3:
				m_Data._vec3 = new glm::vec3(*p_other.m_Data._vec3);
				break;
			case VECTOR3I:
				m_Data._vec3i = new glm::ivec3(*p_other.m_Data._vec3i);
				break;
			case VECTOR4:
				m_Data._vec4 = new glm::vec4(*p_other.m_Data._vec4);
				break;
			case VECTOR4I:
				m_Data._vec4i = new glm::ivec4(*p_other.m_Data._vec4i);
				break;
		}
	}

	Variant::~Variant() {
		switch (m_Type) {
			case STRING:
				delete m_Data._string;
				break;
			case VECTOR2:
				delete m_Data._vec2;
				break;
			case VECTOR2I:
				delete m_Data._vec2i;
				break;
			case VECTOR3:
				delete m_Data._vec3;
				break;
			case VECTOR3I:
				delete m_Data._vec3i;
				break;
			case VECTOR4:
				delete m_Data._vec4;
				break;
			case VECTOR4I:
				delete m_Data._vec4i;
				break;
		}
	}

	Variant& Variant::operator=(const Variant& p_other) {
		m_Type = p_other.m_Type;
		switch (m_Type) {
			case STRING:
				m_Data._string = new std::string(*p_other.m_Data._string);
				break;
			case VECTOR2:
				m_Data._vec2 = new glm::vec2(*p_other.m_Data._vec2);
				break;
			case VECTOR2I:
				m_Data._vec2i = new glm::ivec2(*p_other.m_Data._vec2i);
				break;
			case VECTOR3:
				m_Data._vec3 = new glm::vec3(*p_other.m_Data._vec3);
				break;
			case VECTOR3I:
				m_Data._vec3i = new glm::ivec3(*p_other.m_Data._vec3i);
				break;
			case VECTOR4:
				m_Data._vec4 = new glm::vec4(*p_other.m_Data._vec4);
				break;
			case VECTOR4I:
				m_Data._vec4i = new glm::ivec4(*p_other.m_Data._vec4i);
				break;
		}
		return *this;
	}

	Variant::operator void* () const {
		switch (m_Type) {
			case NIL:
				return nullptr;
			case BOOL:
				return (void*)m_Data._bool;
			case INT:
				return (void*)m_Data._int;
			case FLOAT:
				return (void*)(int)m_Data._float;
			case VECTOR2:
				return m_Data._vec2;
			case VECTOR2I:
				return m_Data._vec2i;
			case VECTOR3:
				return m_Data._vec3;
			case VECTOR3I:
				return m_Data._vec3i;
			case VECTOR4:
				return m_Data._vec4;
			case VECTOR4I:
				return m_Data._vec4i;
			default:
				RE_ASSERT(false, "Variant type not supported");
				return nullptr;
		}
	}

	// We consider all uninitialized or empty types to be false based on the type's
	// zeroiness.
	Variant::operator bool() const {
		switch (m_Type) {
			case NIL:
				return true;
			case BOOL:
				return m_Data._bool;
			case INT:
				return m_Data._int != 0;
			case FLOAT:
				return m_Data._float != 0.0f;
			case STRING:
				return !m_Data._string->empty();
			case VECTOR2:
				return m_Data._vec2;
			case VECTOR2I:
				return m_Data._vec2i;
			case VECTOR3:
				return m_Data._vec3;
			case VECTOR3I:
				return m_Data._vec3i;
			case VECTOR4:
				return m_Data._vec4;
			case VECTOR4I:
				return m_Data._vec4i;
			default:
				RE_ASSERT(false, "Variant type not supported");
				return false;
		}
	}

	Variant::operator int() const {
		switch (m_Type) {
			case NIL:
				return 0;
			case BOOL:
				return m_Data._bool;
			case INT:
				return m_Data._int;
			case FLOAT:
				return m_Data._float;
			case STRING:
				return std::stoi(*m_Data._string);
			default:
				RE_ASSERT(false, "Variant type not convertable to int!");
				return 0;
		}
	}

	Variant::operator float() const {
		switch (m_Type) {
			case NIL:
				return 0.0f;
			case BOOL:
				return m_Data._bool;
			case INT:
				return m_Data._int;
			case FLOAT:
				return m_Data._float;
			case STRING:
				return std::stof(*m_Data._string);
			default:
				RE_ASSERT(false, "Variant type not convertable to float!");
				return 0.0f;
		}
	}

	Variant::operator std::string() const {
		switch (m_Type) {
			case NIL:
				return "Null";
			case BOOL:
				return m_Data._bool ? "true" : "false";
			case INT:
				return std::to_string(m_Data._int);
			case FLOAT:
				return std::to_string(m_Data._float);
			case STRING:
				return *m_Data._string;
			case VECTOR2:
				return glm::to_string(*m_Data._vec2);
			case VECTOR2I:
				return glm::to_string(*m_Data._vec2i);
			case VECTOR3:
				return glm::to_string(*m_Data._vec3);
			case VECTOR3I:
				return glm::to_string(*m_Data._vec3i);
			case VECTOR4:
				return glm::to_string(*m_Data._vec4);
			case VECTOR4I:
				return glm::to_string(*m_Data._vec4i);
			default:
				RE_ASSERT(false, "Variant type not convertable to string!");
				return "ERROR";
		}
	}

	Variant::operator glm::vec2() const {
		switch (m_Type) {
			case VECTOR2:
				return *m_Data._vec2;
			case VECTOR2I:
				return glm::vec2(m_Data._vec2i->x, m_Data._vec2i->y);
			case VECTOR3:
				return glm::vec2(m_Data._vec3->x, m_Data._vec3->y);
			case VECTOR3I:
				return glm::vec2(m_Data._vec3i->x, m_Data._vec3i->y);
			case VECTOR4:
				return glm::vec2(m_Data._vec4->x, m_Data._vec4->y);
			case VECTOR4I:
				return glm::vec2(m_Data._vec4i->x, m_Data._vec4i->y);
			default:
				RE_ASSERT(false, "Variant type not convertable to glm::vec2!");
				return glm::vec2(0.0f);
		}
	}

	Variant::operator glm::ivec2() const {
		switch (m_Type) {
			case VECTOR2:
				return glm::ivec2(m_Data._vec2->x, m_Data._vec2->y);
			case VECTOR2I:
				return *m_Data._vec2i;
			case VECTOR3:
				return glm::ivec2(m_Data._vec3->x, m_Data._vec3->y);
			case VECTOR3I:
				return glm::ivec2(m_Data._vec3i->x, m_Data._vec3i->y);
			case VECTOR4:
				return glm::ivec2(m_Data._vec4->x, m_Data._vec4->y);
			case VECTOR4I:
				return glm::ivec2(m_Data._vec4i->x, m_Data._vec4i->y);
			default:
				RE_ASSERT(false, "Variant type not convertable to glm::ivec2!");
				return glm::ivec2(0);
		}
	}

	Variant::operator glm::vec3() const {
		switch (m_Type) {
			case VECTOR2:
				return glm::vec3(m_Data._vec2->x, m_Data._vec2->y, 0.0f);
			case VECTOR2I:
				return glm::vec3(m_Data._vec2i->x, m_Data._vec2i->y, 0.0f);
			case VECTOR3:
				return *m_Data._vec3;
			case VECTOR3I:
				return glm::vec3(m_Data._vec3i->x, m_Data._vec3i->y, m_Data._vec3i->z);
			case VECTOR4:
				return glm::vec3(m_Data._vec4->x, m_Data._vec4->y, m_Data._vec4->z);
			case VECTOR4I:
				return glm::vec3(m_Data._vec4i->x, m_Data._vec4i->y, m_Data._vec4i->z);
			default:
				RE_ASSERT(false, "Variant type not convertable to glm::vec3!");
				return glm::vec3(0.0f);
		}
	}

	Variant::operator glm::ivec3() const {
		switch (m_Type) {
			case VECTOR2:
				return glm::ivec3(m_Data._vec2->x, m_Data._vec2->y, 0);
			case VECTOR2I:
				return glm::ivec3(m_Data._vec2i->x, m_Data._vec2i->y, 0);
			case VECTOR3:
				return glm::ivec3(m_Data._vec3->x, m_Data._vec3->y, m_Data._vec3->z);
			case VECTOR3I:
				return *m_Data._vec3i;
			case VECTOR4:
				return glm::ivec3(m_Data._vec4->x, m_Data._vec4->y, m_Data._vec4->z);
			case VECTOR4I:
				return glm::ivec3(m_Data._vec4i->x, m_Data._vec4i->y, m_Data._vec4i->z);
			default:
				RE_ASSERT(false, "Variant type not convertable to glm::ivec3!");
				return glm::ivec3(0);
		}
	}

	Variant::operator glm::vec4() const {
		switch (m_Type) {
			case VECTOR2:
				return glm::vec4(m_Data._vec2->x, m_Data._vec2->y, 0.0f, 0.0f);
			case VECTOR2I:
				return glm::vec4(m_Data._vec2i->x, m_Data._vec2i->y, 0.0f, 0.0f);
			case VECTOR3:
				return glm::vec4(m_Data._vec3->x, m_Data._vec3->y, m_Data._vec3->z, 0.0f);
			case VECTOR3I:
				return glm::vec4(m_Data._vec3i->x, m_Data._vec3i->y, m_Data._vec3i->z, 0.0f);
			case VECTOR4:
				return *m_Data._vec4;
			case VECTOR4I:
				return glm::vec4(m_Data._vec4i->x, m_Data._vec4i->y, m_Data._vec4i->z, m_Data._vec4i->w);
			default:
				RE_ASSERT(false, "Variant type not convertable to glm::vec4!");
				return glm::vec4(0.0f);
		}
	}

	Variant::operator glm::ivec4() const {
		switch (m_Type) {
			case VECTOR2:
				return glm::ivec4(m_Data._vec2->x, m_Data._vec2->y, 0, 0);
			case VECTOR2I:
				return glm::ivec4(m_Data._vec2i->x, m_Data._vec2i->y, 0, 0);
			case VECTOR3:
				return glm::ivec4(m_Data._vec3->x, m_Data._vec3->y, m_Data._vec3->z, 0);
			case VECTOR3I:
				return glm::ivec4(m_Data._vec3i->x, m_Data._vec3i->y, m_Data._vec3i->z, 0);
			case VECTOR4:
				return glm::ivec4(m_Data._vec4->x, m_Data._vec4->y, m_Data._vec4->z, m_Data._vec4->w);
			case VECTOR4I:
				return *m_Data._vec4i;
			default:
				RE_ASSERT(false, "Variant type not convertable to glm::ivec4!");
				return glm::ivec4(0);
		}
	}
}
