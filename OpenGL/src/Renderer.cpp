#include "Renderer.h"
#include <iostream>

void GLClearError() {
    // 清除所有error信息
    while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line) {
    while (GLenum error = glGetError()) {
        std::cout << "[OpenGL Error]( " << error << " ): " << function
            << " " << file << ":" << line << std::endl;
        return false;
    }
    return true;
}

void Renderer::Clear() const
{
    GLCall(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));

    GLbitfield clealMask = GL_COLOR_BUFFER_BIT;
    if (_enableDepthTest)
        clealMask = clealMask | GL_DEPTH_BUFFER_BIT;

    GLCall(glClear(clealMask));
}

void Renderer::SetDepthTest(bool enable)
{
    _enableDepthTest = enable;

    if (_enableDepthTest) {
        GLCall(glEnable(GL_DEPTH_TEST));
    }
    else {
        GLCall(glDisable(GL_DEPTH_TEST));
    }
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
    shader.Bind();
    va.Bind();
    ib.Bind();

    GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::DrawArrays(const VertexArray& va, const Shader& shader, const int count) const
{
    shader.Bind();
    va.Bind();

    GLCall(glDrawArrays(GL_TRIANGLES, 0, count));
}
