#pragma once
#include "../Core/UUID.h"
#include <../entt/entt.hpp>
#include <string>
#include <glm/glm.hpp>

namespace Nullcore {

    class Entity;

    // ============================================================
    // Scene
    // Manages all entities, their components, and runs systems.
    // Similar to Scene in Unity or UWorld in Unreal.
    // ============================================================
    class Scene {
    public:
        Scene();
        ~Scene();

        // Entity management
        Entity CreateEntity(const std::string& name = std::string());
        Entity CreateEntityWithUUID(UUID uuid, const std::string& name = std::string());
        void   DestroyEntity(Entity entity);

        // Scene update — called every frame
        void OnUpdateRuntime(float deltaTime);
        void OnUpdateEditor(float deltaTime);

        // Viewport
        void OnViewportResize(uint32_t width, uint32_t height);

        // Queries
        Entity GetPrimaryCameraEntity();

        // Iterate all entities with given component type
        template<typename... Components>
        auto GetAllEntitiesWith() {
            return m_Registry.view<Components...>();
        }

    private:
        // Systems
        void RenderSystem();
        void ScriptSystem(float deltaTime);

        entt::registry m_Registry;
        uint32_t m_ViewportWidth = 1280;
        uint32_t m_ViewportHeight = 720;

        friend class Entity;
    };

} // namespace Nullcore