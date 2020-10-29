#pragma once

#include "Turtle/Scene/Components.h"

#include <entt.hpp>

namespace Turtle {
	// Add meta data for types that we want to use run time type reflection for
	static void InitMetaRegistry()
	{
		entt::meta<Entity>().type("Entity"_hs)
			.prop("Name"_hs, "Entity");

		entt::meta<glm::vec2>().type("vec2"_hs)
			.prop("Name"_hs, "vec2")
			.ctor<>()
			.data<&glm::vec2::x, entt::as_ref_t>("x"_hs)
			.data<&glm::vec2::y, entt::as_ref_t>("y"_hs);

		entt::meta<glm::vec3>().type("vec3"_hs)
			.prop("Name"_hs, "vec3")
			.ctor<>()
			.data<&glm::vec3::x, entt::as_ref_t>("x"_hs)
			.data<&glm::vec3::y, entt::as_ref_t>("y"_hs)
			.data<&glm::vec3::z, entt::as_ref_t>("z"_hs);

		entt::meta<glm::vec4>().type("vec4"_hs)
			.prop("Name"_hs, "vec4")
			.ctor<>()
			.data<&glm::vec4::x, entt::as_ref_t>("x"_hs)
			.data<&glm::vec4::y, entt::as_ref_t>("y"_hs)
			.data<&glm::vec4::z, entt::as_ref_t>("z"_hs)
			.data<&glm::vec4::w, entt::as_ref_t>("w"_hs);

		//Register generic component information (Name, Get&Add constructor, Serialize, Deserialize, DrawUI )
		
		
		RegisterComponent<TagComponent>("TagComponent");
		RegisterComponent<TransformComponent>("TransformComponent")
			.data<&TransformComponent::Translation, entt::as_ref_t>("Translation"_hs)
		 	.data<&TransformComponent::Rotation, entt::as_ref_t>("Rotation"_hs)
		 	.data<&TransformComponent::Scale, entt::as_ref_t>("Scale"_hs);
		RegisterComponent<SpriteRendererComponent>("SpriteRendererComponent");
		RegisterComponent<CameraComponent>("CameraComponent");
		//RegisterComponent<NativeScriptComponent>("NativeScriptComponent");
		RegisterComponent<ScriptComponent>("ScriptComponent");
		RegisterComponent<ParticleSpawnerComponent>("ParticleSpawnerComponent")
			.data<&ParticleSpawnerComponent::EmissionRate>("EmissionRate"_hs);
		RegisterComponent<TileSetComponent>("TileSetComponent");
		RegisterComponent<GridComponent>("GridComponent");
		RegisterComponent<TileMapComponent>("TileMapComponent");


		// entt::resolve<ParticleSpawnerComponent>()


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