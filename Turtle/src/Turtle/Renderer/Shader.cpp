#include "Shader.h"
#include "Renderer.h"
#include "RendererAPI.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace Turtle {

Ref<Shader> Shader::Create(const std::string &name,
                           const std::string &vertexSrc,
                           const std::string &fragmentSrc) {
  switch (Renderer::GetAPI()) {
  case RendererAPI::API::None:
    TURT_CORE_ASSERT(false, "RendererAPI::None is not currently supported.");
    return nullptr;
  case RendererAPI::API::OpenGL:
    return CreateRef<OpenGLShader>(name, vertexSrc, fragmentSrc);
  }
  TURT_CORE_ASSERT(false, "Unknown RendererAPI");
  return nullptr;
}

Ref<Shader> Shader::Create(const std::string &filepath) {
  switch (Renderer::GetAPI()) {
  case RendererAPI::API::None:
    TURT_CORE_ASSERT(false, "RendererAPI::None is not currently supported.");
    return nullptr;
  case RendererAPI::API::OpenGL:
    return CreateRef<OpenGLShader>(filepath);
  }
  TURT_CORE_ASSERT(false, "Unknown RendererAPI");
  return nullptr;
}

void ShaderLibrary::Add(const std::string &name, const Ref<Shader> &shader) {
  TURT_CORE_ASSERT(!Exists(name), "Shader - {0} - already existst!", name);
  m_Shaders[name] = shader;
}

void ShaderLibrary::Add(const Ref<Shader> &shader) {
  auto &name = shader->GetName();
  Add(name, shader);
}

Ref<Shader> ShaderLibrary::Load(const std::string &name,
                                const std::string &filepath) {
  auto shader = Shader::Create(filepath);
  Add(name, shader);
  return shader;
}

Ref<Shader> ShaderLibrary::Load(const std::string &filepath) {
  auto shader = Shader::Create(filepath);
  Add(shader);
  return shader;
}

Ref<Shader> ShaderLibrary::Get(const std::string &name) {
  TURT_CORE_ASSERT(Exists(name), "Shader - {0} - not found!", name);
  return m_Shaders[name];
}

bool ShaderLibrary::Exists(const std::string &name) const {
  return m_Shaders.find(name) != m_Shaders.end();
}
} // namespace Turtle
