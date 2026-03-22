#include "Shader.h"
#include "../Core/Log.h"
#include <vector>

namespace Nullcore {

    Shader::Shader(const std::string& vertexSrc, const std::string& fragmentSrc) {
        unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
        const char* vSrc = vertexSrc.c_str();
        glShaderSource(vertexShader, 1, &vSrc, nullptr);
        glCompileShader(vertexShader);

        int success;
        char infoLog[512];
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
            NC_CORE_ERROR(std::string("Vertex shader compilation failed:\n") + infoLog);
        }

        unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        const char* fSrc = fragmentSrc.c_str();
        glShaderSource(fragmentShader, 1, &fSrc, nullptr);
        glCompileShader(fragmentShader);

        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
            NC_CORE_ERROR(std::string("Fragment shader compilation failed:\n") + infoLog);
        }

        m_RendererID = glCreateProgram();
        glAttachShader(m_RendererID, vertexShader);
        glAttachShader(m_RendererID, fragmentShader);
        glLinkProgram(m_RendererID);

        glGetProgramiv(m_RendererID, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(m_RendererID, 512, nullptr, infoLog);
            NC_CORE_ERROR(std::string("Shader linking failed:\n") + infoLog);
        }

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        NC_CORE_INFO("Shader compiled successfully");
    }

    Shader::~Shader() {
        glDeleteProgram(m_RendererID);
    }

    void Shader::Bind() const {
        glUseProgram(m_RendererID);
    }

    void Shader::Unbind() const {
        glUseProgram(0);
    }

    void Shader::SetInt(const std::string& name, int value) {
        glUniform1i(glGetUniformLocation(m_RendererID, name.c_str()), value);
    }

    void Shader::SetFloat(const std::string& name, float value) {
        glUniform1f(glGetUniformLocation(m_RendererID, name.c_str()), value);
    }

}