#pragma once

// ============================================================
// Nullcore Engine — master include
// ============================================================

// Core
#include "Core/Core.h"
#include "Core/Log.h"
#include "Core/UUID.h"
#include "Core/Input.h"
#include "Core/KeyCodes.h"
#include "Core/Layer.h"
#include "Core/LayerStack.h"
#include "Core/Application.h"
#include "Window.h"

// Events
#include "Events/Event.h"
#include "Events/ApplicationEvent.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"

// Renderer
#include "Renderer/Buffer.h"
#include "Renderer/VertexArray.h"
#include "Renderer/Shader.h"
#include "Renderer/Texture.h"
#include "Renderer/Camera.h"
#include "Renderer/RenderCommand.h"
#include "Renderer/Renderer.h"

// Scene / ECS
#include "Scene/Scene.h"
#include "Scene/Entity.h"
#include "Scene/Components.h"
#include "Scene/ScriptableEntity.h"

// Entry Point
#include "Core/EntryPoint.h"