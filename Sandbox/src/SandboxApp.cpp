#include <Nullcore.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <C:\Projects\NullcoreEngine\vendor\GLFW\include\GLFW\glfw3.h>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <memory>
#include <string>
#include <cmath>

// ============================================================
// Helpers
// ============================================================
static std::shared_ptr<Nullcore::VertexArray> BuildCubeVAO() {
    float v[] = {
        -0.5f,-0.5f, 0.5f, 1,1,1, 0,0,
         0.5f,-0.5f, 0.5f, 1,1,1, 1,0,
         0.5f, 0.5f, 0.5f, 1,1,1, 1,1,
         0.5f, 0.5f, 0.5f, 1,1,1, 1,1,
        -0.5f, 0.5f, 0.5f, 1,1,1, 0,1,
        -0.5f,-0.5f, 0.5f, 1,1,1, 0,0,

        -0.5f,-0.5f,-0.5f, 1,1,1, 1,0,
         0.5f,-0.5f,-0.5f, 1,1,1, 0,0,
         0.5f, 0.5f,-0.5f, 1,1,1, 0,1,
         0.5f, 0.5f,-0.5f, 1,1,1, 0,1,
        -0.5f, 0.5f,-0.5f, 1,1,1, 1,1,
        -0.5f,-0.5f,-0.5f, 1,1,1, 1,0,

        -0.5f, 0.5f, 0.5f, 1,1,1, 1,1,
        -0.5f, 0.5f,-0.5f, 1,1,1, 0,1,
        -0.5f,-0.5f,-0.5f, 1,1,1, 0,0,
        -0.5f,-0.5f,-0.5f, 1,1,1, 0,0,
        -0.5f,-0.5f, 0.5f, 1,1,1, 1,0,
        -0.5f, 0.5f, 0.5f, 1,1,1, 1,1,

         0.5f, 0.5f, 0.5f, 1,1,1, 0,1,
         0.5f, 0.5f,-0.5f, 1,1,1, 1,1,
         0.5f,-0.5f,-0.5f, 1,1,1, 1,0,
         0.5f,-0.5f,-0.5f, 1,1,1, 1,0,
         0.5f,-0.5f, 0.5f, 1,1,1, 0,0,
         0.5f, 0.5f, 0.5f, 1,1,1, 0,1,

        -0.5f,-0.5f,-0.5f, 1,1,1, 0,1,
         0.5f,-0.5f,-0.5f, 1,1,1, 1,1,
         0.5f,-0.5f, 0.5f, 1,1,1, 1,0,
         0.5f,-0.5f, 0.5f, 1,1,1, 1,0,
        -0.5f,-0.5f, 0.5f, 1,1,1, 0,0,
        -0.5f,-0.5f,-0.5f, 1,1,1, 0,1,

        -0.5f, 0.5f,-0.5f, 1,1,1, 0,1,
         0.5f, 0.5f,-0.5f, 1,1,1, 1,1,
         0.5f, 0.5f, 0.5f, 1,1,1, 1,0,
         0.5f, 0.5f, 0.5f, 1,1,1, 1,0,
        -0.5f, 0.5f, 0.5f, 1,1,1, 0,0,
        -0.5f, 0.5f,-0.5f, 1,1,1, 0,1,
    };
    auto vao = std::shared_ptr<Nullcore::VertexArray>(Nullcore::VertexArray::Create());
    auto vb = std::shared_ptr<Nullcore::VertexBuffer>(Nullcore::VertexBuffer::Create(v, sizeof(v)));
    vb->SetLayout({
        { Nullcore::ShaderDataType::Float3, "a_Position" },
        { Nullcore::ShaderDataType::Float3, "a_Color"    },
        { Nullcore::ShaderDataType::Float2, "a_TexCoord" },
        });
    vao->AddVertexBuffer(vb);
    return vao;
}

static std::shared_ptr<Nullcore::Shader> BuildCubeShader() {
    std::string vs = R"(
        #version 460 core
        layout(location=0) in vec3 a_Position;
        layout(location=1) in vec3 a_Color;
        layout(location=2) in vec2 a_TexCoord;
        out vec3 v_Color; out vec2 v_TexCoord;
        uniform mat4 u_ViewProjection, u_Transform;
        void main() {
            v_Color = a_Color; v_TexCoord = a_TexCoord;
            gl_Position = u_ViewProjection * u_Transform * vec4(a_Position,1.0);
        })";
    std::string fs = R"(
        #version 460 core
        layout(location=0) out vec4 color;
        in vec3 v_Color; in vec2 v_TexCoord;
        uniform float u_Time; uniform sampler2D u_Texture;
        uniform vec4  u_Tint;
        void main() {
            float pulse = 0.85 + 0.15*sin(u_Time*2.0);
            color = texture(u_Texture, v_TexCoord) * vec4(v_Color*pulse,1.0) * u_Tint;
        })";
    return std::make_shared<Nullcore::Shader>(vs, fs);
}

// ============================================================
// SceneLayer — 3 camera modes, switchable by 1/2/3
// ============================================================
class SceneLayer : public Nullcore::Layer {
public:
    enum class CameraMode { Cube = 0, Orbit = 1, Free = 2 };

    SceneLayer() : Layer("SceneLayer") {}

    void OnAttach() override {
        m_Scene = std::make_shared<Nullcore::Scene>();
        auto sh = BuildCubeShader();
        auto vao = BuildCubeVAO();

        auto tex = Nullcore::Texture2D::Create("C:/NullcoreAssets/textures/test.png");
        if (!tex || tex->GetWidth() == 0) {
            tex = Nullcore::Texture2D::Create(1, 1);
            uint32_t w = 0xffffffff; tex->SetData(&w, sizeof(w));
        }

        // camera entity
        m_CameraEntity = m_Scene->CreateEntity("Main Camera");
        auto& cc = m_CameraEntity.AddComponent<Nullcore::CameraComponent>();
        cc.Primary = true;

        // 3 cubes
        auto makeCube = [&](const char* name, glm::vec3 pos, glm::vec3 scale) -> Nullcore::Entity {
            auto e = m_Scene->CreateEntity(name);
            auto& tc = e.GetComponent<Nullcore::TransformComponent>();
            tc.Translation = pos; tc.Scale = scale;
            auto& mesh = e.AddComponent<Nullcore::MeshRendererComponent>();
            mesh.MeshShader = sh; mesh.VAO = vao; mesh.Albedo = tex;
            return e;
            };

        m_Cubes[0] = makeCube("Cube_A", { 0.0f, 0.0f, 0.0f }, { 1.0f,1.0f,1.0f });
        m_Cubes[1] = makeCube("Cube_B", { 2.2f, 0.0f, 0.0f }, { 0.55f,0.55f,0.55f });
        m_Cubes[2] = makeCube("Cube_C", { -2.2f, 0.0f, 0.0f }, { 0.6f,1.6f,0.6f });

        m_Shader = sh;
    }

    // рисует 2D стрелку указывающую на кубы когда они вне FOV
    // использует modern OpenGL (core profile)
    void DrawOffscreenIndicator() {
        GLFWwindow* win = (GLFWwindow*)Nullcore::Application::Get().GetWindow().GetNativeWindow();
        glfwGetWindowSize(win, &m_WinW, &m_WinH);

        float yr = glm::radians(m_FreeYaw);
        float pr = glm::radians(m_FreePitch);
        glm::vec3 forward(cosf(pr) * sinf(yr), sinf(pr), -cosf(pr) * cosf(yr));
        glm::vec3 toTarget = glm::normalize(-m_FreePos);
        float d = glm::dot(forward, toTarget);
        if (d > 0.3f) return; // кубы в поле зрения — не рисуем

        // проецируем направление на экран
        glm::vec3 right(cosf(yr), 0.0f, sinf(yr));
        float sx = glm::dot(toTarget, right);
        float sy = glm::dot(toTarget, glm::vec3(0, 1, 0));
        float len = sqrtf(sx * sx + sy * sy);
        if (len < 0.001f) return;
        sx /= len; sy /= len;

        // NDC позиция кончика и хвоста стрелки
        float r = 0.08f; // радиус в NDC
        float wing = 0.025f;
        float tip_x = sx * r;
        float tip_y = sy * r;
        float tail_x = sx * (r - 0.04f);
        float tail_y = sy * (r - 0.04f);
        float px = -sy, py = sx; // перпендикуляр

        // 6 вершин: стержень (2) + левое крыло (2) + правое крыло (2)
        float verts[] = {
            0.0f,  0.0f,
            tip_x, tip_y,
            tip_x, tip_y,
            tail_x + px * wing, tail_y + py * wing,
            tip_x, tip_y,
            tail_x - px * wing, tail_y - py * wing,
        };

        // ленивая инициализация VAO/VBO/шейдера
        if (!m_ArrowShader) {
            const char* vs = R"(
                #version 460 core
                layout(location=0) in vec2 a_Pos;
                void main() { gl_Position = vec4(a_Pos, 0.0, 1.0); }
            )";
            const char* fs = R"(
                #version 460 core
                out vec4 color;
                uniform vec4 u_Color;
                void main() { color = u_Color; }
            )";
            GLuint v = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(v, 1, &vs, nullptr); glCompileShader(v);
            GLuint f = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(f, 1, &fs, nullptr); glCompileShader(f);
            m_ArrowShader = glCreateProgram();
            glAttachShader(m_ArrowShader, v);
            glAttachShader(m_ArrowShader, f);
            glLinkProgram(m_ArrowShader);
            glDeleteShader(v); glDeleteShader(f);

            glGenVertexArrays(1, &m_ArrowVAO);
            glGenBuffers(1, &m_ArrowVBO);
        }

        glBindVertexArray(m_ArrowVAO);
        glBindBuffer(GL_ARRAY_BUFFER, m_ArrowVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);

        glUseProgram(m_ArrowShader);
        float alpha = glm::clamp(1.0f - (d + 1.0f) * 0.5f, 0.4f, 1.0f);
        GLint loc = glGetUniformLocation(m_ArrowShader, "u_Color");
        glUniform4f(loc, 1.0f, 0.9f, 0.3f, alpha);

        glDisable(GL_DEPTH_TEST);
        glLineWidth(2.5f);
        glDrawArrays(GL_LINES, 0, 6);
        glEnable(GL_DEPTH_TEST);

        glBindVertexArray(0);
        glUseProgram(0);
    }


    void OnUpdate(float deltaTime) override {
        float t = (float)glfwGetTime();

        // ---- авто-вращение кубов (накопленные углы) ----
        float speeds[3][3] = { {0,30,0}, {0,50,0}, {20,0,15} };
        for (int i = 0; i < 3; i++) {
            // не трогаем выбранный куб в режиме Cube
            if (m_Mode == CameraMode::Cube && i == m_SelectedCube) continue;
            m_CubeAngles[i].x += speeds[i][0] * deltaTime;
            m_CubeAngles[i].y += speeds[i][1] * deltaTime;
            m_CubeAngles[i].z += speeds[i][2] * deltaTime;
            auto& tc = m_Cubes[i].GetComponent<Nullcore::TransformComponent>();
            tc.Rotation = glm::radians(m_CubeAngles[i]);
        }

        // ---- камера по режиму ----
        UpdateCamera(deltaTime);

        // uniforms (tint + time)
        m_Shader->Bind();
        m_Shader->SetFloat("u_Time", t);

        // подсветка выбранного куба жёлтым в режиме Cube
        for (int i = 0; i < 3; i++) {
            auto& mesh = m_Cubes[i].GetComponent<Nullcore::MeshRendererComponent>();
            // u_Tint через прямой glUniform4f
            mesh.MeshShader->Bind();
            int loc = glGetUniformLocation(mesh.MeshShader->GetRendererID(), "u_Tint");
            if (m_Mode == CameraMode::Cube && i == m_SelectedCube)
                glUniform4f(loc, 1.0f, 0.95f, 0.4f, 1.0f); // жёлтый highlight
            else
                glUniform4f(loc, 1.0f, 1.0f, 1.0f, 1.0f);
        }

        Nullcore::RenderCommand::SetClearColor({ 0.08f,0.08f,0.12f,1.0f });
        Nullcore::RenderCommand::Clear();
        m_Scene->OnUpdateRuntime(deltaTime);

        // стрелка к кубам в free cam когда они вне поля зрения
        if (m_Mode == CameraMode::Free)
            DrawOffscreenIndicator();

        // title
        m_FpsTimer += deltaTime; m_FrameCount++;
        if (m_FpsTimer >= 0.5f) {
            double fps = m_FrameCount / m_FpsTimer;
            std::ostringstream ss;
            ss << "Nullcore Engine  |  FPS: " << std::fixed << std::setprecision(0) << fps << "  |  ";
            if (m_Mode == CameraMode::Cube) {
                ss << "Mode: CUBE  |  Tab: next cube  |  LMB: rotate  |  1/2/3: mode";
            }
            else if (m_Mode == CameraMode::Orbit) {
                ss << "Mode: ORBIT  |  LMB: rotate  |  Scroll: zoom  |  1/2/3: mode";
            }
            else {
                ss << "Mode: FREE CAM  |  WASD: move  |  Q/E: up/down  |  Shift: sprint  |  RMB/F1: look  |  1/2/3: mode";
            }
            GLFWwindow* win = (GLFWwindow*)Nullcore::Application::Get().GetWindow().GetNativeWindow();
            glfwSetWindowTitle(win, ss.str().c_str());
            m_FpsTimer = 0; m_FrameCount = 0;
        }
    }

    void UpdateCamera(float dt) {
        auto& camTc = m_CameraEntity.GetComponent<Nullcore::TransformComponent>();

        if (m_Mode == CameraMode::Orbit || m_Mode == CameraMode::Cube) {
            // orbit: камера крутится вокруг (0,0,0)
            float yaw = glm::radians(m_OrbitYaw);
            float pitch = glm::radians(m_OrbitPitch);
            // позиция через сферические координаты
            camTc.Translation = {
                m_OrbitZoom * cosf(pitch) * sinf(yaw),
                m_OrbitZoom * sinf(pitch),
                m_OrbitZoom * cosf(pitch) * cosf(yaw)
            };
            // новая view = rotX(pitch)*rotY(yaw)*translate(-pos)
            // чтобы смотреть на (0,0,0) нужно pitch = -OrbitPitch, yaw = 180-OrbitYaw
            // но проще: просто инвертируем углы
            camTc.Rotation = { -m_OrbitPitch, m_OrbitYaw + 180.0f, 0.0f };
        }
        else if (m_Mode == CameraMode::Free) {
            GLFWwindow* win = (GLFWwindow*)Nullcore::Application::Get().GetWindow().GetNativeWindow();

            // строим forward/right/up из yaw и pitch
            // yaw=0 смотрим в -Z, yaw=90 в +X
            float yr = glm::radians(m_FreeYaw);
            float pr = glm::radians(m_FreePitch);

            glm::vec3 forward;
            forward.x = cosf(pr) * sinf(yr);
            forward.y = sinf(pr);
            forward.z = -cosf(pr) * cosf(yr);
            forward = glm::normalize(forward);

            // right всегда в горизонтальной плоскости — не зависит от pitch
            glm::vec3 right;
            right.x = cosf(yr);
            right.y = 0.0f;
            right.z = sinf(yr);
            right = glm::normalize(right);

            // up = мировой, Q/E всегда летят вертикально
            glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);

            float speed = dt * (glfwGetKey(win, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS ? 20.0f : 5.0f);

            if (glfwGetKey(win, GLFW_KEY_W) == GLFW_PRESS) m_FreePos += forward * speed;
            if (glfwGetKey(win, GLFW_KEY_S) == GLFW_PRESS) m_FreePos -= forward * speed;
            if (glfwGetKey(win, GLFW_KEY_A) == GLFW_PRESS) m_FreePos -= right * speed;
            if (glfwGetKey(win, GLFW_KEY_D) == GLFW_PRESS) m_FreePos += right * speed;
            if (glfwGetKey(win, GLFW_KEY_E) == GLFW_PRESS) m_FreePos += worldUp * speed;
            if (glfwGetKey(win, GLFW_KEY_Q) == GLFW_PRESS) m_FreePos -= worldUp * speed;

            // считаем view через lookAt — полностью независимо от Camera rotation
            glm::mat4 view = glm::lookAt(m_FreePos, m_FreePos + forward, worldUp);

            auto& cc = m_CameraEntity.GetComponent<Nullcore::CameraComponent>();
            cc.UseDirectView = true;
            cc.Camera.SetViewMatrixDirect(view);
            camTc.Translation = m_FreePos;
        }
    }

    void OnEvent(Nullcore::Event& e) override {
        Nullcore::EventDispatcher d(e);

        // --- переключение режимов ---
        d.Dispatch<Nullcore::KeyPressedEvent>([this](Nullcore::KeyPressedEvent& e) {
            // F1 — toggle mouse lock (как в MC/CS)
            if (e.GetKeyCode() == NC_KEY_F1) {
                m_MouseLocked = !m_MouseLocked;
                GLFWwindow* win = (GLFWwindow*)Nullcore::Application::Get().GetWindow().GetNativeWindow();
                if (m_MouseLocked) {
                    glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                    m_FirstMouse = true;
                }
                else {
                    glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                }
            }
            if (e.GetKeyCode() == NC_KEY_1) {
                m_Mode = CameraMode::Cube;
                m_CameraEntity.GetComponent<Nullcore::CameraComponent>().UseDirectView = false;
            }
            if (e.GetKeyCode() == NC_KEY_2) {
                m_Mode = CameraMode::Orbit;
                m_CameraEntity.GetComponent<Nullcore::CameraComponent>().UseDirectView = false;
            }
            if (e.GetKeyCode() == NC_KEY_3) {
                m_Mode = CameraMode::Free;
                // переносим free cam на позицию orbit камеры
                auto& camTc = m_CameraEntity.GetComponent<Nullcore::TransformComponent>();
                m_FreePos = camTc.Translation;
                m_FreeYaw = m_OrbitYaw + 180.0f;  // orbit и free используют разные системы
                m_FreePitch = -m_OrbitPitch;
                NC_INFO("Mode: FREE CAM");
            }
            // выбор куба в режиме Cube
            if (m_Mode == CameraMode::Cube) {
                if (e.GetKeyCode() == NC_KEY_TAB) m_SelectedCube = (m_SelectedCube + 1) % 3;
            }
            return false;
            });

        // --- скролл = zoom ---
        d.Dispatch<Nullcore::MouseScrolledEvent>([this](Nullcore::MouseScrolledEvent& e) {
            if (m_Mode == CameraMode::Free) return false;
            m_OrbitZoom -= e.GetYOffset() * 0.4f;
            m_OrbitZoom = std::clamp(m_OrbitZoom, 1.5f, 25.0f);
            return false;
            });

        // --- мышь зажата ---
        d.Dispatch<Nullcore::MouseButtonPressedEvent>([this](Nullcore::MouseButtonPressedEvent& e) {
            if (e.GetMouseButton() == NC_MOUSE_BUTTON_LEFT) { m_MouseDown = true; m_FirstMouse = true; }
            if (e.GetMouseButton() == NC_MOUSE_BUTTON_RIGHT) { m_RmbDown = true; m_FirstMouse = true; }
            return false;
            });
        d.Dispatch<Nullcore::MouseButtonReleasedEvent>([this](Nullcore::MouseButtonReleasedEvent& e) {
            if (e.GetMouseButton() == NC_MOUSE_BUTTON_LEFT)  m_MouseDown = false;
            if (e.GetMouseButton() == NC_MOUSE_BUTTON_RIGHT) m_RmbDown = false;
            return false;
            });

        // --- движение мыши ---
        d.Dispatch<Nullcore::MouseMovedEvent>([this](Nullcore::MouseMovedEvent& e) {
            float mx = e.GetX(), my = e.GetY();

            // free cam: ПКМ зажат ИЛИ мышь залочена (F1)
            if (m_Mode == CameraMode::Free) {
                if (!m_RmbDown && !m_MouseLocked) return false;
                if (m_FirstMouse) { m_LastX = mx; m_LastY = my; m_FirstMouse = false; return false; }
                m_FreeYaw += (mx - m_LastX) * 0.18f;
                m_FreePitch -= (my - m_LastY) * 0.18f;  // минус = не инвертировано
                m_LastX = mx; m_LastY = my;
                return false;
            }

            if (!m_MouseDown) return false;
            if (m_FirstMouse) { m_LastX = mx; m_LastY = my; m_FirstMouse = false; return false; }

            float dx = (mx - m_LastX) * 0.35f;
            float dy = (m_LastY - my) * 0.35f;
            m_LastX = mx; m_LastY = my;

            if (m_Mode == CameraMode::Orbit) {
                m_OrbitYaw += dx;
                m_OrbitPitch += dy;
                m_OrbitPitch = std::clamp(m_OrbitPitch, -89.0f, 89.0f);
            }
            else if (m_Mode == CameraMode::Cube) {
                // крутим выбранный куб
                m_CubeAngles[m_SelectedCube].y += dx * 2.0f;
                m_CubeAngles[m_SelectedCube].x -= dy * 2.0f;
                auto& tc = m_Cubes[m_SelectedCube].GetComponent<Nullcore::TransformComponent>();
                tc.Rotation = glm::radians(m_CubeAngles[m_SelectedCube]);
            }
            return false;
            });
    }

private:
    std::shared_ptr<Nullcore::Scene>  m_Scene;
    std::shared_ptr<Nullcore::Shader> m_Shader;
    Nullcore::Entity m_CameraEntity;
    Nullcore::Entity m_Cubes[3];

    CameraMode m_Mode = CameraMode::Free;
    int        m_SelectedCube = 0;

    // orbit state
    float m_OrbitYaw = 0.0f;
    float m_OrbitPitch = 15.0f;
    float m_OrbitZoom = 6.5f;

    // free cam state
    glm::vec3 m_FreePos = { 0.0f, 0.5f, 6.5f };
    float     m_FreeYaw = 0.0f;   // yaw=0 -> forward в -Z
    float     m_FreePitch = -4.4f;   // чуть вниз чтобы смотреть прямо на кубы

    // cube rotation accumulators
    glm::vec3 m_CubeAngles[3] = {};

    float m_LastX = 640, m_LastY = 360;
    bool  m_MouseDown = false, m_RmbDown = false, m_FirstMouse = true;
    bool  m_MouseLocked = false;  // F1 — lock курсора
    float m_FpsTimer = 0; int m_FrameCount = 0;
    int   m_WinW = 1280, m_WinH = 720;
    // arrow indicator (modern GL)
    GLuint m_ArrowShader = 0, m_ArrowVAO = 0, m_ArrowVBO = 0;
};

// ============================================================
class Sandbox : public Nullcore::Application {
public:
    Sandbox() { PushLayer(new SceneLayer()); }
    void OnEvent(Nullcore::Event& e) override {
        Nullcore::EventDispatcher d(e);
        d.Dispatch<Nullcore::KeyPressedEvent>([this](Nullcore::KeyPressedEvent& e) {
            if (e.GetKeyCode() == NC_KEY_ESCAPE) { m_Running = false; return true; }
            return false;
            });
        Application::OnEvent(e);
    }
};

Nullcore::Application* Nullcore::CreateApplication() { return new Sandbox(); }