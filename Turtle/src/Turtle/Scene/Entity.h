#pragma once

#include "Turtle/Scene/Scene.h"
#include "Turtle/Core/Log.h"

#include <entt.hpp>

namespace Turtle {

	class Entity
	{
	public:
		Entity() = default;
		Entity(entt::entity handle, Scene* scene);
		Entity(const Entity& other) = default;

		Scene* GetScene() { return m_Scene; };

		template<typename T>
		static T& GetComponentFromHandle(Entity entity)
		{
			return entity.GetScene()->m_Registry.get<T>(entity);
		}

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			TURT_CORE_ASSERT(!HasComponent<T>(), "Entity already has component.");
			return m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
		}

		template<typename T>
		T& GetComponent()
		{
			TURT_CORE_ASSERT(HasComponent<T>(), "Entity doesn't have component.");
			T& result = m_Scene->m_Registry.get<T>(m_EntityHandle);
			return result;
		}

		template<typename T>
		bool HasComponent()
		{
			return m_Scene->m_Registry.has<T>(m_EntityHandle);
		}

		template<typename T>
		void RemoveComponent()
		{
			TURT_CORE_ASSERT(HasComponent<T>(), "Entity doesn't have component.");
			m_Scene->m_Registry.remove<T>(m_EntityHandle);
		}

		operator bool() const { return m_EntityHandle != entt::null; }
		operator uint32_t() const { return (uint32_t)m_EntityHandle; }
		operator entt::entity() const { return m_EntityHandle; }
		bool operator==(const Entity& other) const 
		{
			return m_EntityHandle == other.m_EntityHandle && m_Scene == other.m_Scene;
		} 
		bool operator!=(const Entity& other) const 
		{
			return !(*this==other);
		} 
	private:
		entt::entity m_EntityHandle{ entt::null };
		Scene* m_Scene = nullptr;
	};
}