#pragma once

#include "Core.h"
#include "../Events/Event.h"
#include <string>

namespace Nullcore {

    class Layer {
    public:
        Layer(const std::string& name = "Layer") : m_DebugName(name) {}
        virtual ~Layer() = default;

        virtual void OnAttach() {}                    // когда слой добавляется
        virtual void OnDetach() {}                    // когда слой убирается
        virtual void OnUpdate(float deltaTime) {}     // каждый кадр
        virtual void OnEvent(Event& event) {}         // обработка событий
        virtual void OnImGuiRender() {}               // debug UI

        const std::string& GetName() const { return m_DebugName; }

    protected:
        std::string m_DebugName;
    };

}