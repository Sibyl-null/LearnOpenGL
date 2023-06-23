#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "stb_image/stb_image.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "VertexBufferLayout.h"
#include "Texture.h"

GLFWwindow* OpenGLInit();
void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
void ProcessInput(GLFWwindow* window);

const unsigned int Scr_Width = 800;
const unsigned int Scr_Height = 600;

int main(void)
{
    GLFWwindow* window = OpenGLInit();

    // 一个新的作用域，让VertexBuffer/IndexBuffer的析构发生在glfwTerminate之前
    // glfwTerminate调用之后，opengl上下文销毁，glGetError会一直返回一个错误，使GLClearError方法进入死循环
    {
        Shader shader("res/shaders/Basic.shader");
        shader.Bind();

        //------------------------------------------------------------

        Texture texture1("res/textures/container.jpg");
        Texture texture2("res/textures/awesomeface.png");

        shader.SetUniform1i("texture1", 0);
        shader.SetUniform1i("texture2", 1);

        //------------------------------------------------------------

        float vertices[] = {
        //     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
            0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // 右上
            0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // 右下
           -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 左下
           -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // 左上
        };
        unsigned int indices[] = {
            0, 1, 3,
            1, 2, 3
        };

        VertexArray va;
        VertexBuffer vb(vertices, sizeof(vertices));

        VertexBufferLayout layout;
        layout.Push<float>(3);      // 位置属性
        layout.Push<float>(3);      // 颜色属性
        layout.Push<float>(2);      // 纹理坐标
        va.AddBuffer(vb, layout);

        IndexBuffer ib(indices, 6);

        vb.UnBind();
        va.UnBind();

        //------------------------------------------------------------

        Renderer renderer;

        while (!glfwWindowShouldClose(window)) {
            ProcessInput(window);

            renderer.Clear();

            texture1.Bind(0);
            texture2.Bind(1);

            glm::mat4 model, view, projection;
            model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
            projection = glm::perspective(glm::radians(45.0f), (float)Scr_Width / (float)Scr_Height, 0.1f, 100.0f);

            shader.SetUniformMat4f("model", 1, GL_FALSE, glm::value_ptr(model));
            shader.SetUniformMat4f("view", 1, GL_FALSE, glm::value_ptr(view));
            shader.SetUniformMat4f("projection", 1, GL_FALSE, glm::value_ptr(projection));

            renderer.Draw(va, ib, shader);

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