#pragma once

#include "../Core/Core.h" 
#include <string>
#include <functional>
#include "../Core/Core.h"

namespace Nullcore {

    enum class EventType {
        None = 0,
        // window
        WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
        // keyboard
        KeyPressed, KeyReleased, KeyTyped,
        // mouse
        MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
    };

    enum EventCategory {
        None = 0,
        EventCategoryApplication = BIT(0),
        EventCategoryInput = BIT(1),
        EventCategoryKeyboard = BIT(2),
        EventCategoryMouse = BIT(3),
        EventCategoryMouseButton = BIT(4)
    };

    // макросы для реализации в каждом ивенте
#define EVENT_CLASS_TYPE(type) \
    static EventType GetStaticType() { return EventType::type; } \
    virtual EventType GetEventType() const override { return GetStaticType(); } \
    virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) \
    virtual int GetCategoryFlags() const override { return category; }

    class Event {
    public:
        bool Handled = false;

        virtual EventType   GetEventType()     const = 0;
        virtual const char* GetName()          const = 0;
        virtual int         GetCategoryFlags() const = 0;
        virtual std::string ToString()         const { return GetName(); }

        bool IsInCategory(EventCategory category) const {
            return GetCategoryFlags() & category;
        }
    };

    // диспетчер - вызывает нужную функцию для нужного типа ивента
    class EventDispatcher {
    public:
        EventDispatcher(Event& event) : m_Event(event) {}

        template<typename T, typename F>
        bool Dispatch(const F& func) {
            if (m_Event.GetEventType() == T::GetStaticType()) {
                m_Event.Handled |= func(static_cast<T&>(m_Event));
                return true;
            }
            return false;
        }

    private:
        Event& m_Event;
    };

}