# Nullcore Engine

A custom 3D game engine built from scratch in C++20. Developed by [4eterytyy](https://t.me/eterytyy) and PluvCoder1337 as an open-source project alongside the game **Bytefire**.

---

## Features

- **Entity Component System** — powered by EnTT. Transform, MeshRenderer, Camera, Light, NativeScript components
- **OpenGL 4.6 Core Profile** — modern renderer with abstracted RendererAPI, VAO, VBO, shaders, Texture2D via stb_image
- **Scene System** — runtime and editor update loops, RenderSystem, ScriptSystem, SceneSerializer
- **Native C++ Scripting** — derive from `ScriptableEntity`, attach to any entity, zero overhead
- **3 Camera Modes** — Orbit, Free Noclip (WASD + mouse lock), Cube rotation
- **Event System** — keyboard, mouse, window events dispatched through typed handlers
- **Layer Stack** — ordered rendering and update layers

## Requirements

- Windows 10/11 x64
- Visual Studio 2022
- GPU with OpenGL 4.6 support

## Building

Dependencies are vendored — no package manager needed.

```
git clone https://github.com/onemorefix1337/nullcore.git
cd nullcore
```

1. Open `NullcoreEngine.slnx` in Visual Studio 2022
2. Set configuration to **Release x64**
3. Build solution `Ctrl+Shift+B`

The sandbox executable lands in `x64/Release/Sandbox.exe`.

## Sandbox Controls

| Key | Action |
|-----|--------|
| `1` | Cube mode — LMB rotates selected cube |
| `2` | Orbit mode — LMB orbits camera, Scroll zooms |
| `3` | Free cam (noclip) |
| `Tab` | Cycle selected cube |
| `WASD` | Move in free cam |
| `Q / E` | Down / Up |
| `Shift` | Sprint (4× speed) |
| `RMB` | Hold to look around |
| `F1` | Toggle mouse lock |
| `Esc` | Quit |

## Architecture

```
NullcoreEngine/          ← static library
├── Core/                ← Application, LayerStack, Log, UUID
├── Renderer/            ← RendererAPI, Shader, Buffer, Texture, Camera
├── Scene/               ← Scene, Entity, Components, SceneSerializer
└── Platform/Windows/    ← GLFW window, OpenGL backend

Sandbox/                 ← executable, links NullcoreEngine.lib
└── SandboxApp.cpp       ← 3 cubes, 3 camera modes, ECS demo
```

## Vendored Libraries

| Library | Purpose |
|---------|---------|
| GLFW | Window and input |
| GLAD | OpenGL loader |
| GLM | Math (vectors, matrices, quaternions) |
| EnTT | Entity-Component registry |
| stb_image | Texture loading |

## Authors

- **4eterytyy** — Core Developer
- **PluvCoder1337** — Core Developer

## License

MIT
