#pragma once

#include <unordered_map>

#include "Turtle/Renderer/Texture.h"
#include "Turtle/Core/Core.h"

namespace Turtle {

	class TextureManager 
	{
	public:
		TextureManager() = default;

		static void RegisterTexture(uint32_t textureID, const Ref<Texture2D> texture);
		static const Ref<Texture2D>& GetTexture(uint32_t textureID);

	};
}