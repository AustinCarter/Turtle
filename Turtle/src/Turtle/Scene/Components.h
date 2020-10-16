#pragma once 

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

#include "Turtle/Scene/SceneCamera.h"
#include "Turtle/Scene/ScriptableEntity.h"
#include "Turtle/Renderer/Texture.h"

#include "Turtle/Core/AssetManager.h"

#include "Turtle/Renderer/ParticleSpawner.h"

#include <fstream>


namespace Turtle {

	//NOTE: Tag and Transform component should be after any newly added  components
	enum class ComponentTypes { 
		SpriteRendererComponent = 0, CameraComponent = 1, NativeScriptComponent = 2,
		TagComponent = 3, TransformComponent = 4, ParticleSpawnerComponenet = 5
	};

	struct TagComponent
	{
		std::string Tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag)
			: Tag(tag) {}

		void Serialize(std::ofstream outputStream)
		{
			outputStream << "{";
			outputStream << "\t\"Tag\":\"" << Tag << "\"";
			outputStream << "}";
			outputStream.flush();
		}
	};

	struct TransformComponent
	{
		glm::mat4 Transform{ 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::mat4& transform)
			: Transform(transform) {}

		operator glm::mat4& () { return Transform; }
		operator const glm::mat4& () const { return Transform; }

		void Serialize(std::ofstream outputStream)
		{
			outputStream << "{";
			outputStream << "\t\"Transform\":\"" << glm::to_string(Transform) << "\"";
			outputStream << "}";
			outputStream.flush();
		}
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

		void Serialize(std::ofstream outputStream)
		{
			outputStream << "{";
			outputStream << "\t\"Color\":\"" << glm::to_string(Color) << "\"";
			outputStream << "\t\"TextureID\":\"" << TextureID << "\"";
			outputStream << "\t\"RendererID\":\"" << RendererID << "\"";
			outputStream << "\t\"TexturedID\":\"" << (Textured ? "True" : "False") << "\"";
			outputStream << "}";
			outputStream.flush();
		}

	};

	struct CameraComponent
	{
		SceneCamera Camera;
		bool Primary = true; //TODO: POSSIBLY MOVE TO SCENE
		bool FixedAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;

		void Serialize(std::ofstream outputStream)
		{
			outputStream << "{";
			outputStream << "\t\"Primary\":\"" <<  (Primary ? "True" : "False") << "\"";
			outputStream << "\t\"FixedAspectRatio\":\"" <<  (FixedAspectRatio ? "True" : "False") << "\"";
			outputStream << "}";
			outputStream.flush();
		}
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
	};

	struct ParticleSpawnerComponenet
	{
		ParticleSpawner ParticleSpawner;
		ParticleProps Particle;

		ParticleSpawnerComponenet() = default;
		ParticleSpawnerComponenet(const ParticleSpawnerComponenet&) = default;
		ParticleSpawnerComponenet(const ParticleProps& particle) :  Particle(particle) {}

	};
}