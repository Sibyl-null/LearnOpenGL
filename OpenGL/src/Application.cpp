#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"

GLFWwindow* OpenGLInit();
void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
void ProcessInput(GLFWwindow* window);

const unsigned int Scr_Width = 800;
const unsigned int Scr_Height = 600;
const float Clear_RGBA[4] = { 0.2f, 0.3f, 0.3f, 1.0f };

int main(void)
{
    GLFWwindow* window = OpenGLInit();

    // 一个新的作用域，让VertexBuffer/IndexBuffer的析构发生在glfwTerminate之前
    // glfwTerminate调用之后，opengl上下文销毁，glGetError会一直返回一个错误，使GLClearError方法进入死循环
    {
        Shader shader("res/shaders/Basic.shader");

        //------------------------------------------------------------

        float vertices[] = {
            // 位置              // 颜色
             0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // 右下
            -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // 左下
             0.5f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f,   // 右上
            -0.5f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // 左上
        };
        unsigned int indices[] = {
            0, 1, 2,
            1, 2, 3
        };

        VertexArray vertexArray;
        VertexBuffer vertexBuffer(vertices, sizeof(vertices));

        VertexBufferLayout layout;
        layout.Push<float>(3);      // 位置属性
        layout.Push<float>(3);      // 颜色属性
        vertexArray.AddBuffer(vertexBuffer, layout);

        IndexBuffer indexBuffer(indices, 6);

        vertexBuffer.UnBind();
        vertexArray.UnBind();

        //------------------------------------------------------------

        while (!glfwWindowShouldClose(window)) {
            ProcessInput(window);

            GLCall(glClearColor(Clear_RGBA[0], Clear_RGBA[1], Clear_RGBA[2], Clear_RGBA[3]));
            GLCall(glClear(GL_COLOR_BUFFER_BIT));

            shader.Bind();
            shader.SetUniform4f("xxColor", 0.5f, 0, 0.5f, 0.5f);

            vertexArray.Bind();

            GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));

            GLCall(glfwSwapBuffers(window));
            GLCall(glfwPollEvents());
        }
    }
    
    glfwTerminate();
    return 0;
}

GLFWwindow* OpenGLInit() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(Scr_Width, Scr_Height, "LearnOpenGL", nullptr, nullptr);
    if (window == nullptr) {
        glfwTerminate();
        throw std::exception("Failed to create GLFW window");
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);

    if (glewInit() != GLEW_OK)
        throw std::exception("Failed to init GLEW");

    return window;
}

void FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void ProcessInput(GLFWwindow* window) {
    // 按下 ECS 键，就关闭窗口
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}