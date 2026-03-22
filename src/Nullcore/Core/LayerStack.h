#pragma once

#include "Layer.h"
#include <vector>

namespace Nullcore {

    class LayerStack {
    public:
        LayerStack() = default;
        ~LayerStack();

        void PushLayer(Layer* layer);      // обычный слой (снизу)
        void PushOverlay(Layer* overlay);  // оверлей (сверху, типа UI/debug)
        void PopLayer(Layer* layer);
        void PopOverlay(Layer* overlay);

        // итераторы для прохода по всем слоям
        std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }
        std::vector<Layer*>::iterator end() { return m_Layers.end(); }
        std::vector<Layer*>::reverse_iterator rbegin() { return m_Layers.rbegin(); }
        std::vector<Layer*>::reverse_iterator rend() { return m_Layers.rend(); }

    private:
        std::vector<Layer*> m_Layers;
        unsigned int m_LayerInsertIndex = 0; // индекс границы слоёв/оверлеев
    };

}