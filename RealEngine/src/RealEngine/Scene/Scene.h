#pragma once
#include <entt.hpp>

#include "RealEngine/Core/UUID.h"
#include "RealEngine/Core/TimeStep.h"
#include "RealEngine/Renderer/EditorCamera.h"

class b2World;

namespace RealEngine {
	class Entity;

	class Scene {
	public:
		Scene();
		~Scene();

		static Ref<Scene> Copy(Ref<Scene> other);

		Entity CreateEntity(const std::string& name = std::string());
		Entity CreateEntityWithUUID(UUID uuid, const std::string& name = std::string());
		
		void DestroyEntity(Entity entity);

		void OnRuntimeStart();
		void OnRuntimeStop();

		void OnUpdateEditor(Timestep ts, EditorCamera& editorCamera);
		void OnUpdateRuntime(Timestep ts);
		void OnViewportResize(uint32_t width, uint32_t height);

		void DuplicateEntity(Entity entity);
	
		Entity GetPrimaryComponentEntity();
	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);
	private:
		entt::registry m_Registry;
		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

		b2World* m_PhysicsWorld = nullptr;

		friend class Entity;
		friend class SceneSerializer;
		friend class SceneHierarchyPanel;
	};
}
