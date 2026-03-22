#include "Scene.h"
#include "Components.h"
#include "Entity.h"
#include "ScriptableEntity.h"
#include "../Renderer/Renderer.h"
#include "../Renderer/RenderCommand.h"
#include "../Core/Log.h"

namespace Nullcore {

    Scene::Scene() {}
    Scene::~Scene() {}

    // ============================================================
    // Entity management
    // ============================================================
    Entity Scene::CreateEntity(const std::string& name) {
        return CreateEntityWithUUID(UUID(), name);
    }

    Entity Scene::CreateEntityWithUUID(UUID uuid, const std::string& name) {
        Entity entity = { m_Registry.create(), this };
        entity.AddComponent<IDComponent>(uuid);
        entity.AddComponent<TransformComponent>();
        auto& tag = entity.AddComponent<TagComponent>();
        tag.Tag = name.empty() ? "Entity" : name;
        return entity;
    }

    void Scene::DestroyEntity(Entity entity) {
        m_Registry.destroy(entity);
    }

    // ============================================================
    // Runtime update — runs all systems
    // ============================================================
    void Scene::OnUpdateRuntime(float deltaTime) {
        ScriptSystem(deltaTime);
        RenderSystem();
    }

    void Scene::OnUpdateEditor(float deltaTime) {
        // Editor mode: just render without scripts
        RenderSystem();
    }

    // ============================================================
    // Viewport resize — update primary camera aspect ratio
    // ============================================================
    void Scene::OnViewportResize(uint32_t width, uint32_t height) {
        m_ViewportWidth = width;
        m_ViewportHeight = height;

        // Update all non-fixed-aspect cameras
        auto view = m_Registry.view<CameraComponent>();
        for (auto entity : view) {
            auto& cc = view.get<CameraComponent>(entity);
            if (!cc.FixedAspectRatio) {
                cc.Camera.SetProjection(
                    45.0f,
                    (float)width / (float)height,
                    0.1f, 1000.0f
                );
            }
        }
    }

    // ============================================================
    // RenderSystem
    // Finds primary camera, then draws all MeshRendererComponents.
    // This is the core 3D ECS rendering loop.
    // ============================================================
    void Scene::RenderSystem() {
        // Find primary camera entity
        Entity cameraEntity = GetPrimaryCameraEntity();
        if (!cameraEntity) return;

        auto& cc = cameraEntity.GetComponent<CameraComponent>();
        auto& camTransform = cameraEntity.GetComponent<TransformComponent>();

        // Apply camera transform (position + rotation from TransformComponent)
        if (!cc.UseDirectView) {
            cc.Camera.SetPosition(camTransform.Translation);
            cc.Camera.SetRotation(camTransform.Rotation);
        }

        // Begin scene
        Renderer::BeginScene(cc.Camera);

        // Draw all visible mesh renderers
        auto view = m_Registry.view<TransformComponent, MeshRendererComponent>();
        for (auto entity : view) {
            auto& transform = view.get<TransformComponent>(entity);
            auto& mesh = view.get<MeshRendererComponent>(entity);

            if (!mesh.Visible) continue;
            if (!mesh.MeshShader || !mesh.VAO) continue;

            // Bind texture if present
            if (mesh.Albedo) {
                mesh.Albedo->Bind(0);
                mesh.MeshShader->Bind();
                mesh.MeshShader->SetInt("u_Texture", 0);
            }
            else {
                mesh.MeshShader->Bind();
            }

            // Pass tint color
            // SetVec4 not in Shader yet — pass via uniform location directly (see Renderer::Submit)

            Renderer::Submit(mesh.MeshShader, mesh.VAO, transform.GetTransform());
        }

        Renderer::EndScene();
    }

    // ============================================================
    // ScriptSystem
    // Instantiates and ticks all NativeScriptComponents
    // ============================================================
    void Scene::ScriptSystem(float deltaTime) {
        m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc) {
            if (!nsc.Instance) {
                nsc.Instance = nsc.InstantiateScript();
                nsc.Instance->m_Entity = Entity{ entity, this };
                nsc.Instance->OnCreate();
            }
            nsc.Instance->OnUpdate(deltaTime);
            });
    }

    // ============================================================
    // GetPrimaryCameraEntity
    // ============================================================
    Entity Scene::GetPrimaryCameraEntity() {
        auto view = m_Registry.view<CameraComponent>();
        for (auto entity : view) {
            const auto& camera = view.get<CameraComponent>(entity);
            if (camera.Primary)
                return Entity{ entity, this };
        }
        return {};
    }

} // namespace Nullcore