#pragma once 

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Turtle/Scene/SceneCamera.h"
#include "Turtle/Scene/ScriptableEntity.h"
#include "Turtle/Renderer/Texture.h"
#include "Turtle/Renderer/SubTexture2D.h"

#include "Turtle/Core/AssetManager.h"

#include "Turtle/Renderer/ParticleSpawner.h"

#include "Turtle/Scripting/LuaScript.h"

#include "Turtle/Audio/AudioPlayer.h"

//#include <entt.hpp>

#include <fstream>

#include <yaml-cpp/yaml.h>


namespace Turtle {
	struct TagComponent
	{
		std::string Tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag)
			: Tag(tag) {}

		void Serialize(YAML::Emitter& out);
		void Deserialize(YAML::Node& out, Entity entity);
		void DrawUI(Entity entity);
	};

	struct TransformComponent
	{
		glm::vec3 Translation = { 0.0f, 0.0f, 0.0f};
		glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f};
		glm::vec3 Scale = { 1.0f, 1.0f, 1.0f};

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::vec3& translation)
			: Translation(translation) {}

		glm::mat4 GetTransform() const
		{
			glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), Rotation.x, {1, 0, 0})
				* glm::rotate(glm::mat4(1.0f), Rotation.y, {0, 1, 0})
				* glm::rotate(glm::mat4(1.0f), Rotation.z, {0, 0, 1});
			return glm::translate(glm::mat4(1.0f), Translation) 
				* rotation 
				* glm::scale(glm::mat4(1.0f), Scale);
		}
		void Serialize(YAML::Emitter& out);
		void Deserialize(YAML::Node& out, Entity entity);
		void DrawUI(Entity entity);
	};

	struct SpriteRendererComponent
	{
		glm::vec4 Color { 1.0f, 1.0f, 1.0f, 1.0f };
		Ref<Texture2D> Texture;
		uint32_t RendererID; 
		bool Textured = false; 


		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const glm::vec4& color)
			: Color(color) {}
		SpriteRendererComponent(Ref<Texture2D> texture, const glm::vec4& color)
			: Texture(texture), Color(color), Textured(true) 
			{
				RendererID = texture->GetRendererID();
			}

		void Serialize(YAML::Emitter& out);
		void Deserialize(YAML::Node& out, Entity entity);
		void DrawUI(Entity entity);

	};

	struct CameraComponent
	{
		SceneCamera Camera;
		bool Primary = true; //TODO: POSSIBLY MOVE TO SCENE
		bool FixedAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;

		void Serialize(YAML::Emitter& out);
		void Deserialize(YAML::Node& out, Entity entity);
		void DrawUI(Entity entity);
	};

	struct ScriptComponent
	{
		Ref<LuaScript> Script;

		ScriptComponent() = default;
		ScriptComponent(const ScriptComponent&) = default;

		void Serialize(YAML::Emitter& out);
		void Deserialize(YAML::Node& out, Entity entity);
		void DrawUI(Entity entity);

	};

	// TODO: figure out how to do (de)serialization to get decoder attached to correct device (could be done with assetmanger and hashing)
	struct AudioSourceComponent
	{
		Ref<AudioPlayer> Player;
		Ref<AudioDecoder> Sound; 

		AudioSourceComponent() = default;
		AudioSourceComponent(const AudioSourceComponent&) = default;

		void Serialize(YAML::Emitter& out);
		void Deserialize(YAML::Node& out, Entity entity);
		void DrawUI(Entity entity);
	};


	// struct NativeScriptComponent
	// {
	// 	ScriptableEntity* Instance = nullptr;
	// 	bool Bound = false; 

	// 	ScriptableEntity*(*InstantiateScript)();
	// 	void (*DestroyScript)(NativeScriptComponent*);

	// 	template<typename T>
	// 	void Bind()
	// 	{
	// 		InstantiateScript = []() { return static_cast<ScriptableEntity*>(new T()); };
	// 		DestroyScript = [](NativeScriptComponent* nsc) {delete nsc->Instance; nsc->Instance = nullptr; };
	// 		Bound = true; 
	// 	}
	// 	void Serialize(YAML::Emitter& out);
	// 	void Deserialize(YAML::Node& out, Entity entity);
	// 	void DrawUI(Entity entity);
	// };

	struct ParticleSpawnerComponent
	{
		ParticleSpawner ParticleSpawner;
		ParticleProps Particle;
		unsigned int EmissionRate = 1;

		ParticleSpawnerComponent()
		{
			Particle.ColorBegin = { 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
			Particle.ColorEnd = { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 0.9f };
			Particle.SizeBegin = 0.3f, Particle.SizeVariation = 0.2f, Particle.SizeEnd = 0.0f;
			Particle.LifeTime = 2.0f;
			Particle.Velocity = { 0.0f, 2.0f, 0.0f };
			Particle.VelocityVariation = { 3.0f, 2.0f, 0.0f };
			Particle.Position = { 0.0f, 0.0f, 0.0f };
		}
		ParticleSpawnerComponent(const ParticleSpawnerComponent&) = default;
		ParticleSpawnerComponent(const ParticleProps& particle) :  Particle(particle) {}

		void Serialize(YAML::Emitter& out);
		void Deserialize(YAML::Node& out, Entity entity);
		void DrawUI(Entity entity);

	};

	struct TileSetComponent
	{
		Ref<Texture2D> TileSet;
		uint32_t TileWidth = 64, TileHeight = 64;
		uint32_t SelectedX = 0, SelectedY = 0;
		bool DisplayPallette;

		TileSetComponent() = default;
		TileSetComponent(const TileSetComponent&) = default;

		void Serialize(YAML::Emitter& out);
		void Deserialize(YAML::Node& out, Entity entity);
		void DrawUI(Entity entity);
	};

	struct GridComponent
	{
		//COULD MAKE A GLM::VEC2 FOR NON SQUARE GRID
		float GridSize = 1.0f;
		bool Active = true;

		GridComponent() = default;
		GridComponent(const GridComponent&) = default;

		void Serialize(YAML::Emitter& out);
		void Deserialize(YAML::Node& out, Entity entity);
		void DrawUI(Entity entity);
	};

	struct TileMapComponent
	{
		//NOTE: ON SERIALIZATION COULD STORE THE SIZE OF THE VECTORS TO ALLOW FOR ONLY
		//ALLOCATION DURING DESERIALIZATION
		// Ref<TileSetComponent> TileSet;
		glm::vec4 Tint {1.0f, 1.0f, 1.0f, 1.0f};
		std::vector<glm::vec2> Positions;
		std::vector<Ref<SubTexture2D>> Textures;

		TileMapComponent() = default;
		TileMapComponent(const TileMapComponent&) = default;

		void Serialize(YAML::Emitter& out);
		void Deserialize(YAML::Node& out, Entity entity);
		void DrawUI(Entity entity);
	};

	
	template<typename Component>
	static Component & Get(entt::registry &registry, const entt::entity entity) 
	{
    	return registry.get<Component>(entity);
	}

	template<typename Component>
	static Component& Add(Entity entity) 
	{
		if(entity.HasComponent<Component>())
			return entity.GetComponent<Component>();
		return entity.AddComponent<Component>();
	}
	
	//Register generic component information (Name, Get&Add constructor, Serialize, Deserialize, DrawUI )
	template<typename Component>
	static entt::meta_factory<Component> RegisterComponent(char const* name)
	{
		auto ret =  entt::meta<Component>().type(entt::hashed_string{ name })
			.prop("Name"_hs, name)
			.prop("Component"_hs)
			.ctor<>()
			.ctor<&Get<Component>>()
			.ctor<&Add<Component>>()
			.func<&Component::Serialize>("Serialize"_hs)
			.func<&Component::Deserialize>("Deserialize"_hs)
			.func<&Component::DrawUI>("DrawUI"_hs);

		std::string getterName = "Get__";
		getterName.append(name);

		char* getterName_c = (char*)malloc(128);
		strcpy_s(getterName_c, 128, getterName.c_str());
		
		entt::meta<Entity>()
			.func<&Entity::GetComponent<Component>, entt::as_ref_t>(entt::hashed_string{getterName_c})
				.prop("Name"_hs, (char const*)getterName_c);

		return ret;
	}

}