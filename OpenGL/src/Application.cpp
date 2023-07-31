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
        // positions          // texture Coords
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    float skyboxVertices[] = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    // 一个新的作用域，让VertexBuffer/IndexBuffer的析构发生在glfwTerminate之前
    // glfwTerminate调用之后，opengl上下文销毁，glGetError会一直返回一个错误，使GLClearError方法进入死循环
    {
        Texture cubeTexture("res/textures/container.jpg", TextureType::texture_diffuse);

        std::vector<std::string> faces{
            "res/textures/skybox/right.jpg",
            "res/textures/skybox/left.jpg",
            "res/textures/skybox/top.jpg",
            "res/textures/skybox/bottom.jpg",
            "res/textures/skybox/front.jpg",
            "res/textures/skybox/back.jpg"
        };
        CubeMap skyboxMap(faces);

        // ---------------------------------------------------

        Shader basicShader("res/shaders/Basic.shader");
        Shader skyboxShader("res/shaders/Skybox.shader");

        VertexArray cubeVAO;
        VertexBuffer cubeVBO(cubeVertices, sizeof(cubeVertices));
        VertexBufferLayout layout;
        layout.Push<float>(3);
        layout.Push<float>(2);
        cubeVAO.AddBuffer(cubeVBO, layout);

        VertexArray skyboxVAO;
        VertexBuffer skyboxVBO(skyboxVertices, sizeof(skyboxVertices));
        VertexBufferLayout skyboxLayout;
        skyboxLayout.Push<float>(3);
        skyboxVAO.AddBuffer(skyboxVBO, skyboxLayout);

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

            glm::mat4 model;
            glm::mat4 view = camera.GetViewMatrix();
            glm::mat4 projection = glm::perspective(glm::radians(camera.GetFoV()),
                (float)Scr_Width / (float)Scr_Height, 0.1f, 100.0f);

            basicShader.Bind();
            basicShader.SetUniformMat4f("model", model);
            basicShader.SetUniformMat4f("view", view);
            basicShader.SetUniformMat4f("projection", projection);
            basicShader.SetUniform1i("texture1", 0);

            skyboxShader.Bind();
            view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
            skyboxShader.SetUniformMat4f("view", view);
            skyboxShader.SetUniformMat4f("projection", projection);
            skyboxShader.SetUniform1i("skybox", 0);

            // cube
            cubeTexture.Bind(0);
            renderer.DrawArrays(cubeVAO, basicShader, sizeof(cubeVertices) / sizeof(float));

            // skybox
            GLCall(glDepthFunc(GL_LEQUAL));
            skyboxMap.Bind();
            renderer.DrawArrays(skyboxVAO, skyboxShader, sizeof(skyboxVertices) / sizeof(float));
            GLCall(glDepthFunc(GL_LESS));

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