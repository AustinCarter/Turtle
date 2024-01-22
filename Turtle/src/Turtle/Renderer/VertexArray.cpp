#include "VertexArray.h"

#include "Platform/OpenGL/OpenGLVertexArray.h"

#include "Renderer.h"

namespace Turtle {

Ref<VertexArray> VertexArray::Create() {
  switch (Renderer::GetAPI()) {
  case RendererAPI::API::None:
    TURT_CORE_ASSERT(false, "RendererAPI::None is not currently supported.");
    return nullptr;
  case RendererAPI::API::OpenGL:
    return CreateRef<OpenGLVertexArray>();
  }
  TURT_CORE_ASSERT(false, "Unknown RendererAPI");
  return nullptr;
}
} // namespace Turtle
