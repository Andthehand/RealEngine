#include "repch.h"
#include "ScriptGlue.h"
#include "ScriptEngine.h"

#include "RealEngine/Core/UUID.h"
#include "RealEngine/Core/KeyCodes.h"
#include "RealEngine/Core/Input.h"

#include "RealEngine/Scene/Scene.h"
#include "RealEngine/Scene/Entity.h"

#include "RealEngine/Physics/Physics2D.h"

#include "mono/metadata/object.h"
#include "mono/metadata/reflection.h"

#include "box2d/b2_body.h"

namespace RealEngine {
	static std::unordered_map<MonoType*, std::function<bool(Entity)>> s_EntityHasComponentFuncs;

#define RE_ADD_INTERNAL_CALL(Name) mono_add_internal_call("RealEngine.InternalCalls::" #Name, Name)

	static void NativeLog(MonoString* text) {
		char* cStr = mono_string_to_utf8(text);
		std::string str(cStr);
		mono_free(cStr);
		RE_WARN(str);
	}

	static void NativeLog_ULong(uint64_t parameter, MonoString* format) {
		char* cStr = mono_string_to_utf8(format);
		std::string str(cStr);
		mono_free(cStr);
		RE_WARN(str, parameter);
	}

	static void NativeLog_Vector2(glm::vec2* parameter, MonoString* format) {
		char* cStr = mono_string_to_utf8(format);
		std::string str(cStr);
		mono_free(cStr);
		RE_WARN(str, glm::to_string(*parameter));
	}

	static void NativeLog_Vector3(glm::vec3* parameter, MonoString* format) {
		char* cStr = mono_string_to_utf8(format);
		std::string str(cStr);
		mono_free(cStr);
		RE_WARN(str, glm::to_string(*parameter));
	}

	static void NativeLog_Vector4(glm::vec4* parameter, MonoString* format) {
		char* cStr = mono_string_to_utf8(format);
		std::string str(cStr);
		mono_free(cStr);
		RE_WARN(str, glm::to_string(*parameter));
	}

	static MonoObject* GetScriptInstance(UUID entityID) {
		return ScriptEngine::GetManagedInstance(entityID);
	}

	static bool Entity_HasComponent(UUID entityID, MonoReflectionType* componentType) {
		Scene* scene = ScriptEngine::GetSceneContext();
		RE_CORE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(entityID);
		RE_CORE_ASSERT(entity);

		MonoType* managedType = mono_reflection_type_get_type(componentType);
		RE_CORE_ASSERT(s_EntityHasComponentFuncs.find(managedType) != s_EntityHasComponentFuncs.end());
		return s_EntityHasComponentFuncs.at(managedType)(entity);
	}

	static uint64_t Entity_FindEntityByName(MonoString* name) {
		char* nameCStr = mono_string_to_utf8(name);

		Scene* scene = ScriptEngine::GetSceneContext();
		RE_CORE_ASSERT(scene);
		Entity entity = scene->FindEntityByName(nameCStr);
		mono_free(nameCStr);

		if (!entity)
			return 0;

		return entity.GetUUID();
	}

	static uint64_t Entity_InstantiateWithTranslation(uint64_t entityID, glm::vec3 translation) {
		Scene* scene = ScriptEngine::GetSceneContext();
		RE_CORE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(entityID);
		RE_CORE_ASSERT(entity);

		if (!entity)
			return 0;

		Entity duplicate = scene->DuplicateEntity(entity);
		duplicate.GetComponent<TransformComponent>().Translation = translation;
		
		return duplicate.GetUUID();
	}

	static void TransformComponent_GetTranslation(UUID entityID, glm::vec3* outTranslation) {
		Scene* scene = ScriptEngine::GetSceneContext();
		RE_CORE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(entityID);
		RE_CORE_ASSERT(entity);

		*outTranslation = entity.GetComponent<TransformComponent>().Translation;
	}

	static void TransformComponent_SetTranslation(UUID entityID, glm::vec3* translation) {
		Scene* scene = ScriptEngine::GetSceneContext();
		RE_CORE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(entityID);
		RE_CORE_ASSERT(entity);

		entity.GetComponent<TransformComponent>().Translation = *translation;
	}

	static void SpriteRendererComponent_GetColor(UUID entityID, glm::vec4* outColor) {
		Scene* scene = ScriptEngine::GetSceneContext();
		RE_CORE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(entityID);
		RE_CORE_ASSERT(entity);

		*outColor = entity.GetComponent<SpriteRendererComponent>().Color;
	}

	static void SpriteRendererComponent_SetColor(UUID entityID, glm::vec4* color) {
		Scene* scene = ScriptEngine::GetSceneContext();
		RE_CORE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(entityID);
		RE_CORE_ASSERT(entity);

		entity.GetComponent<SpriteRendererComponent>().Color = *color;
	}

	static float SpriteRendererComponent_GetTilingFactor(UUID entityID) {
		Scene* scene = ScriptEngine::GetSceneContext();
		RE_CORE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(entityID);
		RE_CORE_ASSERT(entity);

		return entity.GetComponent<SpriteRendererComponent>().TilingFactor;
	}

	static void SpriteRendererComponent_SetTilingFactor(UUID entityID, float tilingFactor) {
		Scene* scene = ScriptEngine::GetSceneContext();
		RE_CORE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(entityID);
		RE_CORE_ASSERT(entity);

		entity.GetComponent<SpriteRendererComponent>().TilingFactor = tilingFactor;
	}

	static void CircleRendererComponent_GetColor(UUID entityID, glm::vec4* outColor) {
		Scene* scene = ScriptEngine::GetSceneContext();
		RE_CORE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(entityID);
		RE_CORE_ASSERT(entity);

		*outColor = entity.GetComponent<CircleRendererComponent>().Color;
	}

	static void CircleRendererComponent_SetColor(UUID entityID, glm::vec4* color) {
		Scene* scene = ScriptEngine::GetSceneContext();
		RE_CORE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(entityID);
		RE_CORE_ASSERT(entity);

		entity.GetComponent<CircleRendererComponent>().Color = *color;
	}

	static float CircleRendererComponent_GetThickness(UUID entityID) {
		Scene* scene = ScriptEngine::GetSceneContext();
		RE_CORE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(entityID);
		RE_CORE_ASSERT(entity);

		return entity.GetComponent<CircleRendererComponent>().Thickness;
	}

	static void CircleRendererComponent_SetThickness(UUID entityID, float thickness) {
		Scene* scene = ScriptEngine::GetSceneContext();
		RE_CORE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(entityID);
		RE_CORE_ASSERT(entity);

		entity.GetComponent<CircleRendererComponent>().Thickness = thickness;
	}

	static float CircleRendererComponent_GetFade(UUID entityID) {
		Scene* scene = ScriptEngine::GetSceneContext();
		RE_CORE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(entityID);
		RE_CORE_ASSERT(entity);

		return entity.GetComponent<CircleRendererComponent>().Fade;
	}

	static void CircleRendererComponent_SetFade(UUID entityID, float fade) {
		Scene* scene = ScriptEngine::GetSceneContext();
		RE_CORE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(entityID);
		RE_CORE_ASSERT(entity);

		entity.GetComponent<CircleRendererComponent>().Fade = fade;
	}

	static void Rigidbody2DComponent_ApplyLinearImpulse(UUID entityID, glm::vec2* impulse, glm::vec2* point, bool wake) {
		Scene* scene = ScriptEngine::GetSceneContext();
		RE_CORE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(entityID);
		RE_CORE_ASSERT(entity);

		auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();
		b2Body* body = (b2Body*)rb2d.RuntimeBody;
		body->ApplyLinearImpulse(b2Vec2(impulse->x, impulse->y), b2Vec2(point->x, point->y), wake);
	}

	static void Rigidbody2DComponent_ApplyLinearImpulseToCenter(UUID entityID, glm::vec2* impulse, bool wake) {
		Scene* scene = ScriptEngine::GetSceneContext();
		RE_CORE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(entityID);
		RE_CORE_ASSERT(entity);

		auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();
		b2Body* body = (b2Body*)rb2d.RuntimeBody;
		body->ApplyLinearImpulseToCenter(b2Vec2(impulse->x, impulse->y), wake);
	}

	static void Rigidbody2DComponent_GetLinearVelocity(UUID entityID, glm::vec2* outLinearVelocity) {
		Scene* scene = ScriptEngine::GetSceneContext();
		RE_CORE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(entityID);
		RE_CORE_ASSERT(entity);

		auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();
		b2Body* body = (b2Body*)rb2d.RuntimeBody;
		const b2Vec2& linearVelocity = body->GetLinearVelocity();
		*outLinearVelocity = glm::vec2(linearVelocity.x, linearVelocity.y);
	}

	static Rigidbody2DComponent::BodyType Rigidbody2DComponent_GetType(UUID entityID) {
		Scene* scene = ScriptEngine::GetSceneContext();
		RE_CORE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(entityID);
		RE_CORE_ASSERT(entity);

		auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();
		b2Body* body = (b2Body*)rb2d.RuntimeBody;
		return Utils::Rigidbody2DTypeFromBox2DBody(body->GetType());
	}

	static void Rigidbody2DComponent_SetType(UUID entityID, Rigidbody2DComponent::BodyType bodyType) {
		Scene* scene = ScriptEngine::GetSceneContext();
		RE_CORE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(entityID);
		RE_CORE_ASSERT(entity);

		auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();
		b2Body* body = (b2Body*)rb2d.RuntimeBody;
		body->SetType(Utils::Rigidbody2DTypeToBox2DBody(bodyType));
	}

	static bool Input_IsKeyDown(KeyCode keycode) {
		return Input::IsKeyPressed(keycode);
	}

	template<typename... Component>
	static void RegisterComponent() {
		([]() {
			std::string_view typeName = typeid(Component).name();
			size_t pos = typeName.find_last_of(':');
			std::string_view structName = typeName.substr(pos + 1);
			std::string managedTypename = fmt::format("RealEngine.{}", structName);

			MonoType* managedType = mono_reflection_type_from_name(managedTypename.data(), ScriptEngine::GetCoreAssemblyImage());
			if (!managedType) {	
				RE_CORE_ERROR("Could not find component type {}", managedTypename);
				return;
			}
			s_EntityHasComponentFuncs[managedType] = [](Entity entity) { return entity.HasComponent<Component>(); };
		}(), ...);
	}

	template<typename... Component>
	static void RegisterComponent(ComponentGroup<Component...>) {
		RegisterComponent<Component...>();
	}

	void ScriptGlue::RegisterComponents() {
		s_EntityHasComponentFuncs.clear();
		RegisterComponent(AllComponents{});
	}

	void ScriptGlue::RegisterFunctions() {
		RE_ADD_INTERNAL_CALL(NativeLog);
		RE_ADD_INTERNAL_CALL(NativeLog_ULong);
		RE_ADD_INTERNAL_CALL(NativeLog_Vector2);
		RE_ADD_INTERNAL_CALL(NativeLog_Vector3);
		RE_ADD_INTERNAL_CALL(NativeLog_Vector4);

		RE_ADD_INTERNAL_CALL(GetScriptInstance);

		RE_ADD_INTERNAL_CALL(Entity_HasComponent);
		RE_ADD_INTERNAL_CALL(Entity_FindEntityByName);
		RE_ADD_INTERNAL_CALL(Entity_InstantiateWithTranslation);

		RE_ADD_INTERNAL_CALL(TransformComponent_GetTranslation);
		RE_ADD_INTERNAL_CALL(TransformComponent_SetTranslation);

		RE_ADD_INTERNAL_CALL(SpriteRendererComponent_GetColor);
		RE_ADD_INTERNAL_CALL(SpriteRendererComponent_SetColor);
		RE_ADD_INTERNAL_CALL(SpriteRendererComponent_GetTilingFactor);
		RE_ADD_INTERNAL_CALL(SpriteRendererComponent_SetTilingFactor);

		RE_ADD_INTERNAL_CALL(CircleRendererComponent_GetColor);
		RE_ADD_INTERNAL_CALL(CircleRendererComponent_SetColor);
		RE_ADD_INTERNAL_CALL(CircleRendererComponent_GetThickness);
		RE_ADD_INTERNAL_CALL(CircleRendererComponent_SetThickness);
		RE_ADD_INTERNAL_CALL(CircleRendererComponent_GetFade);
		RE_ADD_INTERNAL_CALL(CircleRendererComponent_SetFade);

		RE_ADD_INTERNAL_CALL(Rigidbody2DComponent_ApplyLinearImpulse);
		RE_ADD_INTERNAL_CALL(Rigidbody2DComponent_ApplyLinearImpulseToCenter);
		RE_ADD_INTERNAL_CALL(Rigidbody2DComponent_GetLinearVelocity);
		RE_ADD_INTERNAL_CALL(Rigidbody2DComponent_GetType);
		RE_ADD_INTERNAL_CALL(Rigidbody2DComponent_SetType);

		RE_ADD_INTERNAL_CALL(Input_IsKeyDown);
	}
}
