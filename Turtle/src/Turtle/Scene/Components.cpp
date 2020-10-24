#include "turtpch.h"
#include "Components.h"

#include <entt.hpp>

//Register types and functions to ECS (entt) meta system

namespace Turtle {
	
	void InitComponentMeta()
	{
		entt::meta<TagComponent>().type()
			.ctor<&get<TagComponent>>()
			.func<&TagComponent::Serialize>("Serialize"_hs);

		entt::meta<TransformComponent>().type()
			.ctor<&get<TransformComponent>>()
			.func<&TransformComponent::Serialize>("Serialize"_hs);

		entt::meta<SpriteRendererComponent>().type()
			.ctor<&get<SpriteRendererComponent>>()
			.func<&SpriteRendererComponent::Serialize>("Serialize"_hs);

		entt::meta<CameraComponent>().type()
			.ctor<&get<CameraComponent>>()
			.func<&CameraComponent::Serialize>("Serialize"_hs);

		entt::meta<NativeScriptComponent>().type()
			.ctor<&get<NativeScriptComponent>>()
			.func<&NativeScriptComponent::Serialize>("Serialize"_hs);

		entt::meta<ParticleSpawnerComponenet>().type()
			.ctor<&get<ParticleSpawnerComponenet>>()
			.func<&ParticleSpawnerComponenet::Serialize>("Serialize"_hs);

		entt::meta<TileSetComponenet>().type()
			.ctor<&get<TileSetComponenet>>()
			.func<&TileSetComponenet::Serialize>("Serialize"_hs);

		entt::meta<GridComponent>().type()
			.ctor<&get<GridComponent>>()
			.func<&GridComponent::Serialize>("Serialize"_hs);

		entt::meta<TileMapComponent>().type()
			.ctor<&get<TileMapComponent>>()
			.func<&TileMapComponent::Serialize>("Serialize"_hs);
	}

}