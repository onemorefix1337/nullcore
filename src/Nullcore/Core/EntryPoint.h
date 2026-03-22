#pragma once

#include "Application.h"
#include "Log.h"

#ifdef NC_PLATFORM_WINDOWS

extern Nullcore::Application* Nullcore::CreateApplication();

int main(int argc, char** argv) {
    Nullcore::Log::Init();

    NC_CORE_INFO("=================================");
    NC_CORE_INFO("   NULLCORE ENGINE v0.1 Alpha   ");
    NC_CORE_INFO("=================================");

    auto app = Nullcore::CreateApplication();
    app->Run();
    delete app;

    NC_CORE_INFO("Engine shutdown complete");
    return 0;
}

#endif