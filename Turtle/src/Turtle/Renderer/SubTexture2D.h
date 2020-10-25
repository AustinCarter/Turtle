#pragma once

#include "Turtle/Renderer/Texture.h"

#include <glm/glm.hpp>

namespace Turtle {
	class SubTexture2D
	{
	public:
		SubTexture2D(const Ref<Texture2D>& texture, const glm::vec2& min, const glm::vec2 max);

		const Ref<Texture2D> GetTexture() const { return m_Texture; }
		const glm::vec2* GetTexCoords() const { return m_TexCoords; }

		//NOTE: I don't love this api
		static Ref<SubTexture2D> CreateFromCoords(const Ref<Texture2D>& texture, const glm::vec2& spritePos, const glm::vec2& cellSize, const glm::vec2& spriteSize = { 1, 1});
		static Ref<SubTexture2D> CreateFromBounds(const Ref<Texture2D>& texture, glm::vec2* bounds);
	private:
		Ref<Texture2D> m_Texture;
		glm::vec2 m_TexCoords[4];

	};
}