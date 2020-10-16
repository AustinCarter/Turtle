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
					spawner.ParticleSpawner.Emit(spawner.Particle);
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