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

}

namespace Turtle {

	void TagComponent::Serialize(YAML::Emitter& out)
	{
		out << YAML::Key << "TagComponent";
		out << YAML::BeginMap; // TagComponent

		out << YAML::Key << "Tag" << YAML::Value << Tag;

		out << YAML::EndMap; // TagComponent
	}

	void TransformComponent::Serialize(YAML::Emitter& out)
	{
		out << YAML::Key << "TransformComponent";
		out << YAML::BeginMap; // TransformComponent

		// out << YAML::Key << "Translation" << YAML::Value << Translation;
		// out << YAML::Key << "Rotation" << YAML::Value << Rotation;
		// out << YAML::Key << "Scale" << YAML::Value << Scale;

		out << YAML::EndMap; // TransformComponent
	}

	void SpriteRendererComponent::Serialize(YAML::Emitter& out)
	{
		out << YAML::Key << "SpriteRendererComponent";
		out << YAML::BeginMap; // SpriteRendererComponent

		// out << YAML::Key << "Color" << YAML::Value << Color;

		out << YAML::EndMap; // SpriteRendererComponent
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

	void NativeScriptComponent::Serialize(YAML::Emitter& out)
	{
	}

	void ParticleSpawnerComponenet::Serialize(YAML::Emitter& out)
	{
		
	}

	void TileSetComponenet::Serialize(YAML::Emitter& out)
	{
		
	}

	void GridComponent::Serialize(YAML::Emitter& out)
	{
		
	}

	void TileMapComponent::Serialize(YAML::Emitter& out)
	{
		
	}
}