#pragma once

#include "Turtle/Renderer/Texture.h"

namespace Turtle {

	class TilePallettePanel 
	{
	public:
		bool Active() {return m_Active;}
		void Display();
		void Close();
		void Open();

		void SetTexture(Ref<Texture2D> texture) { m_Texture = texture; }
	private:
		bool m_Active = false;
		Ref<Texture2D> m_Texture;


	};

}