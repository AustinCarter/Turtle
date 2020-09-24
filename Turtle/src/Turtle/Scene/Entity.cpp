#include "turtpch.h"
#include "Entity.h"

namespace Turtle {

	Entity::Entity(entt::entity handle, Scene* scene)
		: m_EntityHandle(handle), m_Scene(scene)
	{
	}

}