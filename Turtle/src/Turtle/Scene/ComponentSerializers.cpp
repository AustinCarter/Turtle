#include "turtpch.h"
#include "Components.h"

namespace Turtle {

	void TagComponent::Serialize(std::ofstream outputStream)
	{
		outputStream << "{";
		outputStream << "\t\"Tag\":\"" << Tag << "\"";
		outputStream << "}";
		outputStream.flush();
	}

	void TransformComponent::Serialize(std::ofstream outputStream)
	{
		outputStream << "{";
		outputStream << "\t\"Translation\":\"" << glm::to_string(Translation) << "\"";
		outputStream << "}";
		outputStream.flush();
	}

	void SpriteRendererComponent::Serialize(std::ofstream outputStream)
	{
		outputStream << "{";
		outputStream << "\t\"Color\":\"" << glm::to_string(Color) << "\"";
		outputStream << "\t\"TextureID\":\"" << TextureID << "\"";
		outputStream << "\t\"RendererID\":\"" << RendererID << "\"";
		outputStream << "\t\"TexturedID\":\"" << (Textured ? "True" : "False") << "\"";
		outputStream << "}";
		outputStream.flush();
	}

	void CameraComponent::Serialize(std::ofstream outputStream)
	{
		outputStream << "{";
		outputStream << "\t\"Primary\":\"" << (Primary ? "True" : "False") << "\"";
		outputStream << "\t\"FixedAspectRatio\":\"" << (FixedAspectRatio ? "True" : "False") << "\"";
		outputStream << "}";
		outputStream.flush();
	}

	void NativeScriptComponent::Serialize(std::ofstream outputStream)
	{
	}

	void ParticleSpawnerComponenet::Serialize(std::ofstream outputStream)
	{
		outputStream << "{";
		outputStream << "}";
		outputStream.flush();
	}

	void TileSetComponenet::Serialize(std::ofstream outputStream)
	{
		outputStream << "{";
		outputStream << "}";
		outputStream.flush();
	}

	void GridComponent::Serialize(std::ofstream outputStream)
	{
		outputStream << "{";
		outputStream << "}";
		outputStream.flush();
	}

	void TileMapComponent::Serialize(std::ofstream outputStream)
	{
		outputStream << "{";
		outputStream << "}";
		outputStream.flush();
	}
}