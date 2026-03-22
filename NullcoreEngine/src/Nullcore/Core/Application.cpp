#include "Application.h"
#include "../Renderer/Renderer.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Nullcore {

    Application* Application::s_Instance = nullptr;

    Application::Application() {
        if (s_Instance) {
            NC_CORE_ERROR("Application already exists!");
            return;
        }
        s_Instance = this;

        m_Window = std::unique_ptr<Window>(Window::Create(WindowProps("Nullcore Engine", 1280, 720)));
        m_Window->SetEventCallback([this](Event& e) { this->OnEvent(e); });

        Renderer::Init();  // ← ДОБАВЬ ЭТО

        NC_CORE_INFO("Application created");
    }

    Application::~Application() {
        NC_CORE_INFO("Application destroyed");
    }

    void Application::PushLayer(Layer* layer) {
        m_LayerStack.PushLayer(layer);
    }

    void Application::PushOverlay(Layer* overlay) {
        m_LayerStack.PushOverlay(overlay);
    }

    void Application::OnEvent(Event& e) {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>([this](WindowCloseEvent& e) {
            return this->OnWindowClose(e);
            });
        dispatcher.Dispatch<WindowResizeEvent>([this](WindowResizeEvent& e) {
            return this->OnWindowResize(e);
            });

        // шлем ивент по слоям СВЕРХУ ВНИЗ (оверлеи первые)
        for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it) {
            if (e.Handled) break;
            (*it)->OnEvent(e);
        }
    }

    bool Application::OnWindowClose(WindowCloseEvent& e) {
        m_Running = false;
        return true;
    }

    bool Application::OnWindowResize(WindowResizeEvent& e) {
        NC_CORE_INFO("Window resize: " + std::to_string(e.GetWidth()) + "x" + std::to_string(e.GetHeight()));
        Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());  // ← ВМЕСТО glViewport
        return false;
    }

    void Application::Run() {
        while (m_Running && !m_Window->ShouldClose()) {
            float time = (float)glfwGetTime();
            float deltaTime = time - m_LastFrameTime;
            m_LastFrameTime = time;

            // апдейт всех слоёв СНИЗУ ВВЕРХ
            for (Layer* layer : m_LayerStack)
                layer->OnUpdate(deltaTime);

            m_Window->OnUpdate();
            Renderer::Init();
        }
    }

}