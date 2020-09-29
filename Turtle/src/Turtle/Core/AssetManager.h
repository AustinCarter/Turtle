#pragma once

#include <unordered_map>

#include "Turtle/Renderer/Texture.h"
#include "Turtle/Core/Core.h"

namespace Turtle {


	class AssetManager 
	{
	public:
		AssetManager() = default;

		static void Init();

		// TODO: Textures should eventually be registered with an internal asset id instead of their opengl renderer id
		static void RegisterTexture(uint32_t textureID, const Ref<Texture2D> texture);
		static const Ref<Texture2D>& GetTexture(uint32_t textureID);

	};
}