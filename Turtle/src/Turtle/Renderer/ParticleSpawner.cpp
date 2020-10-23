#include "turtpch.h"
#include "ParticleSpawner.h"

#include "Turtle/Core/Random.h"
#include "Turtle/Renderer/renderer2d.h"

#include <glm/gtc/constants.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/compatibility.hpp>

namespace Turtle {

	ParticleSpawner::ParticleSpawner()
	{
		m_ParticlePool.resize(1000);
	}

	ParticleSpawner::ParticleSpawner(uint32_t poolSize) :
		m_PoolIndex(poolSize-1)
	{
		m_ParticlePool.resize(poolSize);
	}

	void ParticleSpawner::OnUpdate(Timestep ts)
	{
		for (auto& particle : m_ParticlePool)
		{
			if (!particle.Active)
				continue;

			if (particle.LifeRemaining <= 0.0f)
			{
				particle.Active = false;
				continue;
			}

			particle.LifeRemaining -= ts;
			particle.Position += particle.Velocity * (float)ts;
			particle.Rotation += 0.01f * ts;
		}
	}

	void ParticleSpawner::OnRender()
	{
		
		// Renderer2D::BeginScene(camera.GetProjection(), glm::mat4(1.0f));
		for (auto& particle : m_ParticlePool)
		{
			if (!particle.Active)
				continue;

			// Fade away particles
			float life = particle.LifeRemaining / particle.LifeTime;
			glm::vec4 color = glm::lerp(particle.ColorEnd, particle.ColorBegin, life);

			float size = glm::lerp(particle.SizeEnd, particle.SizeBegin, life);
			
			// Render
			// 
			// NOTE: MIGHT NOT WANT TO DO A BEGIN SCENE AND END SCENE HERE, WILL MEAN PARTICLE GENERATOR WILL ALWAYS RESULT IN A FULL DRAW CALL
			Renderer2D::DrawRotatedQuad(particle.Position, glm::vec2{ size , size}, particle.Rotation, color);

		}
		// Renderer2D::EndScene();
	}

	void ParticleSpawner::Emit(const ParticleProps& particleProps)
	{
		Particle& particle = m_ParticlePool[m_PoolIndex];
		particle.Active = true;
		particle.Position = particleProps.Position;
		particle.Rotation = particleProps.RandomRotate ? Random::Float() * 2.0f * glm::pi<float>() : 0;
		// particle.Rotation = particleProps.Rotation * 2.0f * glm::pi<float>();

		// Velocity
		particle.Velocity = particleProps.Velocity;
		particle.Velocity.x += particleProps.VelocityVariation.x * (Random::Float() - 0.5f);
		particle.Velocity.y += particleProps.VelocityVariation.y * (Random::Float() - 0.5f);
		particle.Velocity.z += particleProps.VelocityVariation.z * (Random::Float() - 0.5f);

		// Color
		particle.ColorBegin = particleProps.ColorBegin;
		particle.ColorEnd = particleProps.ColorEnd;

		particle.LifeTime = particleProps.LifeTime;
		particle.LifeRemaining = particleProps.LifeTime;
		particle.SizeBegin = particleProps.SizeBegin + particleProps.SizeVariation * (Random::Float() - 0.5f);
		particle.SizeEnd = particleProps.SizeEnd;

		m_PoolIndex = m_PoolIndex > 0 ?  --m_PoolIndex : m_ParticlePool.size() - 1;
	}
}