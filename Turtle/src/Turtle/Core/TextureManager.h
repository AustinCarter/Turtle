#pragma once

#include <unordered_map>

#include "Turtle/Renderer/Texture.h"
#include "Turtle/Core/Core.h"

//TODO: s_Instance currently raw pointer never getting deleted, maybe not the worst it lives entire runtime of app
namespace Turtle {

	class TextureManager 
	{
	public:
		TextureManager();

		void RegisterTexture(uint32_t textureID, const Ref<Texture2D> texture)
		{
			m_TextureMap[textureID] = texture;
		}

		const Ref<Texture2D>& GetTexture(uint32_t textureID)
		{
			return m_TextureMap[textureID];
		}

		inline static TextureManager& Get() {return *s_Instance;}
		
	private:
		std::unordered_map<uint32_t, Ref<Texture2D>> m_TextureMap;
		
 
		static TextureManager* s_Instance;
	};
}