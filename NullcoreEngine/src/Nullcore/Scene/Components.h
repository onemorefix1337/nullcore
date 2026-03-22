#pragma once
#include "../Core/UUID.h"
#include "../Renderer/Shader.h"
#include "../Renderer/Texture.h"
#include "../Renderer/VertexArray.h"
#include "../Renderer/Camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <string>
#include <memory>
#include <vector>

namespace Nullcore {

    // ============================================================
    // IDComponent
    // ============================================================
    struct IDComponent {
        UUID ID;
        IDComponent() = default;
        IDComponent(const IDComponent&) = default;
        IDComponent(const UUID& id) : ID(id) {}
    };

    // ============================================================
    // TagComponent
    // ============================================================
    struct TagComponent {
        std::string Tag;
        TagComponent() = default;
        TagComponent(const TagComponent&) = default;
        TagComponent(const std::string& tag) : Tag(tag) {}
    };

    // ============================================================
    // TransformComponent
    // ============================================================
    struct TransformComponent {
        glm::vec3 Translation = { 0.0f, 0.0f, 0.0f };
        glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f }; // euler angles (radians)
        glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };

        TransformComponent() = default;
        TransformComponent(const TransformComponent&) = default;
        TransformComponent(const glm::vec3& translation) : Translation(translation) {}

        glm::mat4 GetTransform() const {
            glm::mat4 rot = glm::toMat4(glm::quat(Rotation));
            return glm::translate(glm::mat4(1.0f), Translation)
                * rot
                * glm::scale(glm::mat4(1.0f), Scale);
        }
    };

    // ============================================================
    // MeshRendererComponent
    // like MeshRenderer in Unity / StaticMeshComponent in Unreal
    // ============================================================
    struct MeshRendererComponent {
        std::shared_ptr<Shader>      MeshShader;
        std::shared_ptr<VertexArray> VAO;
        std::shared_ptr<Texture2D>   Albedo;
        glm::vec4                    Tint = { 1.0f, 1.0f, 1.0f, 1.0f };
        bool                         Visible = true;
        uint32_t                     VertexCount = 0; // used if no index buffer

        MeshRendererComponent() = default;
        MeshRendererComponent(const MeshRendererComponent&) = default;
    };

    // ============================================================
    // SpriteRendererComponent (2D)
    // ============================================================
    struct SpriteRendererComponent {
        glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };
        SpriteRendererComponent() = default;
        SpriteRendererComponent(const SpriteRendererComponent&) = default;
        SpriteRendererComponent(const glm::vec4& color) : Color(color) {}
    };

    // ============================================================
    // CameraComponent
    // holds a camera used to render the scene
    // ============================================================
    struct CameraComponent {
        PerspectiveCamera Camera{ 45.0f, 16.0f / 9.0f, 0.1f, 1000.0f };
        bool Primary = true;
        bool FixedAspectRatio = false;
        bool UseDirectView = false; // если true — Scene не трогает view matrix

        CameraComponent() = default;
        CameraComponent(const CameraComponent&) = default;
    };

    // ============================================================
    // LightComponent
    // like Light in Unity / DirectionalLightComponent in Unreal
    // ============================================================
    enum class LightType { Directional = 0, Point = 1 };

    struct LightComponent {
        LightType Type = LightType::Directional;
        glm::vec3 Color = { 1.0f, 1.0f, 1.0f };
        float     Intensity = 1.0f;
        // point light
        float     Radius = 10.0f;

        LightComponent() = default;
        LightComponent(const LightComponent&) = default;
    };

    // ============================================================
    // ScriptableEntityComponent (NativeScript)
    // ============================================================
    class ScriptableEntity;
    struct NativeScriptComponent {
        ScriptableEntity* Instance = nullptr;

        ScriptableEntity* (*InstantiateScript)();
        void (*DestroyScript)(NativeScriptComponent*);

        template<typename T>
        void Bind() {
            InstantiateScript = []() { return static_cast<ScriptableEntity*>(new T()); };
            DestroyScript = [](NativeScriptComponent* nsc) {
                delete nsc->Instance;
                nsc->Instance = nullptr;
                };
        }
    };

} // namespace Nullcore