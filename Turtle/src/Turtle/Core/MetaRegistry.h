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

		entt::meta<ParticleProps>().type("ParticleProps"_hs)
			.prop("Name"_hs, "ParticleProps")
			.ctor<>()
			.data<&ParticleProps::Position, entt::as_ref_t>("Position"_hs)
			.data<&ParticleProps::Velocity, entt::as_ref_t>("Velocity"_hs)
			.data<&ParticleProps::VelocityVariation, entt::as_ref_t>("VelocityVariation"_hs)
			.data<&ParticleProps::ColorBegin, entt::as_ref_t>("ColorBegin"_hs)
			.data<&ParticleProps::ColorEnd, entt::as_ref_t>("ColorEnd"_hs)
			.data<&ParticleProps::SizeEnd, entt::as_ref_t>("SizeEnd"_hs)
			.data<&ParticleProps::SizeBegin, entt::as_ref_t>("SizeBegin"_hs)
			.data<&ParticleProps::SizeVariation, entt::as_ref_t>("SizeVariation"_hs)
			.data<&ParticleProps::LifeTime, entt::as_ref_t>("LifeTime"_hs);
		
		RegisterComponent<TagComponent>("TagComponent");


		RegisterComponent<TransformComponent>("TransformComponent")
			.data<&TransformComponent::Translation, entt::as_ref_t>("Translation"_hs)
		 	.data<&TransformComponent::Rotation, entt::as_ref_t>("Rotation"_hs)
		 	.data<&TransformComponent::Scale, entt::as_ref_t>("Scale"_hs);

		RegisterComponent<SpriteRendererComponent>("SpriteRendererComponent")
			.data<&SpriteRendererComponent::Color, entt::as_ref_t>("Color"_hs);

		RegisterComponent<CameraComponent>("CameraComponent")
			.data<&CameraComponent::Primary, entt::as_ref_t>("Primary"_hs);

		RegisterComponent<ScriptComponent>("ScriptComponent");

		RegisterComponent<AudioSourceComponent>("AudioSourceComponent")
			.data<&AudioSourceComponent::Sound, entt::as_ref_t>("Sound"_hs);

		RegisterComponent<ParticleSpawnerComponent>("ParticleSpawnerComponent")
			.data<&ParticleSpawnerComponent::EmissionRate>("EmissionRate"_hs)
			.data<&ParticleSpawnerComponent::Particle, entt::as_ref_t>("Particle"_hs);
			
		RegisterComponent<TileSetComponent>("TileSetComponent");

		RegisterComponent<GridComponent>("GridComponent");
		
		RegisterComponent<TileMapComponent>("TileMapComponent");
	}
}