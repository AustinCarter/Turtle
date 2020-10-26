#include "turtpch.h"
#include "Scene.h"
 
#include "Turtle/Renderer/Renderer2D.h"

#include "Turtle/Scene/Entity.h"
#include "Turtle/Core/AssetManager.h"
#include "Turtle/Scene/Components.h"
#include "Turtle/Scene/ComponentConnections.cpp"

#include "Turtle/Core/Log.h"

#include <yaml-cpp/yaml.h>

#include <glm/glm.hpp>

#include <fstream>

#include <Turtle/Utils/PlatformUtils.h>


namespace Turtle {

	Scene::Scene()
	{
		m_Registry.on_construct<CameraComponent>().connect<&Scene::OnCameraConstruct>(*this);
		m_Registry.on_construct<TileSetComponent>().connect<&OnTileSetConstruct>();
	}

	Scene::~Scene()
	{
		
	}

	Entity Scene::CreateEntity(const std::string& name)
	{

		TURT_PROFILE_FUNCTION();
		Entity entity = {  m_Registry.create(), this };
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Unnamed Entity" : name;

		return entity;
	}

	void Scene::DestroyEntity(Entity entity)
	{
		m_Registry.destroy(entity);
	}

	void Scene::OnUpdate(Timestep ts)
	{
		TURT_PROFILE_FUNCTION();
		//update scripts
		{
			m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc)
			{
				//TODO: MOVE TO ONSCENEPLAY
				if(nsc.Bound)
				{	
					if(!nsc.Instance)
					{
						nsc.Instance = nsc.InstantiateScript();
						nsc.Instance->m_Entity = Entity{ entity, this };
						nsc.Instance->OnCreate();
					}

					nsc.Instance->OnUpdate(ts);
				}
				
			});
		}


		//Render sprites
		glm::mat4 cameraTransform;
		Camera* primaryCamera = nullptr;
		{
			auto view = m_Registry.view<TransformComponent, CameraComponent>();
			for (auto entity : view)
			{
				auto [transform, camera]= view.get<TransformComponent, CameraComponent>(entity);

				if(camera.Primary)
				{
					primaryCamera = &camera.Camera;
					cameraTransform = transform.GetTransform();
					break;
				} 
			}
		}

		if (primaryCamera)
		{
			Renderer2D::BeginScene(primaryCamera->GetProjection(), cameraTransform);
			
			{
				auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
				for (auto entity : group)
				{
					auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

					if (sprite.Textured)
						Renderer2D::DrawQuad(transform.GetTransform(),sprite.Texture, sprite.Color);
					else
						Renderer2D::DrawQuad(transform.GetTransform(), sprite.Color);

				}
			}

			{
				auto view = m_Registry.view<TransformComponent, ParticleSpawnerComponent>();

				for (auto entity : view)
				{
					auto [transform, spawner] = view.get<TransformComponent, ParticleSpawnerComponent>(entity);

					spawner.Particle.Position = transform.Translation;

					spawner.ParticleSpawner.OnUpdate(ts);
					spawner.ParticleSpawner.OnRender();
					for(int i = 0; i < spawner.EmissionRate; i++)
						spawner.ParticleSpawner.Emit(spawner.Particle);
					
				}
			}

			{
				auto view = m_Registry.view<GridComponent, TileMapComponent>();
				

				for (auto entity : view)
				{
					auto [grid, tileMap] = view.get<GridComponent, TileMapComponent>(entity);
					
					int index = 0;
					for( auto& pos : tileMap.Positions)
					{
						Renderer2D::DrawQuad(glm::vec2(pos.x, pos.y), glm::vec2(1.0f, 1.0f), tileMap.Textures[index], tileMap.Tint);
						index ++;
					}

					if(grid.Active && grid.GridSize > 0)
					{
						float orthoSize = ((SceneCamera*)primaryCamera)->GetOrthographicSize();
						float aspectRatio = ((SceneCamera*)primaryCamera)->GetAspectRatio();

						for(int x = -1; x <= orthoSize*aspectRatio/grid.GridSize; x++)
						{
							Renderer2D::DrawQuad(glm::vec2( ((x*grid.GridSize) + (cameraTransform[3][0]-fmod(cameraTransform[3][0], grid.GridSize)) - (((orthoSize * aspectRatio) / 2) - fmod((orthoSize * aspectRatio) / 2, grid.GridSize))), cameraTransform[3][1]), glm::vec2(.001f*orthoSize, orthoSize), glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
						}
						for(int y = -1; y <= orthoSize/grid.GridSize ; y++)
						{
							Renderer2D::DrawQuad(glm::vec2(cameraTransform[3][0],(y*grid.GridSize) + (cameraTransform[3][1]-fmod(cameraTransform[3][1], grid.GridSize)) - ((orthoSize/ 2) - fmod(orthoSize/ 2, grid.GridSize))), glm::vec2(orthoSize*aspectRatio, .0015f*orthoSize), glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
						}

						break;
					}

					//TileMapComponent tileMap = view.get<TileMapComponent>(entity);

				}
			}

			Renderer2D::EndScene();
		}
	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		
		TURT_PROFILE_FUNCTION();
		m_ViewportWidth = width;
		m_ViewportHeight = height;

		auto view = m_Registry.view<CameraComponent>();
		for(auto entity : view)
		{
			auto& cameraComponent = view.get<CameraComponent>(entity);
			if(!cameraComponent.FixedAspectRatio)
			{
				cameraComponent.Camera.SetViewportSize(width, height);
			}
		}
	}

	void Scene::OnCameraConstruct(entt::registry & registry, entt::entity entity)
	{
		auto& cameraComponent = registry.get<CameraComponent>(entity);
		cameraComponent.Camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);	
	}

	void Scene::SerializeScene(const std::string& filepath)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << "Untitled";
		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;

		 m_Registry.each([&](auto entityID)
		{
			Entity entity = { entityID, this };
			if (!entity)
				return;

			out << YAML::BeginMap; // Entity
			out << YAML::Key << "Entity" << YAML::Value << "12837192831273"; // TODO: Entity ID goes here

			m_Registry.visit(entityID, [&](const auto component) {
				auto type = entt::resolve_type(component);
				auto instance = type.ctor<decltype(m_Registry), entt::entity>().invoke(std::ref(m_Registry), entityID);
				type.func("Serialize"_hs).invoke(instance, std::ref(out));
			});

			out << YAML::EndMap; // Entity
		});

	 	out << YAML::EndSeq;
		out << YAML::EndMap;

		std::ofstream fout(filepath);
		fout << out.c_str();
	}

	void Scene::DeserializeScene(const std::string& filepath)
	{
		std::ifstream stream(filepath);
		std::stringstream strStream;
		strStream << stream.rdbuf();

		YAML::Node data = YAML::Load(strStream.str());
		if (!data["Scene"])
			return;

		std::string sceneName = data["Scene"].as<std::string>();

		YAML::Node entities = data["Entities"];
		if(entities)
		{
			for (YAML::Node entity : entities)
			{
				uint64_t uuid = entity["Entity"].as<uint64_t>(); //TODO
				Entity deserializedEntity = CreateEntity();

				 for(auto component : entity)
				 {
					entt::hashed_string hs{ component.first.as<std::string>().c_str() };
					//auto comp = component.second.as<YAML::Node>();
					auto type = entt::resolve_id(hs);
					if(type)
					{
						auto instance = type.ctor<Entity>().invoke(deserializedEntity);
						type.func("Deserialize"_hs).invoke(instance, std::ref(component.second), deserializedEntity);
					}
				 }

			}
		}


	}

}