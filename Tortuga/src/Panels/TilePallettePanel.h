#pragma once

#include "Turtle/Renderer/Texture.h"

#include "glm/glm.hpp"

namespace Turtle {

	class TilePallettePanel 
	{
	public:
		bool Active() {return m_Active;}
		void Display();
		void Close();
		void Open();

		void SetTileSize(int tileWidth, int tileHeight);

		void SetTexture(Ref<Texture2D> texture) { m_Texture = texture; }

		int* GetSelection() {return m_Selected;}
	private:
		bool m_Active = false;
		Ref<Texture2D> m_Texture;
		int m_Selected[2] = {0, 0}, m_TileSize[2];


	};

}