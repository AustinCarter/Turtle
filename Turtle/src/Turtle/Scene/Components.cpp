#include "turtpch.h"
#include "Components.h"

#include <entt.hpp>


namespace Turtle {
	
	//Register types and functions to ECS (entt) meta system
	void InitComponentMeta()
	{
		entt::meta<TagComponent>().type("TagComponent"_hs)
			.prop("Name"_hs, "TagComponent")
			.ctor<&Get<TagComponent>>()
			.ctor<&Add<TagComponent>>()
			.func<&TagComponent::Serialize>("Serialize"_hs)
			.func<&TagComponent::Deserialize>("Deserialize"_hs);

		entt::meta<TransformComponent>().type("TransformComponent"_hs)
			.prop("Name"_hs, "TransformComponent")
			.ctor<&Get<TransformComponent>>()
			.ctor<&Add<TransformComponent>>()
			.func<&TransformComponent::Serialize>("Serialize"_hs)
			.func<&TransformComponent::Deserialize>("Deserialize"_hs);

		entt::meta<SpriteRendererComponent>().type("SpriteRendererComponent"_hs)
			.prop("Name"_hs, "SpriteRendererComponent")
			.ctor<&Get<SpriteRendererComponent>>()
			.ctor<&Add<SpriteRendererComponent>>()
			.func<&SpriteRendererComponent::Serialize>("Serialize"_hs)
			.func<&SpriteRendererComponent::Deserialize>("Deserialize"_hs);

		entt::meta<CameraComponent>().type("CameraComponent"_hs)
			.prop("Name"_hs, "CameraComponent")
			.ctor<&Get<CameraComponent>>()
			.ctor<&Add<CameraComponent>>()
			.func<&CameraComponent::Serialize>("Serialize"_hs)
			.func<&CameraComponent::Deserialize>("Deserialize"_hs);

		entt::meta<NativeScriptComponent>().type("NativeScriptComponent"_hs)
			.prop("Name"_hs, "NativeScriptComponent")
			.ctor<&Get<NativeScriptComponent>>()
			.ctor<&Add<NativeScriptComponent>>()
			.func<&NativeScriptComponent::Serialize>("Serialize"_hs)
			.func<&NativeScriptComponent::Deserialize>("Deserialize"_hs);

		entt::meta<ParticleSpawnerComponent>().type("ParticleSpawnerComponent"_hs)
			.prop("Name"_hs, "ParticleSpawnerComponent")
			.ctor<&Get<ParticleSpawnerComponent>>()
			.ctor<&Add<ParticleSpawnerComponent>>()
			.func<&ParticleSpawnerComponent::Serialize>("Serialize"_hs)
			.func<&ParticleSpawnerComponent::Deserialize>("Deserialize"_hs);

		entt::meta<TileSetComponent>().type("TileSetComponent"_hs)
			.prop("Name"_hs, "TileSetComponent")
			.ctor<&Get<TileSetComponent>>()
			.ctor<&Add<TileSetComponent>>()
			.func<&TileSetComponent::Serialize>("Serialize"_hs)
			.func<&TileSetComponent::Deserialize>("Deserialize"_hs);

		entt::meta<GridComponent>().type("GridComponent"_hs)
			.prop("Name"_hs, "GridComponent")
			.ctor<&Get<GridComponent>>()
			.ctor<&Add<GridComponent>>()
			.func<&GridComponent::Serialize>("Serialize"_hs)
			.func<&GridComponent::Deserialize>("Deserialize"_hs);

		entt::meta<TileMapComponent>().type("TileMapComponent"_hs)
			.prop("Name"_hs, "TileMapComponent")
			.ctor<&Get<TileMapComponent>>()
			.ctor<&Add<TileMapComponent>>()
			.func<&TileMapComponent::Serialize>("Serialize"_hs)
			.func<&TileMapComponent::Deserialize>("Deserialize"_hs);
	}

}