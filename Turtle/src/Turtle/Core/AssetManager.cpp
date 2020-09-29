#include "turtpch.h"
#include "AssetMAnager.h"

namespace Turtle {

	struct AssetManagerData
	{
		std::unordered_map<uint32_t, Ref<Texture2D>> TextureMap;
	};

	static AssetManagerData s_Data;

	void AssetManager::Init()
	{
	}

	void AssetManager::RegisterTexture(uint32_t textureID, const Ref<Texture2D> texture)
	{
		s_Data.TextureMap[textureID] = texture;
	}

	const Ref<Texture2D>& AssetManager::GetTexture(uint32_t textureID)
	{
		return s_Data.TextureMap[textureID];
	}
}