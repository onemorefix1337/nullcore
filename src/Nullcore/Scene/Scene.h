#pragma once

#include "../Core/UUID.h"
#include <../entt/entt.hpp>
#include <string>

namespace Nullcore {

    class Entity;  // forward declaration, БЕЗ include

    class Scene {
    public:
        Scene();
        ~Scene();

        Entity CreateEntity(const std::string& name = std::string());
        Entity CreateEntityWithUUID(UUID uuid, const std::string& name = std::string());
        void DestroyEntity(Entity entity);

        void OnUpdateRuntime(float deltaTime);
        void OnUpdateEditor(float deltaTime);

        Entity GetPrimaryCameraEntity();

    private:
        entt::registry m_Registry;
        uint32_t m_ViewportWidth = 0;
        uint32_t m_ViewportHeight = 0;

        friend class Entity;
    };

}