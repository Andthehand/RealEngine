#pragma once

#include "RealEngine/Scene/Scene.h"
#include "RealEngine/Scene/Entity.h"

#include <filesystem>
#include <string>
#include <map>

extern "C" {
	typedef struct _MonoClass MonoClass;
	typedef struct _MonoObject MonoObject;
	typedef struct _MonoMethod MonoMethod;
	typedef struct _MonoAssembly MonoAssembly;
	typedef struct _MonoImage MonoImage;
	typedef struct _MonoClassField MonoClassField;
}

namespace RealEngine {
	enum class ScriptFieldType {
		None = 0,
		Float, Double,
		Bool, String, Char, Byte, Short, Int, Long,
		UByte, UShort, UInt, ULong,
		Vector2, Vector3, Vector4,
		Vector2Int, Vector3Int, Vector4Int,
		Color,
		Entity
	};

		
	struct ScriptField {
		ScriptFieldType Type;
		std::string Name;

		MonoClassField* ClassField;
	};

	// ScriptField + data storage
	struct ScriptFieldInstance {
		ScriptField Field;

		ScriptFieldInstance() {
			memset(m_Buffer, 0, sizeof(m_Buffer));
		}

		template<typename T>
		T GetValue() {
			static_assert(sizeof(T) <= 16, "Type too large!");
			return *(T*)m_Buffer;
		}

		template<typename T>
		void SetValue(T value) {
			static_assert(sizeof(T) <= 16, "Type too large!");
			memcpy(m_Buffer, &value, sizeof(T));
		}
	private:
		uint8_t m_Buffer[16];

		friend class ScriptEngine;
		friend class ScriptInstance;
	};

	using ScriptFieldMap = std::unordered_map<std::string, ScriptFieldInstance>;

	class ScriptClass {
	public:
		ScriptClass() = default;
		ScriptClass(const std::string& classNamespace, const std::string& className, bool isCore = false);

		MonoObject* Instantiate();
		MonoMethod* GetMethod(const std::string& name, int parameterCount);
		MonoObject* InvokeMethod(MonoObject* instance, MonoMethod* method, void** params = nullptr);

		MonoClassField* GetField(const char* name);
		const std::map<std::string, ScriptField>& GetFields() const { return m_Fields; }
	private:
		std::string m_ClassNamespace;
		std::string m_ClassName;

		std::map<std::string, ScriptField> m_Fields;

		MonoClass* m_MonoClass = nullptr;

		friend class ScriptEngine;
	};

	class ScriptInstance {
	public:
		ScriptInstance(Ref<ScriptClass> scriptClass, Entity entity);

		void InvokeOnCreate();
		void InvokeOnUpdate(float ts);

		void InvokeOnBeginContact();
		void InvokeOnEndContact();

		Ref<ScriptClass> GetScriptClass() { return m_ScriptClass; }

		template<typename T>
		T GetFieldValue(const std::string& name) {
			static_assert(sizeof(T) <= 16, "Type too large!");

			bool success = GetFieldValueInternal(name, s_FieldValueBuffer);
			if (!success)
				return T();

			return *(T*)s_FieldValueBuffer;
		}

		template<typename T>
		void SetFieldValue(const std::string& name, T value) {
			static_assert(sizeof(T) <= 16, "Type too large!");

			SetFieldValueInternal(name, &value);
		}
	private:
		MonoObject* GetManagedObject() { return m_Instance; }

		bool GetFieldValueInternal(const std::string& name, void* buffer);
		bool SetFieldValueInternal(const std::string& name, const void* value);
	private:
		Ref<ScriptClass> m_ScriptClass;

		MonoObject* m_Instance = nullptr;
		MonoMethod* m_Constructor = nullptr;

		MonoMethod* m_OnCreateMethod = nullptr;
		MonoMethod* m_OnUpdateMethod = nullptr;

		//Physics
		MonoMethod* m_OnBeginContact = nullptr;
		MonoMethod* m_OnEndContact = nullptr;

		inline static char s_FieldValueBuffer[16];

		friend class ScriptEngine;
		friend struct ScriptFieldInstance;
	};

	class ScriptEngine {
	public:
		static void Init();
		static void Shutdown();

		static bool LoadAssembly(const std::filesystem::path& filepath);
		static bool LoadAppAssembly(const std::filesystem::path& filepath);

		static void ReloadAssembly();	

		static void OnRuntimeStart(Scene* scene);
		static void OnRuntimeStop();

		static bool EntityClassExists(const std::string& fullClassName);

		static void OnCreateEntity(Entity entity);
		static void OnUpdateEntity(Entity entity, Timestep ts);

		//Physics
		static void OnBeginContactEntity(UUID entityUUID);
		static void OnEndContactEntity(UUID entityUUID);

		static Scene* GetSceneContext();
		static Ref<ScriptInstance> GetEntityScriptInstance(UUID entityID);

		static Ref<ScriptClass> GetEntityClass(const std::string& name);
		static std::unordered_map<std::string, Ref<ScriptClass>> GetEntityClasses();
		static ScriptFieldMap& GetScriptFieldMap(Entity entity);

		static MonoImage* GetCoreAssemblyImage();

		static MonoObject* GetManagedInstance(UUID uuid);
	private:
		static void InitMono();
		static void ShutdownMono();

		static MonoObject* InstantiateClass(MonoClass* monoClass);
		static void LoadAssemblyClasses();

		friend class ScriptClass;
		friend class ScriptGlue;
	};

	namespace Utils {
		inline const char* ScriptFieldTypeToString(ScriptFieldType fieldType) {
			switch (fieldType) {
				case ScriptFieldType::None:    return "None";
				case ScriptFieldType::Float:   return "Float";
				case ScriptFieldType::Double:  return "Double";
				case ScriptFieldType::Bool:    return "Bool";
				case ScriptFieldType::Char:    return "Char";
				case ScriptFieldType::String:  return "String";
				case ScriptFieldType::Byte:    return "Byte";
				case ScriptFieldType::Short:   return "Short";
				case ScriptFieldType::Int:     return "Int";
				case ScriptFieldType::Long:    return "Long";
				case ScriptFieldType::UByte:   return "UByte";
				case ScriptFieldType::UShort:  return "UShort";
				case ScriptFieldType::UInt:    return "UInt";
				case ScriptFieldType::ULong:   return "ULong";
				case ScriptFieldType::Vector2: return "Vector2";
				case ScriptFieldType::Vector3: return "Vector3";
				case ScriptFieldType::Vector4: return "Vector4";
				case ScriptFieldType::Vector2Int: return "Vector2Int";
				case ScriptFieldType::Vector3Int: return "Vector3Int";
				case ScriptFieldType::Vector4Int: return "Vector4Int";
				case ScriptFieldType::Entity:  return "Entity";
				case ScriptFieldType::Color:   return "Color";
			}
			RE_CORE_ASSERT(false, "Unknown ScriptFieldType");
			return "None";
		}

		inline ScriptFieldType ScriptFieldTypeFromString(std::string_view fieldType) {
			if (fieldType == "None")    return ScriptFieldType::None;
			if (fieldType == "Float")   return ScriptFieldType::Float;
			if (fieldType == "Double")  return ScriptFieldType::Double;
			if (fieldType == "Bool")    return ScriptFieldType::Bool;
			if (fieldType == "Char")    return ScriptFieldType::Char;
			if (fieldType == "String")  return ScriptFieldType::String;
			if (fieldType == "Byte")    return ScriptFieldType::Byte;
			if (fieldType == "Short")   return ScriptFieldType::Short;
			if (fieldType == "Int")     return ScriptFieldType::Int;
			if (fieldType == "Long")    return ScriptFieldType::Long;
			if (fieldType == "UByte")   return ScriptFieldType::UByte;
			if (fieldType == "UShort")  return ScriptFieldType::UShort;
			if (fieldType == "UInt")    return ScriptFieldType::UInt;
			if (fieldType == "ULong")   return ScriptFieldType::ULong;
			if (fieldType == "Vector2") return ScriptFieldType::Vector2;
			if (fieldType == "Vector3") return ScriptFieldType::Vector3;
			if (fieldType == "Vector4") return ScriptFieldType::Vector4;
			if (fieldType == "Vector2Int") return ScriptFieldType::Vector2Int;
			if (fieldType == "Vector3Int") return ScriptFieldType::Vector3Int;
			if (fieldType == "Vector4Int") return ScriptFieldType::Vector4Int;
			if (fieldType == "Entity")  return ScriptFieldType::Entity;
			if (fieldType == "Color")  return ScriptFieldType::Color;

			RE_CORE_ASSERT(false, "Unknown ScriptFieldType");
			return ScriptFieldType::None;
		}
	}
}
