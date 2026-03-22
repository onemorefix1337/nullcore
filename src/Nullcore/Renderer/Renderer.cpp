#include "Renderer.h"
#include <glm/gtc/type_ptr.hpp>

namespace Nullcore {

    std::unique_ptr<Renderer::SceneData> Renderer::s_SceneData = std::make_unique<Renderer::SceneData>();

    void Renderer::Init() {
        RenderCommand::Init();
    }

    void Renderer::Shutdown() {
    }

    void Renderer::OnWindowResize(uint32_t width, uint32_t height) {
        RenderCommand::SetViewport(0, 0, width, height);
    }

    void Renderer::BeginScene(Camera& camera) {
        s_SceneData->ViewProjectionMatrix = camera.GetProjection();
        if (auto* ortho = dynamic_cast<OrthographicCamera*>(&camera)) {
            s_SceneData->ViewProjectionMatrix = ortho->GetViewProjectionMatrix();
        }
        else if (auto* persp = dynamic_cast<PerspectiveCamera*>(&camera)) {
            s_SceneData->ViewProjectionMatrix = persp->GetViewProjectionMatrix();
        }
    }

    void Renderer::EndScene() {
    }

    void Renderer::Submit(const std::shared_ptr<Shader>& shader,
        const std::shared_ptr<VertexArray>& vertexArray,
        const glm::mat4& transform) {
        shader->Bind();

        // устанавливаем uniforms
        int vpLoc = glGetUniformLocation(shader->GetRendererID(), "u_ViewProjection");
        if (vpLoc != -1)
            glUniformMatrix4fv(vpLoc, 1, GL_FALSE, glm::value_ptr(s_SceneData->ViewProjectionMatrix));

        int modelLoc = glGetUniformLocation(shader->GetRendererID(), "u_Transform");
        if (modelLoc != -1)
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(transform));

        vertexArray->Bind();

        if (vertexArray->GetIndexBuffer())
            RenderCommand::DrawIndexed(vertexArray);
        else
            RenderCommand::DrawArrays(vertexArray, 36); // временно хардкод
    }

}