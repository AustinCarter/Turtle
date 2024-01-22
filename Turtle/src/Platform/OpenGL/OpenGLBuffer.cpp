#include "OpenGLBuffer.h"
#include "turtpch.h"

#include <glad/glad.h>

namespace Turtle {

////////////////////////////////////////////////////////
/****************** Vertex Buffer **********************/
////////////////////////////////////////////////////////
OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size) {
  TURT_PROFILE_FUNCTION();

  glGenBuffers(1, &m_RendererID);
  glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
  glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
}

OpenGLVertexBuffer::OpenGLVertexBuffer(float *vertices, uint32_t size) {
  TURT_PROFILE_FUNCTION();

  glGenBuffers(1, &m_RendererID);
  glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
  glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

OpenGLVertexBuffer::~OpenGLVertexBuffer() {
  TURT_PROFILE_FUNCTION();

  glDeleteBuffers(1, &m_RendererID);
}

void OpenGLVertexBuffer::Bind() const {
  TURT_PROFILE_FUNCTION();

  glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
}

void OpenGLVertexBuffer::Unbind() const {
  TURT_PROFILE_FUNCTION();

  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void OpenGLVertexBuffer::SetData(const void *data, uint32_t size) {
  TURT_PROFILE_FUNCTION();

  glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
  glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
}

////////////////////////////////////////////////////////
/****************** Index Buffer **********************/
////////////////////////////////////////////////////////
OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t *indicies, uint32_t count)
    : m_Count(count) {
  TURT_PROFILE_FUNCTION();

  glGenBuffers(1, &m_RendererID);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indicies,
               GL_STATIC_DRAW);
}

OpenGLIndexBuffer::~OpenGLIndexBuffer() {
  TURT_PROFILE_FUNCTION();

  glDeleteBuffers(1, &m_RendererID);
}

void OpenGLIndexBuffer::Bind() const {
  TURT_PROFILE_FUNCTION();

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
}

void OpenGLIndexBuffer::Unbind() const {
  TURT_PROFILE_FUNCTION();

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
} // namespace Turtle
