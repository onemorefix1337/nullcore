#pragma once

#include "Core/Core.h"
#include "Events/Event.h"
#include <string>
#include <functional>

namespace Nullcore {

    struct WindowProps {
        std::string  Title;
        unsigned int Width;
        unsigned int Height;

        WindowProps(const std::string& title = "Nullcore Engine",
            unsigned int width = 1280,
            unsigned int height = 720)
            : Title(title), Width(width), Height(height) {
        }
    };

    class Window {
    public:
        using EventCallbackFn = std::function<void(Event&)>;

        virtual ~Window() = default;

        virtual void OnUpdate() = 0;
        virtual void Clear(float r, float g, float b, float a = 1.0f) = 0;

        virtual unsigned int GetWidth()  const = 0;
        virtual unsigned int GetHeight() const = 0;

        virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
        virtual void SetVSync(bool enabled) = 0;
        virtual bool IsVSync() const = 0;

        virtual void* GetNativeWindow() const = 0;
        virtual bool  ShouldClose()     const = 0;

        static Window* Create(const WindowProps& props = WindowProps());
    };

}