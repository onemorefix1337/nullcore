#pragma once

#include "Nullcore/Core/Input.h"
#include <iostream>

namespace Nullcore {

    class WindowsInput : public Input {
    public:
        static bool IsKeyPressed(int keycode);
        static bool IsMouseButtonPressed(int button);
        static float GetMouseX();
        static float GetMouseY();
        static std::pair<float, float> GetMousePosition();
    };

}