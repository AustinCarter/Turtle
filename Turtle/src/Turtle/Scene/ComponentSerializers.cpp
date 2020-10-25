#include "turtpch.h"
#include "Components.h"

#include <yaml-cpp/yaml.h>

namespace YAML {

	template<>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec4>
	{
		static Node encode(const glm::vec4& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			return node;
		}

		static bool decode(const Node& node, glm::vec4& rhs)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec2>
	{
		static Node encode(const glm::vec2& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			return node;
		}

		static bool decode(const Node& node, glm::vec2& rhs)
		{
			if (!node.IsSequence() || node.size() != 2)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			return true;
		}
	};

}

YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec2& v)
{
	out << YAML::Flow;
	out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
	return out;
}

YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v)
{
	out << YAML::Flow;
	out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
	return out;
}

YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v)
{
	out << YAML::Flow;
	out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
	return out;
}

namespace Turtle {

	

	void TagComponent::Serialize(YAML::Emitter& out)
	{
		out << YAML::Key << "TagComponent";
		out << YAML::BeginMap; // TagComponent

		out << YAML::Key << "Tag" << YAML::Value << Tag;

		out << YAML::EndMap; // TagComponent
	}

	void TagComponent::Deserialize(YAML::Node& data, Entity entity)
	{
		// TURT_CORE_WARN("TAG");
		auto& comp = entity.GetComponent<TagComponent>();
		comp.Tag = data["Tag"].as<std::string>();
	}

	void TransformComponent::Serialize(YAML::Emitter& out)
	{
		out << YAML::Key << "TransformComponent";
		out << YAML::BeginMap; // TransformComponent

		out << YAML::Key << "Translation" << YAML::Value << Translation;
		out << YAML::Key << "Rotation" << YAML::Value << Rotation;
		out << YAML::Key << "Scale" << YAML::Value << Scale;

		out << YAML::EndMap; // TransformComponent
	}

	void TransformComponent::Deserialize(YAML::Node& data, Entity entity)
	{
		auto& comp = entity.GetComponent<TransformComponent>();
		comp.Translation = data["Translation"].as<glm::vec3>();
		comp.Rotation = data["Rotation"].as<glm::vec3>();
		comp.Scale = data["Scale"].as<glm::vec3>();
	}

	void SpriteRendererComponent::Serialize(YAML::Emitter& out)
	{
		out << YAML::Key << "SpriteRendererComponent";
		out << YAML::BeginMap; // SpriteRendererComponent

		out << YAML::Key << "Color" << YAML::Value << Color;
		out << YAML::Key << "Textured" << YAML::Value << Textured;
		if(Textured)
			out << YAML::Key << "Texture" << YAML::Value << Texture->GetPath();

		out << YAML::EndMap; // SpriteRendererComponent
	}

	void SpriteRendererComponent::Deserialize(YAML::Node& data, Entity entity)
	{

		auto& comp = entity.GetComponent<SpriteRendererComponent>();
		comp.Color = data["Color"].as<glm::vec4>();
		comp.Textured = data["Textured"].as<bool>();
		if(comp.Textured)
		{
			std::string path = data["Texture"].as<std::string>();
			comp.Texture = AssetManager::CreateTexture(path);
			comp.RendererID = comp.Texture->GetRendererID();
		}
	}

	void CameraComponent::Serialize(YAML::Emitter& out)
	{
		out << YAML::Key << "CameraComponent";
		out << YAML::BeginMap; // CameraComponent


		out << YAML::Key << "Camera" << YAML::Value;
		out << YAML::BeginMap; // Camera
		out << YAML::Key << "ProjectionType" << YAML::Value << (int)Camera.GetProjectionType();
		out << YAML::Key << "PerspectiveFOV" << YAML::Value << Camera.GetPerspectiveVerticalFOV();
		out << YAML::Key << "PerspectiveNear" << YAML::Value << Camera.GetPerspectiveNearClip();
		out << YAML::Key << "PerspectiveFar" << YAML::Value << Camera.GetPerspectiveFarClip();
		out << YAML::Key << "OrthographicSize" << YAML::Value << Camera.GetOrthographicSize();
		out << YAML::Key << "OrthographicNear" << YAML::Value << Camera.GetOrthographicNearClip();
		out << YAML::Key << "OrthographicFar" << YAML::Value << Camera.GetOrthographicFarClip();
		out << YAML::EndMap; // Camera

		out << YAML::Key << "Primary" << YAML::Value << Primary;
		out << YAML::Key << "FixedAspectRatio" << YAML::Value << FixedAspectRatio;

		out << YAML::EndMap; // CameraComponent
	}

	void CameraComponent::Deserialize(YAML::Node& data, Entity entity)
	{
		auto& comp = entity.GetComponent<CameraComponent>();

		auto& cameraProps = data["Camera"];
		comp.Camera.SetProjectionType((SceneCamera::ProjectionType)cameraProps["ProjectionType"].as<int>());

		comp.Camera.SetPerspectiveVerticalFOV(cameraProps["PerspectiveFOV"].as<float>());
		comp.Camera.SetPerspectiveNearClip(cameraProps["PerspectiveNear"].as<float>());
		comp.Camera.SetPerspectiveFarClip(cameraProps["PerspectiveFar"].as<float>());

		comp.Camera.SetOrthographicSize(cameraProps["OrthographicSize"].as<float>());
		comp.Camera.SetOrthographicNearClip(cameraProps["OrthographicNear"].as<float>());
		comp.Camera.SetOrthographicFarClip(cameraProps["OrthographicFar"].as<float>());

		comp.Primary = data["Primary"].as<bool>();
		comp.FixedAspectRatio = data["FixedAspectRatio"].as<bool>();
	}

	void NativeScriptComponent::Serialize(YAML::Emitter& out)
	{
	}

	void NativeScriptComponent::Deserialize(YAML::Node& data, Entity entity)
	{

	}

	void ParticleSpawnerComponent::Serialize(YAML::Emitter& out)
	{
		out << YAML::Key << "ParticleSpawnerComponent";
		out << YAML::BeginMap; // ParticleSpawnerComponent

		out << YAML::Key << "Particle" << YAML::Value;
		out << YAML::BeginMap; // Particle
		out << YAML::Key << "ColorBegin" << YAML::Value << Particle.ColorBegin;
		out << YAML::Key << "ColorEnd" << YAML::Value << Particle.ColorEnd;
		out << YAML::Key << "SizeBegin" << YAML::Value << Particle.SizeBegin;
		out << YAML::Key << "SizeEnd" << YAML::Value << Particle.SizeEnd;
		out << YAML::Key << "SizeVariation" << YAML::Value << Particle.SizeVariation;
		out << YAML::Key << "LifeTime" << YAML::Value << Particle.LifeTime;
		out << YAML::Key << "Velocity" << YAML::Value << Particle.Velocity;
		out << YAML::Key << "VelocityVariation" << YAML::Value << Particle.VelocityVariation;
		out << YAML::Key << "Position" << YAML::Value << Particle.Position;
		out << YAML::EndMap; // Particle

		out << YAML::Key << "EmissionRate" << YAML::Value << EmissionRate;

		out << YAML::EndMap; // ParticleSpawnerComponent
	}

	void ParticleSpawnerComponent::Deserialize(YAML::Node& data, Entity entity)
	{
		auto& comp = entity.GetComponent < ParticleSpawnerComponent >();

		auto& particleProps = data["Particle"];
		comp.Particle.ColorBegin = particleProps["ColorBegin"].as<glm::vec4>();
		comp.Particle.ColorEnd = particleProps["ColorEnd"].as<glm::vec4>();
		comp.Particle.SizeBegin = particleProps["SizeBegin"].as<float>();
		comp.Particle.SizeEnd = particleProps["SizeEnd"].as<float>();
		comp.Particle.SizeVariation = particleProps["SizeVariation"].as<float>();
		comp.Particle.LifeTime = particleProps["LifeTime"].as<float>();
		comp.Particle.Velocity = particleProps["Velocity"].as<glm::vec3>();
		comp.Particle.VelocityVariation = particleProps["VelocityVariation"].as<glm::vec3>();
		comp.Particle.Position = particleProps["Position"].as<glm::vec3>();

		comp.EmissionRate = data["EmissionRate"].as<uint32_t>();
	}

	void TileSetComponent::Serialize(YAML::Emitter& out)
	{
		out << YAML::Key << "TileSetComponent";
		out << YAML::BeginMap; // TileSetComponent

		out << YAML::Key << "TileWidth" << YAML::Value << TileWidth;
		out << YAML::Key << "TileHeight" << YAML::Value << TileHeight;
		out << YAML::Key << "TileSet" << YAML::Value << TileSet->GetPath();

		out << YAML::EndMap; // TileSetComponent
	}

	void TileSetComponent::Deserialize(YAML::Node& data, Entity entity)
	{
		auto& comp = entity.GetComponent < TileSetComponent >();
		comp.TileWidth = data["TileWidth"].as<uint32_t>();
		comp.TileHeight = data["TileHeight"].as<uint32_t>();
		std::string path = data["TileSet"].as<std::string>();
		if (!path.empty())
			comp.TileSet = AssetManager::CreateTexture(path);
	}

	void GridComponent::Serialize(YAML::Emitter& out)
	{
		out << YAML::Key << "GridComponent";
		out << YAML::BeginMap; // GridComponent

		out << YAML::Key << "GridSize" << YAML::Value << GridSize;
		out << YAML::Key << "Active" << YAML::Value << Active;

		out << YAML::EndMap; // GridComponent
	}

	void GridComponent::Deserialize(YAML::Node& data, Entity entity)
	{
		auto& comp = entity.GetComponent < GridComponent >();
		comp.GridSize = data["GridSize"].as<float>();
		comp.Active = data["Active"].as<bool>();
	}

	void TileMapComponent::Serialize(YAML::Emitter& out)
	{
		out << YAML::Key << "TileMapComponent";
		out << YAML::BeginMap; // TileMapComponent

		out << YAML::Key << "Tint" << YAML::Value << Tint;

		out << YAML::Key << "Positions";
		out << YAML::Flow;
		out << YAML::BeginSeq;
		for(auto& position : Positions)
		{
		  out << YAML::Value << position;
		}
		out << YAML::EndSeq;

		out << YAML::Key << "Textures";
		out << YAML::BeginMap; // Textures

	  	out << YAML::Key << "Texture" << YAML::Value << Textures[0]->GetTexture()->GetPath();
		out << YAML::Key << "TextureBounds";
		out << YAML::Flow;
		out << YAML::BeginSeq;
		for(auto& texture : Textures)
		{
	 	 	auto texCorods = texture->GetTexCoords();
			out << YAML::Flow;
		 	out << YAML::BeginSeq;
		 	out << texCorods[0] << texCorods[1] << texCorods[2] << texCorods[3];
		 	out << YAML::EndSeq;
		}
		out << YAML::EndSeq;
		out << YAML::EndMap; // Textures
		//out << YAML::Key << "Textures" << YAML::Value << Textures;

		out << YAML::EndMap; // TileMapComponent
	}

	void TileMapComponent::Deserialize(YAML::Node& data, Entity entity)
	{
		auto& comp = entity.GetComponent < TileMapComponent >();

		comp.Tint = data["Tint"].as<glm::vec4>();
		auto Positions = data["Positions"];
		for(YAML::Node position : Positions)
		{
			comp.Positions.emplace_back(position.as<glm::vec2>());
		}
		auto Textures = data["Textures"];
		std::string path = Textures["Texture"].as<std::string>();
		Ref<Texture2D> tex = AssetManager::CreateTexture(path);
		for(YAML::Node texture : Textures["TextureBounds"])
		{
			glm::vec2 bounds[4];
			int index = 0;
			for(YAML::Node bound : texture)
			{
				bounds[index] = bound.as<glm::vec2>();
				index++;
			}
			comp.Textures.emplace_back(SubTexture2D::CreateFromBounds(tex, bounds));
		}
	}
}