#include "Framebuffer.h"

#include "Platform/OpenGL/OpenGLFramebuffer.h"
#include "Turtle/Renderer/Renderer.h"

namespace Turtle {
Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification &spec) {
  switch (Renderer::GetAPI()) {
  case RendererAPI::API::None:
    TURT_CORE_ASSERT(false, "RendererAPI::None is not currently supported.");
    return nullptr;
  case RendererAPI::API::OpenGL:
    return CreateRef<OpenGLFramebuffer>(spec);
  }
  TURT_CORE_ASSERT(false, "Unknown RendererAPI");
  return nullptr;
}
} // namespace Turtle
