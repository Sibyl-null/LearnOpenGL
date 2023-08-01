#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <map>
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
#include "Camera.h"
#include "Model.h"
#include "CubeMap.h"

GLFWwindow* OpenGLInit();
void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
void ProcessInput(GLFWwindow* window);
void MouseCallback(GLFWwindow* window, double xpos, double ypos);
void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

const unsigned int Scr_Width = 800;
const unsigned int Scr_Height = 600;

static float deltaTime = 0.0f;  // 当前帧与上一帧的时间差
static float lastTime = 0.0f;   // 上一帧的时间

static Camera camera(glm::vec3(0.0f, 1.0f, 5.0f));

static glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

int main(void)
{
    GLFWwindow* window = OpenGLInit();

    float cubeVertices[] = {
        // positions         
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,

        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,

         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,

        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
    };

    // 一个新的作用域，让VertexBuffer/IndexBuffer的析构发生在glfwTerminate之前
    // glfwTerminate调用之后，opengl上下文销毁，glGetError会一直返回一个错误，使GLClearError方法进入死循环
    {
        Shader redShader("res/shaders/SingleColor/RedColor.shader");
        Shader greenShader("res/shaders/SingleColor/GreenColor.shader");
        Shader blueShader("res/shaders/SingleColor/BlueColor.shader");

        VertexArray cubeVAO;
        VertexBuffer cubeVBO(cubeVertices, sizeof(cubeVertices));
        VertexBufferLayout layout;
        layout.Push<float>(3);
        cubeVAO.AddBuffer(cubeVBO, layout);

        // ---------------------------------------------------

        unsigned int redUniformIndex = glGetUniformBlockIndex(redShader.GetId(), "Matrices");
        unsigned int greenUniformIndex = glGetUniformBlockIndex(greenShader.GetId(), "Matrices");
        unsigned int blueUniformIndex = glGetUniformBlockIndex(blueShader.GetId(), "Matrices");

        GLCall(glUniformBlockBinding(redShader.GetId(), redUniformIndex, 0));
        GLCall(glUniformBlockBinding(greenShader.GetId(), greenUniformIndex, 0));
        GLCall(glUniformBlockBinding(blueShader.GetId(), blueUniformIndex, 0));

        unsigned int uboMatrices;
        GLCall(glGenBuffers(1, &uboMatrices));

        GLCall(glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices));
        GLCall(glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), nullptr, GL_STATIC_DRAW));
        GLCall(glBindBuffer(GL_UNIFORM_BUFFER, 0));

        GLCall(glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboMatrices, 0, 2 * sizeof(glm::mat4)));

        glm::mat4 projection = glm::perspective(glm::radians(camera.GetFoV()),
            (float)Scr_Width / (float)Scr_Height, 0.1f, 100.0f);
        GLCall(glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices));
        GLCall(glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projection)));
        GLCall(glBindBuffer(GL_UNIFORM_BUFFER, 0));

        // ---------------------------------------------------

        Renderer renderer;
        renderer.SetDepthTest(true);

        while (!glfwWindowShouldClose(window)) {
            float currentTime = (float)glfwGetTime();
            deltaTime = currentTime - lastTime;
            lastTime = currentTime;

            ProcessInput(window);
            renderer.Clear();
            // ------------------------------------------------

            glm::mat4 view = camera.GetViewMatrix();
            GLCall(glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices));
            GLCall(glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4),
                sizeof(glm::mat4), glm::value_ptr(view)));
            GLCall(glBindBuffer(GL_UNIFORM_BUFFER, 0));

            redShader.Bind();
            glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(-0.75f, 0.75f, 0.0f));
            redShader.SetUniformMat4f("model", model);
            renderer.DrawArrays(cubeVAO, redShader, sizeof(cubeVertices) / sizeof(float));

            greenShader.Bind();
            model = glm::translate(glm::mat4(1.0f), glm::vec3(0.75f, 0.75f, 0.0f));
            greenShader.SetUniformMat4f("model", model);
            renderer.DrawArrays(cubeVAO, greenShader, sizeof(cubeVertices) / sizeof(float));

            blueShader.Bind();
            model = glm::translate(glm::mat4(1.0f), glm::vec3(-0.75f, -0.75f, 0.0f));
            blueShader.SetUniformMat4f("model", model);
            renderer.DrawArrays(cubeVAO, blueShader, sizeof(cubeVertices) / sizeof(float));

            // ------------------------------------------------
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
    glfwSetCursorPosCallback(window, MouseCallback);
    glfwSetScrollCallback(window, ScrollCallback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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

    camera.ProcessKeyboard(window, deltaTime);
}

void MouseCallback(GLFWwindow* window, double xpos, double ypos) {
    camera.ProcessMouseMovement(xpos, ypos);
}

void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    camera.ProcessMouseScroll(yoffset);
}