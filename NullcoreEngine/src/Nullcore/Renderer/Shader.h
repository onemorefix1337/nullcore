#pragma once

#include <string>
#include <../glad/include/glad/glad.h>

namespace Nullcore {

    class Shader {
    public:
        Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
        ~Shader();

        void Bind() const;
        void Unbind() const;

        void SetInt(const std::string& name, int value);        // ← ДОБАВЬ
        void SetFloat(const std::string& name, float value);    // ← ДОБАВЬ

        unsigned int GetRendererID() const { return m_RendererID; }

    private:
        unsigned int m_RendererID;
    };

}