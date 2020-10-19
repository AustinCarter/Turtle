#include "turtpch.h"
#include "Scene.h"

#include "Turtle/Renderer/Renderer2D.h"

#include "Turtle/Scene/Entity.h"
#include "Turtle/Core/AssetManager.h"
#include "Turtle/Scene/Components.h"

#include <glm/glm.hpp>

#include <fstream>


namespace Turtle {

	Scene::Scene()
	{

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
		glm::mat4* cameraTransform = nullptr;
		Camera* primaryCamera = nullptr;
		{
			auto view = m_Registry.view<TransformComponent, CameraComponent>();
			for (auto entity : view)
			{
				auto [transform, camera]= view.get<TransformComponent, CameraComponent>(entity);

				if(camera.Primary)
				{
					primaryCamera = &camera.Camera;
					cameraTransform = &transform.Transform;
					break;
				}
			}
		}

		if (primaryCamera)
		{
			Renderer2D::BeginScene(primaryCamera->GetProjection(), *cameraTransform);
			
			{
				auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
				for (auto entity : group)
				{
					auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

					if (sprite.Textured)
						Renderer2D::DrawQuad(transform, AssetManager::GetTexture(sprite.TextureID), sprite.Color);
					else
						Renderer2D::DrawQuad(transform, sprite.Color);

				}
			}

			{
				auto view = m_Registry.view<TransformComponent, ParticleSpawnerComponenet>();

				for (auto entity : view)
				{
					auto [transform, spawner] = view.get<TransformComponent, ParticleSpawnerComponenet>(entity);

					spawner.Particle.Position = {transform.Transform[3][0], transform.Transform[3][1]};

					spawner.ParticleSpawner.OnUpdate(ts);
					spawner.ParticleSpawner.OnRender();
					for(int i = 0; i < spawner.EmissionRate; i++)
						spawner.ParticleSpawner.Emit(spawner.Particle);
					
				}
			}

			{
				auto view = m_Registry.view<TileSetComponenet>();
				

				for (auto entity : view)
				{
					TileSetComponenet tileSet = view.get<TileSetComponenet>(entity);
					if(tileSet.Active)
					{
						float orthoSize = ((SceneCamera*)primaryCamera)->GetOrthographicSize();
						float ppu = ((SceneCamera*)primaryCamera)->GetOrthographicPixelSize();
						float aspectRatio = ((SceneCamera*)primaryCamera)->GetAspectRatio();
						glm::vec2 unitsPerTile = glm::vec2(tileSet.TileWidth/ppu, tileSet.TileHeight/ppu);
						for(int x = -1; x <=( orthoSize * aspectRatio * ppu)/tileSet.TileWidth + 1; x++)
						{
							Renderer2D::DrawQuad(glm::vec2(((x*unitsPerTile.x) + ((*cameraTransform)[3][0] - fmod((*cameraTransform)[3][0], unitsPerTile.x))) - (orthoSize*aspectRatio)/2, (*cameraTransform)[3][1]), glm::vec2(.05f, orthoSize), glm::vec4(0.5f, 0.0f, 0.8f, 1.0f));
						}
						for(int y = -1; y < (orthoSize * ppu) / tileSet.TileHeight + 1 ; y++)
						{
							Renderer2D::DrawQuad(glm::vec2((*cameraTransform)[3][0], ((y*unitsPerTile.y) + ((*cameraTransform)[3][1] - fmod((*cameraTransform)[3][1], unitsPerTile.y))) - orthoSize/2), glm::vec2( orthoSize*aspectRatio, .05f), glm::vec4(0.5f, 0.0f, 0.8f, 1.0f));
							//Renderer2D::DrawQuad(glm::vec2());
						}
					}
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

	void Scene::OnCameraAdd(CameraComponent& cameraComponent)
	{
		cameraComponent.Camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);	
	}

	void Scene::SerializeScene()
	{
		std::ofstream outputStream;
		outputStream.open("SceneDump.json");
		outputStream << "{";
		m_Registry.each([&](auto entity)
		{
			//TODO: FIGURE OUT AN EFFICENT WAY TO VISIT EACH COMPONENT
			m_Registry.visit(entity, [&](const auto component) {
			});
		});
		outputStream << "}";
		outputStream.close();
	}

}