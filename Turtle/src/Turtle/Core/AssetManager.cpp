#include "turtpch.h"
#include "AssetMAnager.h"

#include "Turtle/Core/Hash.h"

namespace Turtle {

	struct AssetManagerData
	{
		std::unordered_map<uint32_t, Ref<Texture2D>> TextureMap;

		AssetManager::ResourceData Stats;
	};

	static AssetManagerData s_AssetData;

	void AssetManager::Init()
	{
	}

	Ref<Texture2D> AssetManager::CreateTexture(std::string path)
	{
		uint32_t textureID = MurmurHash(path.c_str(), path.size());
		Ref<Texture2D> texture;
		if(!Loaded(textureID))
		{
			texture = Texture2D::Create(path);
			s_AssetData.Stats.TexturesLoaded++;
			s_AssetData.TextureMap[textureID] = texture;
		}
		else
			texture = GetTexture(textureID);

		return texture;
	}

	Ref<Texture2D> AssetManager::GetTexture(uint32_t textureID)
	{
		TURT_CORE_ASSERT(Loaded(textureID), "Invalid assetID Loaded");
		return s_AssetData.TextureMap[textureID];
	}

	bool AssetManager::Loaded(uint32_t textureID)
	{
		return !(s_AssetData.TextureMap.find(textureID) == s_AssetData.TextureMap.end());
	}

	AssetManager::ResourceData AssetManager::GetStats()
	{
		return s_AssetData.Stats;
	}

	void AssetManager::ResetStats()
	{
		s_AssetData.Stats.TexturesLoaded = 0;	
	}
}