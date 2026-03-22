#pragma once

#include "Core.h"
#include <utility>

namespace Nullcore {

    class Input {
    public:
        // keyboard
        static bool IsKeyPressed(int keycode);

        // mouse
        static bool IsMouseButtonPressed(int button);
        static float GetMouseX();
        static float GetMouseY();
        static std::pair<float, float> GetMousePosition();
    };

}