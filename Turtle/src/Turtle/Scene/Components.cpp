#include "turtpch.h"
#include "Components.h"

#include <entt.hpp>

//Register types and functions to ECS (entt) meta system

namespace Turtle {
	void InitComponentMeta()
	{
		// entt::meta<TagComponent>().type()
		// 	.func<&TagComponent::Serialize>("Serialize"_hs);

		// entt::meta<TransformComponent>().type()
		// 	.func<&TransformComponent::Serialize>("Serialize"_hs);

		// entt::meta<SpriteRendererComponent>().type()
		// 	.func<&SpriteRendererComponent::Serialize>("Serialize"_hs);

		// entt::meta<CameraComponent>().type()
		// 	.func<&CameraComponent::Serialize>("Serialize"_hs);

		// entt::meta<NativeScriptComponent>().type()
		// 	.func<&NativeScriptComponent::Serialize>("Serialize"_hs);

		// entt::meta<ParticleSpawnerComponenet>().type()
		// 	.func<&ParticleSpawnerComponenet::Serialize>("Serialize"_hs);

		// entt::meta<TileSetComponenet>().type()
		// 	.func<&TileSetComponenet::Serialize>("Serialize"_hs);

		// entt::meta<GridComponent>().type()
		// 	.func<&GridComponent::Serialize>("Serialize"_hs);

		// entt::meta<TileMapComponent>().type()
		// 	.func<&TileMapComponent::Serialize>("Serialize"_hs);

		entt::meta<TagComponent>().type()
			.ctor<&get<TagComponent>>()
			.func<&TagComponent::Serialize>("Serialize"_hs);

		entt::meta<TransformComponent>().type()
			.ctor<&get<TransformComponent>>()
			.func<&TransformComponent::Serialize>("Serialize"_hs);

		entt::meta<SpriteRendererComponent>().type();

		entt::meta<CameraComponent>().type();

		entt::meta<NativeScriptComponent>().type();

		entt::meta<ParticleSpawnerComponenet>().type();

		entt::meta<TileSetComponenet>().type();

		entt::meta<GridComponent>().type();

		entt::meta<TileMapComponent>().type();
	}

}