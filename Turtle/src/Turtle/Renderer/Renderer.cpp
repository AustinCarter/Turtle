#include "Renderer.h"

#include "RenderCommand.h"
#include "Renderer2D.h"
#include "Turtle/Core/Log.h"

namespace Turtle {

Renderer::SceneData *Renderer::m_SceneData = new Renderer::SceneData;

void Renderer::Init() {
  TURT_CORE_INFO("initializing renderer");
  TURT_PROFILE_FUNCTION();

  RenderCommand::Init();
  TURT_CORE_INFO("\tinitializing Renderer2D");
  Renderer2D::Init();
  TURT_CORE_INFO("\ninitialized Renderer2D");
}

void Renderer::OnWindowResized(uint32_t width, uint32_t height) {
  RenderCommand::SetViewport(0, 0, width, height);
}

void Renderer::BeginScene(OrthographicCamera &camera) {
  m_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
}

void Renderer::EndScene() {}

void Renderer::Submit(const Ref<Shader> &shader,
                      const Ref<VertexArray> &vertexArray,
                      const glm::mat4 &transform) {
  shader->Bind();
  shader->SetMat4("u_ViewProjection", m_SceneData->ViewProjectionMatrix);
  shader->SetMat4("u_Transform", transform);

  vertexArray->Bind();
  RenderCommand::DrawIndexed(vertexArray, 0);
}

} // namespace Turtle
