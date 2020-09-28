#include "turtpch.h"
#include "TextureManager.h"

namespace Turtle {

	struct TextureManagerData
	{
		std::unordered_map<uint32_t, Ref<Texture2D>> TextureMap;
	};

	static TextureManagerData s_Data;


	void TextureManager::RegisterTexture(uint32_t textureID, const Ref<Texture2D> texture)
	{
		s_Data.TextureMap[textureID] = texture;
	}

	const Ref<Texture2D>& TextureManager::GetTexture(uint32_t textureID)
	{
		return s_Data.TextureMap[textureID];
	}
}