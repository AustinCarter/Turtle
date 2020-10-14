#include "turtpch.h"
#include "Renderer2D.h"

#include "Turtle/Renderer/VertexArray.h"
#include "Turtle/Renderer/Shader.h"
#include "Turtle/Renderer/Buffer.h"
#include "Turtle/Renderer/RenderCommand.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Turtle {

	struct QuadVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
		float TexIndex;
		// TODO: maskid
	};

	struct Renderer2DData
	{
		// PER CALL MAXES
		static const uint32_t MaxQuads = 10000;
		static const uint32_t MaxVertices = MaxQuads * 4;
		static const uint32_t MaxIndices = MaxQuads * 6;
		static const uint32_t MaxTextureSlots = 32; //TODO: Render capabilities - poll gpu for texture slots

		Ref<VertexArray> QuadVertexArray;
		Ref<VertexBuffer> QuadVertexBuffer;
		Ref<Shader> TextureShader;
		Ref<Texture2D> WhiteTexture;

		uint32_t QuadIndexCount = 0;
		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;

		std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
		uint32_t TextureSlotIndex = 1; // index 0 is white texture 

		glm::vec4 QuadVertexPositions[4];
		
		Renderer2D::Statistics Stats; 
	};

	static Renderer2DData s_RenderData;

	void Renderer2D::Init()
	{
		TURT_PROFILE_FUNCTION();
		
		s_RenderData.QuadVertexArray = VertexArray::Create();

		s_RenderData.QuadVertexBuffer = VertexBuffer::Create(s_RenderData.MaxVertices * sizeof(QuadVertex));

		s_RenderData.QuadVertexBuffer->SetLayout( {
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color" },
			{ ShaderDataType::Float2, "a_TexCoord" },
			{ ShaderDataType::Float, "a_TexIndex" }
		});	
		s_RenderData.QuadVertexArray->AddVertexBuffer(s_RenderData.QuadVertexBuffer);

		s_RenderData.QuadVertexBufferBase = new QuadVertex[s_RenderData.MaxVertices];


		uint32_t* quadIndicies = new uint32_t[s_RenderData.MaxIndices];

		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_RenderData.MaxIndices; i+=6)
		{
			quadIndicies[i + 0] = offset + 0;
			quadIndicies[i + 1] = offset + 1;
			quadIndicies[i + 2] = offset + 2;

			quadIndicies[i + 3] = offset + 2;
			quadIndicies[i + 4] = offset + 3;
			quadIndicies[i + 5] = offset + 0;

			offset += 4;
		}

		Ref<IndexBuffer> quadIB = IndexBuffer::Create(quadIndicies, s_RenderData.MaxIndices);
		s_RenderData.QuadVertexArray->SetIndexBuffer(quadIB);
		//WARNING: TODO: ASSUMES THAT DATA IS IMMEDIATLY UPLOADED, COULD CAUSE DATA DELETION -> CRASH
		delete[] quadIndicies;

		s_RenderData.WhiteTexture = Texture2D::Create(1,1);
		uint32_t WhiteTextureData = 0xffffffff;
		s_RenderData.WhiteTexture->SetData(&WhiteTextureData, sizeof(uint32_t));

		int32_t samplers[ s_RenderData.MaxTextureSlots];
		for(uint32_t i = 0; i < s_RenderData.MaxTextureSlots; i++)
			samplers[i] = i;

		s_RenderData.TextureShader = Shader::Create("assets/shaders/Texture.glsl");
		s_RenderData.TextureShader->Bind();
		s_RenderData.TextureShader->SetIntArray("u_Textures", samplers, s_RenderData.MaxTextureSlots);

		s_RenderData.TextureSlots[0] = s_RenderData.WhiteTexture;

		s_RenderData.QuadVertexPositions[0] = {-0.5f, -0.5f, 0.0f, 1.0f};
		s_RenderData.QuadVertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f};
		s_RenderData.QuadVertexPositions[2] = { 0.5f,  0.5f, 0.0f, 1.0f};
		s_RenderData.QuadVertexPositions[3] = {-0.5f,  0.5f, 0.0f, 1.0f};
	}

	void Renderer2D::ShutDown()
	{
		TURT_PROFILE_FUNCTION();

		delete[] s_RenderData.QuadVertexBufferBase;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		TURT_PROFILE_FUNCTION();
		
		s_RenderData.TextureShader->Bind();
		s_RenderData.TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

		s_RenderData.QuadIndexCount = 0;
		s_RenderData.TextureSlotIndex = 1;
		s_RenderData.QuadVertexBufferPtr = s_RenderData.QuadVertexBufferBase;
	}


	void Renderer2D::BeginScene(const Camera& camera, const glm::mat4& transform )
	{
		TURT_PROFILE_FUNCTION();

		glm::mat4 viewProj = camera.GetProjection() * glm::inverse(transform);
		
		s_RenderData.TextureShader->Bind();
		s_RenderData.TextureShader->SetMat4("u_ViewProjection", viewProj);

		s_RenderData.QuadIndexCount = 0;
		s_RenderData.TextureSlotIndex = 1;
		s_RenderData.QuadVertexBufferPtr = s_RenderData.QuadVertexBufferBase;
	}

	void Renderer2D::EndScene()
	{
		TURT_PROFILE_FUNCTION();

		uint32_t dataSize = (uint32_t)((uint8_t*)s_RenderData.QuadVertexBufferPtr - (uint8_t*)s_RenderData.QuadVertexBufferBase);
		s_RenderData.QuadVertexBuffer->SetData(s_RenderData.QuadVertexBufferBase, dataSize);

		Flush();
	}

	void Renderer2D::Flush()
	{
		TURT_PROFILE_FUNCTION();

		if (s_RenderData.QuadIndexCount == 0)
			return; 

		for(uint32_t i = 0; i < s_RenderData.TextureSlotIndex; i++)
			s_RenderData.TextureSlots[i]->Bind(i);
		
		RenderCommand::DrawIndexed(s_RenderData.QuadVertexArray, s_RenderData.QuadIndexCount);

		s_RenderData.Stats.DrawCalls ++;
	}

	void Renderer2D::FlushAndReset()
	{
		TURT_PROFILE_FUNCTION();

		EndScene();

		s_RenderData.QuadIndexCount = 0;
		s_RenderData.TextureSlotIndex = 1;
		s_RenderData.QuadVertexBufferPtr = s_RenderData.QuadVertexBufferBase;
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({position.x, position.y, 0.0f}, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		TURT_PROFILE_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});

		DrawQuad(transform, color);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& tint)
	{
		DrawQuad({position.x, position.y, 0.0f}, size, texture, tint);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& tint)
	{
		TURT_PROFILE_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});

		DrawQuad(transform, texture, tint);
		
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<SubTexture2D>& subtexture, const glm::vec4& tint)
	{
		DrawQuad({position.x, position.y, 0.0f}, size, subtexture, tint);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture2D>& subtexture, const glm::vec4& tint)
	{
		TURT_PROFILE_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});

		DrawQuad(transform, subtexture, tint);

	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color)
	{
		TURT_PROFILE_FUNCTION();
		constexpr size_t quadVertexCount = 4;
		const float textureIndex = 0.0f; // white texture
		constexpr glm::vec2 textureCoords[] = { {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f} };

		if(s_RenderData.QuadIndexCount >= s_RenderData.MaxIndices)
			FlushAndReset();
		
		for(size_t i = 0; i < quadVertexCount; i++)
		{
			s_RenderData.QuadVertexBufferPtr->Position = transform * s_RenderData.QuadVertexPositions[i];
			s_RenderData.QuadVertexBufferPtr->Color = color;
			s_RenderData.QuadVertexBufferPtr->TexCoord = textureCoords[i];
			s_RenderData.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_RenderData.QuadVertexBufferPtr++;
		}

		s_RenderData.QuadIndexCount += 6;


		s_RenderData.Stats.QuadCount ++;
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, const glm::vec4& tint)
	{
		TURT_PROFILE_FUNCTION();

		constexpr size_t quadVertexCount = 4;

		if(s_RenderData.QuadIndexCount >= s_RenderData.MaxIndices)
			FlushAndReset();
		
		float textureIndex = 0.0f;
		constexpr glm::vec2 textureCoords[] = { {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f} };

		for (uint32_t i = 1; i < s_RenderData.TextureSlotIndex; i++)
		{
			if(*s_RenderData.TextureSlots[i].get() == *texture.get())
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			textureIndex = (float)s_RenderData.TextureSlotIndex;
			s_RenderData.TextureSlots[s_RenderData.TextureSlotIndex] = texture;
			s_RenderData.TextureSlotIndex++;
		}
		
		for(size_t i = 0; i < quadVertexCount; i++)
		{
			s_RenderData.QuadVertexBufferPtr->Position = transform * s_RenderData.QuadVertexPositions[i];
			s_RenderData.QuadVertexBufferPtr->Color = tint;
			s_RenderData.QuadVertexBufferPtr->TexCoord = textureCoords[i];
			s_RenderData.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_RenderData.QuadVertexBufferPtr++;
		}

		s_RenderData.QuadIndexCount += 6;


		s_RenderData.Stats.QuadCount ++;
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const Ref<SubTexture2D>& subtexture, const glm::vec4& tint)
	{
		TURT_PROFILE_FUNCTION();

		constexpr size_t quadVertexCount = 4;

		if(s_RenderData.QuadIndexCount >= s_RenderData.MaxIndices)
			FlushAndReset();
		
		float textureIndex = 0.0f;
		const glm::vec2* textureCoords = subtexture->GetTexCoords();
		const Ref<Texture2D> texture = subtexture->GetTexture();

		for (uint32_t i = 1; i < s_RenderData.TextureSlotIndex; i++)
		{
			if(*s_RenderData.TextureSlots[i].get() == *texture.get())
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			textureIndex = (float)s_RenderData.TextureSlotIndex;
			s_RenderData.TextureSlots[s_RenderData.TextureSlotIndex] = texture;
			s_RenderData.TextureSlotIndex++;
		}
		

		for(size_t i = 0; i < quadVertexCount; i++)
		{
			s_RenderData.QuadVertexBufferPtr->Position = transform * s_RenderData.QuadVertexPositions[i];
			s_RenderData.QuadVertexBufferPtr->Color = tint;
			s_RenderData.QuadVertexBufferPtr->TexCoord = textureCoords[i];
			s_RenderData.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_RenderData.QuadVertexBufferPtr++;
		}

		s_RenderData.QuadIndexCount += 6;


		s_RenderData.Stats.QuadCount ++;
	}


	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		DrawRotatedQuad({position.x, position.y, 0.0f}, size, rotation, color);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		TURT_PROFILE_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), rotation, {0.0f, 0.0f, 1.0f})
			* glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});

		DrawQuad(transform, color);		

		
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, const glm::vec4& tint)
	{
		DrawRotatedQuad({position.x, position.y, 0.0f}, size, rotation, texture, tint);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, const glm::vec4& tint)
	{
		TURT_PROFILE_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), rotation, {0.0f, 0.0f, 1.0f})
			* glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});

		DrawQuad(transform, texture, tint);

	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<SubTexture2D>& subtexture, const glm::vec4& tint)
	{
		DrawRotatedQuad({position.x, position.y, 0.0f}, size, rotation, subtexture, tint);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<SubTexture2D>& subtexture, const glm::vec4& tint)
	{
		TURT_PROFILE_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), rotation, {0.0f, 0.0f, 1.0f})
			* glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});

		DrawQuad(transform, subtexture, tint);

	}

	Renderer2D::Statistics Renderer2D::GetStats()
	{
		return s_RenderData.Stats;
	}

	void Renderer2D::ResetStats()
	{
		memset(&s_RenderData.Stats, 0, sizeof(Statistics));
	}
}