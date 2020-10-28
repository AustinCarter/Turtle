#pragma once

#include "Turtle/Scene/Components.h"

#include <entt.hpp>

namespace Turtle {
	// Add meta data for types that we want to use run time type reflection for
	static void InitMetaRegistry()
	{
		entt::meta<Entity>().type("Entity"_hs);

		//Register generic component information (Name, Get&Add constructor, Serialize, Deserialize, DrawUI )
		RegisterComponent<TagComponent>("TagComponent");
		RegisterComponent<TransformComponent>("TransformComponent");
		RegisterComponent<SpriteRendererComponent>("SpriteRendererComponent");
		RegisterComponent<CameraComponent>("CameraComponent");
		// RegisterComponent<NativeScriptComponent>("NativeScriptComponent");
		RegisterComponent<ScriptComponent>("ScriptComponent");
		RegisterComponent<ParticleSpawnerComponent>("ParticleSpawnerComponent");
		RegisterComponent<TileSetComponent>("TileSetComponent");
		RegisterComponent<GridComponent>("GridComponent");
		RegisterComponent<TileMapComponent>("TileMapComponent");


		//Register Component Data Memebers (used to expose to lua scripts)
		// emtt::meta<TransformComponent>();
		// emtt::meta<SpriteRendererComponent>();
		// emtt::meta<CameraComponent>();
		// emtt::meta<NativeScriptComponent>();
		// emtt::meta<ParticleSpawnerComponent>();
		// emtt::meta<TileSetComponent>();
		// emtt::meta<GridComponent>();
		// emtt::meta<TileMapComponent>();
	}
}