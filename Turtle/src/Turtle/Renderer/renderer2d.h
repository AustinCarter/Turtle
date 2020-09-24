#pragma once

#include "Turtle/Renderer/OrthographicCamera.h"
#include "Turtle/Renderer/Camera.h"

#include "glm/glm.hpp"
#include "Turtle/Renderer/Texture.h"
#include "Turtle/Renderer/SubTexture2D.h"

namespace Turtle {
	class Renderer2D
	{
	public:
		struct RenderCamera
		{
			glm::mat4 Projection;
			glm::mat4 View;
		};

		static void Init();
		static void ShutDown();

		static void BeginScene(const Camera& camera, const glm::mat4& transform);
		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();
		static void Flush();

		//primatives
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& tint = glm::vec4(1.0f));
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& tint = glm::vec4(1.0f));
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<SubTexture2D>& subtexture, const glm::vec4& tint = glm::vec4(1.0f));
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture2D>& subtexture, const glm::vec4& tint = glm::vec4(1.0f));

/*		// TODO: this allows for rendering of a texture without having the ref to it, however it does not allow for subtextures to be communicated effectively. Furthermore, if more information is ever needed for rendering, then communicating that information will become more difficult. One solution is to create a texture/asset manager that will allow for a texture/asset id to be used to return a reference to that item. That way, SCene can use the id from SpriteRendererComponent to obtain a ref to that texture for rendering (alternativly, the renderer could do this process, however the Renderer side of things should probably not be tasked with such a task)
		static void DrawQuad(const glm::mat4& transform, uint32_t textureID, const glm::vec4& tint = glm::vec4(1.0f));
		//-----------------------------------		*/
		static void DrawQuad(const glm::mat4& transform, const glm::vec4& color);
		static void DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, const glm::vec4& tint = glm::vec4(1.0f));
		static void DrawQuad(const glm::mat4& transform, const Ref<SubTexture2D>& subtexture, const glm::vec4& tint = glm::vec4(1.0f));

		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color);
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color);
		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, const glm::vec4& tint = glm::vec4(1.0f));
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, const glm::vec4& tint = glm::vec4(1.0f));
		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<SubTexture2D>& subtexture, const glm::vec4& tint = glm::vec4(1.0f));
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<SubTexture2D>& subtexture, const glm::vec4& tint = glm::vec4(1.0f));
		

		struct Statistics
		{
			uint32_t DrawCalls = 0;
			uint32_t QuadCount = 0;

			uint32_t GetTotalVertexCount() { return QuadCount * 4; }
			uint32_t GetTotalIndexCount() { return QuadCount * 6; }
		};
		static Statistics GetStats();
		static void ResetStats();
		
	private:
		static void FlushAndReset();

	};
}