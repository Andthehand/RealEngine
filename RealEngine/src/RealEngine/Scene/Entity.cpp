#include "repch.h"
#include "Entity.h"

namespace RealEngine {
	Entity::Entity(entt::entity handle, Scene* scene) 
	: m_EntityHandle(handle), m_Scene(scene) {
		
	}
}