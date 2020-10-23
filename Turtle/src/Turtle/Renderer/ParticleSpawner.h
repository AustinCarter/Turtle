#pragma once

#include <glm/glm.hpp>

#include "Turtle/Core/Timestep.h"
#include "Turtle/Renderer/Camera.h"

namespace Turtle {
	struct ParticleProps
	{
		glm::vec3 Position;
		glm::vec3 Velocity, VelocityVariation;
		glm::vec4 ColorBegin, ColorEnd;
		float SizeBegin, SizeEnd, SizeVariation;
		// float Rotation = 0;
		float LifeTime = 1.0f;
		bool RandomRotate = true;
	};

	class ParticleSpawner
	{
	public:
		ParticleSpawner();
		ParticleSpawner(uint32_t poolSize);

		void OnUpdate(Timestep ts);
		void OnRender();

		void Emit(const ParticleProps& particleProps);
	private:
		struct Particle
		{
			glm::vec3 Position;
			glm::vec3 Velocity;
			glm::vec4 ColorBegin, ColorEnd;
			float Rotation = 0.0f;
			float SizeBegin, SizeEnd;

			float LifeTime = 1.0f;
			float LifeRemaining = 0.0f;

			bool Active = false;
		};
		std::vector<Particle> m_ParticlePool;
		uint32_t m_PoolIndex = 999;
	};
}