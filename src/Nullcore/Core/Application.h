#pragma once

#include "Core.h"
#include "Log.h"
#include "Layer.h"
#include "LayerStack.h"
#include "../Window.h"
#include "../Events/Event.h"
#include "../Events/ApplicationEvent.h"
#include <memory>

namespace Nullcore {

    class Application {
    public:
        Application();
        virtual ~Application();

        virtual void Run();
        virtual void OnEvent(Event& e);

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* overlay);

        Window& GetWindow() { return *m_Window; }
        static Application& Get() { return *s_Instance; }

    protected:
        std::unique_ptr<Window> m_Window;
        bool m_Running = true;
        float m_LastFrameTime = 0.0f;

    private:
        bool OnWindowClose(WindowCloseEvent& e);
        bool OnWindowResize(WindowResizeEvent& e);

        LayerStack m_LayerStack;
        static Application* s_Instance;
    };

    Application* CreateApplication();

}