#pragma once

#include "Nullcore/Window.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Nullcore {

    class WindowsWindow : public Window {
    public:
        WindowsWindow(const WindowProps& props);
        virtual ~WindowsWindow();

        void OnUpdate() override;
        void Clear(float r, float g, float b, float a = 1.0f) override;

        unsigned int GetWidth()  const override { return m_Data.Width; }
        unsigned int GetHeight() const override { return m_Data.Height; }

        void SetEventCallback(const EventCallbackFn& callback) override {
            m_Data.EventCallback = callback;
        }
        void SetVSync(bool enabled) override;
        bool IsVSync() const override { return m_Data.VSync; }

        void* GetNativeWindow() const override { return m_Window; }
        bool  ShouldClose()     const override { return glfwWindowShouldClose(m_Window); }

    private:
        void Init(const WindowProps& props);
        void Shutdown();

        GLFWwindow* m_Window;

        struct WindowData {
            std::string   Title;
            unsigned int  Width = 0;
            unsigned int  Height = 0;
            bool          VSync = true;
            EventCallbackFn EventCallback;
        };

        WindowData m_Data;
    };

}