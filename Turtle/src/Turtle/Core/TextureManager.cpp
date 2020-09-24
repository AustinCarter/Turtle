#include "turtpch.h"
#include "TextureManager.h"

namespace Turtle {
	TextureManager* TextureManager::s_Instance = nullptr;

	TextureManager::TextureManager() 
	{
		TURT_CORE_ASSERT(!s_Instance, "TextureManager Already exixts.");
		s_Instance = this;
	}
}