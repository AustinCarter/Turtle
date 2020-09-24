#include "turtpch.h"
#include "OpenGLVertexArray.h"

#include <glad/glad.h>

namespace Turtle {

	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float: 	return GL_FLOAT;
		case ShaderDataType::Float2: 	return GL_FLOAT;
		case ShaderDataType::Float3: 	return GL_FLOAT;
		case ShaderDataType::Float4: 	return GL_FLOAT;
		case ShaderDataType::Mat3: 		return GL_FLOAT;
		case ShaderDataType::Mat4: 		return GL_FLOAT;
		case ShaderDataType::Int: 		return GL_INT;
		case ShaderDataType::Int2: 		return GL_INT;
		case ShaderDataType::Int3: 		return GL_INT;
		case ShaderDataType::Int4: 		return GL_INT;
		case ShaderDataType::Bool: 		return GL_BOOL;
		}
	}


	OpenGLVertexArray::OpenGLVertexArray()
	{
		TURT_PROFILE_FUNCTION();
		
		glCreateVertexArrays(1, &m_RendererID);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		TURT_PROFILE_FUNCTION();
		
		glDeleteVertexArrays(1, &m_RendererID);
	}

	void OpenGLVertexArray::Bind() const
	{
		TURT_PROFILE_FUNCTION();
		
		glBindVertexArray(m_RendererID);
	}
	void OpenGLVertexArray::Unbind() const
	{
		TURT_PROFILE_FUNCTION();
		
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
	{
		TURT_PROFILE_FUNCTION();
		
		TURT_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout.");

		glBindVertexArray(m_RendererID);
		vertexBuffer->Bind();

		const auto& layout = vertexBuffer->GetLayout();

		uint32_t index = 0;
		for (const auto& element : layout)
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index,
				element.GetComponentCount(),
				ShaderDataTypeToOpenGLBaseType(element.Type),
				element.Normalized ? GL_TRUE : GL_FALSE,
				layout.GetStride(),
				(const void*)element.Offset);

			index++;
		}

		m_VertexBuffers.push_back(vertexBuffer);
	}
	void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
	{
		TURT_PROFILE_FUNCTION();
		
		glBindVertexArray(m_RendererID);
		indexBuffer->Bind();

		m_IndexBuffer = indexBuffer;
	}
}