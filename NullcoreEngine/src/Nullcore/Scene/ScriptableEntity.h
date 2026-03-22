#pragma once
#include "Entity.h"

namespace Nullcore {

    // ============================================================
    // ScriptableEntity
    // Base class for native C++ scripts attached to entities.
    // Like MonoBehaviour in Unity or ActorComponent in Unreal.
    // Usage:
    //   class MyScript : public ScriptableEntity {
    //       void OnCreate() override { ... }
    //       void OnUpdate(float dt) override { ... }
    //   };
    //   entity.AddComponent<NativeScriptComponent>().Bind<MyScript>();
    // ============================================================
    class ScriptableEntity {
    public:
        virtual ~ScriptableEntity() {}

        template<typename T>
        T& GetComponent() {
            return m_Entity.GetComponent<T>();
        }

    protected:
        virtual void OnCreate() {}
        virtual void OnUpdate(float deltaTime) {}
        virtual void OnDestroy() {}

    private:
        Entity m_Entity;
        friend class Scene;
    };

} // namespace Nullcore