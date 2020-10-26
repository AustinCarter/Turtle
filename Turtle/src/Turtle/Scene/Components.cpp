#include "turtpch.h"
#include "Components.h"

#include <entt.hpp>


namespace Turtle {
	
	//Register types and functions to ECS (entt) meta system
	void InitComponentMeta()
	{
		RegisterComponent<TagComponent>("TagComponent");
		RegisterComponent<TransformComponent>("TransformComponent");
		RegisterComponent<SpriteRendererComponent>("SpriteRendererComponent");
		RegisterComponent<CameraComponent>("CameraComponent");
		RegisterComponent<NativeScriptComponent>("NativeScriptComponent");
		RegisterComponent<ParticleSpawnerComponent>("ParticleSpawnerComponent");
		RegisterComponent<TileSetComponent>("TileSetComponent");
		RegisterComponent<GridComponent>("GridComponent");
		RegisterComponent<TileMapComponent>("TileMapComponent");

	}
} 