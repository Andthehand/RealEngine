#include "repch.h"
#include "ScriptGlue.h"

#include "mono/metadata/object.h"

namespace RealEngine {

#define RE_ADD_INTERNAL_CALL(Name) mono_add_internal_call("RealEngine.InternalCalls::" #Name, Name)

	static void NativeLog(MonoString* string, int parameter) {
		char* cStr = mono_string_to_utf8(string);
		std::string str(cStr);
		mono_free(cStr);
		std::cout << str << ", " << parameter << std::endl;
	}

	static void NativeLog_Vector(glm::vec3* parameter, glm::vec3* outResult) {
		RE_CORE_WARN("Value: {0}", glm::to_string(*parameter));
		*outResult = glm::normalize(*parameter);
	}

	static float NativeLog_VectorDot(glm::vec3* parameter) {
		RE_CORE_WARN("Value: {0}", glm::to_string(*parameter));
		return glm::dot(*parameter, *parameter);
	}

	void ScriptGlue::RegisterFunctions() {
		RE_ADD_INTERNAL_CALL(NativeLog);
		RE_ADD_INTERNAL_CALL(NativeLog_Vector);
		RE_ADD_INTERNAL_CALL(NativeLog_VectorDot);
	}
}
