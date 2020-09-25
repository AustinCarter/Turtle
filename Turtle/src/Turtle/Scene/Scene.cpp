#include "turtpch.h"
#include "Scene.h"

#include "Turtle/Renderer/Renderer2D.h"

#include <glm/glm.hpp>

#include "Turtle/Scene/Entity.h"

#include "Turtle/Core/TextureManager.h"

#include "Turtle/Scene/Components.h"


namespace Turtle {

	Scene::Scene()
	{

	}

	Scene::~Scene()
	{
		
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity = {  m_Registry.create(), this };
		entity.AddComponenet<TransformComponent>();
		auto& tag = entity.AddComponenet<TagComponent>();
		tag.Tag = name.empty() ? "Unnamed Entity" : name;

		return entity;
	}

	void Scene::OnUpdate(Timestep ts)
	{
		//update scripts
		{
			m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc)
			{
				//TODO: MOVE TO ONSCENEPLAY
				if(!nsc.Instance)
				{
					nsc.Instance = nsc.InstantiateScript();
					nsc.Instance->m_Entity = Entity{ entity, this };
					nsc.Instance->OnCreate();
				}

				nsc.Instance->OnUpdate(ts);
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
			
			auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
			for (auto entity : group)
			{
				auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

				if (sprite.Textured)
					Renderer2D::DrawQuad(transform, TextureManager::Get().GetTexture(sprite.TextureID), sprite.Color);
				else
					Renderer2D::DrawQuad(transform, sprite.Color);

			}	
			Renderer2D::EndScene();
		}
	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
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

}