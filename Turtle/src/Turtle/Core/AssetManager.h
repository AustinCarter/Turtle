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

		//TODO: at some point should probably have managers for specific assets that inheret from this asset manager class
		static Ref<Texture2D> CreateTexture(std::string path);
		static Ref<Texture2D> GetTexture(uint32_t textureID);

		static void Unload();


		struct ResourceData 
		{
			uint32_t TexturesLoaded = 0;
		};

		static ResourceData GetStats();
		static void ResetStats();
	private:
		static bool Loaded(uint32_t textureID);

	};
}