#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
                  x;\
                  ASSERT(GLLogCall(#x, __FILE__, __LINE__));

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);

class Renderer {
private:
    bool _enableDepthTest;
    bool _enableStencilTest;
    glm::vec4 _clearColor = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f);
public:
    void Clear() const;
    void SetDepthTest(bool enable);
    void SetStencilTest(bool enable);
    void SetClearColor(glm::vec4 color);
    void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
    void DrawArrays(const VertexArray& va, const Shader& shader, const int count) const;
};