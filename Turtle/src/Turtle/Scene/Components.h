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

#include <fstream>


namespace Turtle {


	void InitComponentMeta();

	//NOTE: Tag and Transform component should be after any newly added  components
	enum class ComponentTypes { 
		SpriteRendererComponent = 0, CameraComponent = 1, NativeScriptComponent = 2, 
		ParticleSpawnerComponenet = 3, TileSetComponenet = 4,
		TagComponent, TransformComponent
	};

	struct TagComponent
	{
		std::string Tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag)
			: Tag(tag) {}

		void Serialize(std::ofstream outputStream);
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
		void Serialize(std::ofstream outputStream);
	};

	struct SpriteRendererComponent
	{
		glm::vec4 Color { 1.0f, 1.0f, 1.0f, 1.0f };
		uint32_t TextureID, RendererID; 
		bool Textured = false; 


		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const glm::vec4& color)
			: Color(color) {}
		SpriteRendererComponent(uint32_t texture, const glm::vec4& color)
			: TextureID(texture), Color(color), Textured(true) 
			{
				RendererID = AssetManager::GetTexture(texture).get()->GetRendererID();
			}

		void Serialize(std::ofstream outputStream);

	};

	struct CameraComponent
	{
		SceneCamera Camera;
		bool Primary = true; //TODO: POSSIBLY MOVE TO SCENE
		bool FixedAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;

		void Serialize(std::ofstream outputStream);
	};


	struct NativeScriptComponent
	{
		ScriptableEntity* Instance = nullptr;
		bool Bound = false; 

		ScriptableEntity*(*InstantiateScript)();
		void (*DestroyScript)(NativeScriptComponent*);

		template<typename T>
		void Bind()
		{
			InstantiateScript = []() { return static_cast<ScriptableEntity*>(new T()); };
			DestroyScript = [](NativeScriptComponent* nsc) {delete nsc->Instance; nsc->Instance = nullptr; };
			Bound = true; 
		}
		void Serialize(std::ofstream outputStream);
	};

	struct ParticleSpawnerComponenet
	{
		ParticleSpawner ParticleSpawner;
		ParticleProps Particle;
		uint32_t EmissionRate = 1;

		ParticleSpawnerComponenet()
		{
			Particle.ColorBegin = { 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
			Particle.ColorEnd = { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 0.9f };
			Particle.SizeBegin = 0.3f, Particle.SizeVariation = 0.2f, Particle.SizeEnd = 0.0f;
			Particle.LifeTime = 2.0f;
			Particle.Velocity = { 0.0f, 2.0f, 0.0f };
			Particle.VelocityVariation = { 3.0f, 2.0f, 0.0f };
			Particle.Position = { 0.0f, 0.0f, 0.0f };
		}
		ParticleSpawnerComponenet(const ParticleSpawnerComponenet&) = default;
		ParticleSpawnerComponenet(const ParticleProps& particle) :  Particle(particle) {}

		void Serialize(std::ofstream outputStream);

	};

	//NOTE: if an Entity has a tile set, it is expected to also have a grid and tile map at the moment
	struct TileSetComponenet
	{
		Ref<Texture2D> TileSet;
		uint32_t TileWidth = 64, TileHeight = 64;

		TileSetComponenet() = default;
		TileSetComponenet(const TileSetComponenet&) = default;

		void Serialize(std::ofstream outputStream);
	};

	struct GridComponent
	{
		//COULD MAKE A GLM::VEC2 FOR NON SQUARE GRID
		float GridSize = 1.0f;
		bool Active = false;

		GridComponent() = default;
		GridComponent(const GridComponent&) = default;

		void Serialize(std::ofstream outputStream);
	};

	struct TileMapComponent
	{
		//NOTE: ON SERIALIZATION COULD STORE THE SIZE OF THE VECTORS TO ALLOW FOR ONLY
		//ALLOCATION DURING DESERIALIZATION
		Ref<TileSetComponenet> TileSet;
		glm::vec4 Tint {1.0f, 1.0f, 1.0f, 1.0f};
		std::vector<glm::vec2> Positions;
		std::vector<Ref<SubTexture2D>> Textures;

		TileMapComponent() = default;
		TileMapComponent(const TileMapComponent&) = default;

		void Serialize(std::ofstream outputStream);
	};

	template<typename Component>
	static Component & get(entt::registry &registry, const entt::entity entity) 
	{
    	return registry.get<Component>(entity);
	}
}