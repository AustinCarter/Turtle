#include "turtpch.h"

#include "Turtle/Scene/Components.h"
#include "Turtle/Utils/PlatformUtils.h"


namespace Turtle {

	void OnTileSetConstruct(entt::registry & registry, entt::entity entity)
	{
		auto& tileSet = registry.get<TileSetComponent>(entity);
		std::string filepath = FileDialogs::OpenFile("*.png\0*.png\0");

		if(!filepath.empty())
			tileSet.TileSet = AssetManager::CreateTexture(filepath);
	}

	
}