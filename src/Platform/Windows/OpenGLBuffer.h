#pragma once

#include "Nullcore/Renderer/Buffer.h"

namespace Nullcore {

    // ============================================================
    // OpenGL VertexBuffer
    // ============================================================
    class OpenGLVertexBuffer : public VertexBuffer {
    public:
        OpenGLVertexBuffer(float* vertices, unsigned int size);
        virtual ~OpenGLVertexBuffer();

        virtual void Bind()   const override;
        virtual void Unbind() const override;
        virtual void SetData(const void* data, uint32_t size) override;

        virtual void SetLayout(const BufferLayout& layout) override { m_Layout = layout; }
        virtual const BufferLayout& GetLayout() const override { return m_Layout; }

    private:
        unsigned int m_RendererID;
        BufferLayout m_Layout;
    };

    // ============================================================
    // OpenGL IndexBuffer
    // ============================================================
    class OpenGLIndexBuffer : public IndexBuffer {
    public:
        OpenGLIndexBuffer(unsigned int* indices, unsigned int count);
        virtual ~OpenGLIndexBuffer();

        virtual void Bind()   const override;
        virtual void Unbind() const override;

        virtual unsigned int GetCount() const override { return m_Count; }

    private:
        unsigned int m_RendererID;
        unsigned int m_Count;
    };

}