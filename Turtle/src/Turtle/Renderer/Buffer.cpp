#include "turtpch.h"

#include "Buffer.h"
#include "Renderer.h"

#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Turtle {

	Ref<VertexBuffer> VertexBuffer::Create(uint32_t size)
	{
		switch(Renderer::GetAPI())
		{
			case RendererAPI::API::None: TURT_CORE_ASSERT(false, "RendererAPI::None is not currently supported."); return nullptr;
			case RendererAPI::API::OpenGL: return CreateRef<OpenGLVertexBuffer>(size);
		}
		TURT_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}

	Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch(Renderer::GetAPI())
		{
			case RendererAPI::API::None: TURT_CORE_ASSERT(false, "RendererAPI::None is not currently supported."); return nullptr;
			case RendererAPI::API::OpenGL: return CreateRef<OpenGLVertexBuffer>(vertices, size);
		}
		TURT_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}

	Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indicies, uint32_t count)
	{
		switch(Renderer::GetAPI())
		{
			case RendererAPI::API::None: TURT_CORE_ASSERT(false, "RendererAPI::None is not currently supported."); return nullptr;
			case RendererAPI::API::OpenGL: return CreateRef<OpenGLIndexBuffer>(indicies, count);
		}
		TURT_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}

}