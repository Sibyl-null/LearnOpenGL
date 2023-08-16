#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <map>
#include <random>
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
float OurLerp(float a, float b, float f);

const unsigned int Scr_Width = 800;
const unsigned int Scr_Height = 600;

static float deltaTime = 0.0f;  // 当前帧与上一帧的时间差
static float lastTime = 0.0f;   // 上一帧的时间

static Camera camera(glm::vec3(0.0f, 0.0f, 5.0f));

int main(void)
{
    GLFWwindow* window = OpenGLInit();

    glm::vec3 lightPos = glm::vec3(2.0, 4.0, -2.0);
    glm::vec3 lightColor = glm::vec3(1.0, 1.0, 1.0);

    float quadVertices[] = {
        // positions        // texture Coords
        -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
         1.0f,  1.0f, 0.0f, 1.0f, 1.0f,

         1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
         -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
         1.0f, -1.0f, 0.0f, 1.0f, 0.0f
    };

    float cubeVertices[] = {
        // Back face
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // Bottom-left
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, // top-right
        0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,  // top-right
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,  // bottom-left
        -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,// top-left
        // Front face
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom-left
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,  // bottom-right
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,  // top-right
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // top-right
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // top-left
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,  // bottom-left
        // Left face
        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-right
        -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top-left
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // bottom-left
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-left
        -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  // bottom-right
        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-right
        // Right face
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-left
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-right
        0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top-right         
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // bottom-right
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,  // top-left
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // bottom-left     
        // Bottom face
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // top-right
        0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f, // top-left
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,// bottom-left
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, // bottom-left
        -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, // bottom-right
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // top-right
        // Top face
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,// top-left
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom-right
        0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, // top-right     
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom-right
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,// top-left
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f // bottom-left        
    };

    // 一个新的作用域，让VertexBuffer/IndexBuffer的析构发生在glfwTerminate之前
    // glfwTerminate调用之后，opengl上下文销毁，glGetError会一直返回一个错误，使GLClearError方法进入死循环
    {
        VertexArray quadVAO;
        VertexBuffer quadVBO(quadVertices, sizeof(quadVertices));
        VertexBufferLayout quadLayout;
        quadLayout.Push<float>(3);
        quadLayout.Push<float>(2);
        quadVAO.AddBuffer(quadVBO, quadLayout);

        VertexArray cubeVAO;
        VertexBuffer cubeVBO(cubeVertices, sizeof(cubeVertices));
        VertexBufferLayout cubeLayout;
        cubeLayout.Push<float>(3);
        cubeLayout.Push<float>(3);
        cubeLayout.Push<float>(2);
        cubeVAO.AddBuffer(cubeVBO, cubeLayout);

        // ---------------------------------------------------

        Model nanosuitModel("res/models/nanosuit/nanosuit.obj");

        Shader gBufferShader("res/shaders/G_buffer.shader");
        Shader ssaoShader("res/shaders/BasicSSAO.shader");
        Shader ssaoBlurShader("res/shaders/BlurSSAO.shader");
        Shader ssaoLightShader("res/shaders/LightSSAO.shader");

        // ---------------------------------------------------

        unsigned int gBuffer;
        GLCall(glGenFramebuffers(1, &gBuffer));
        GLCall(glBindFramebuffer(GL_FRAMEBUFFER, gBuffer));

        // position color buffer
        unsigned int gPositionDepth;
        GLCall(glGenTextures(1, &gPositionDepth));
        GLCall(glBindTexture(GL_TEXTURE_2D, gPositionDepth));
        GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, Scr_Width, Scr_Height, 0, GL_RGBA, GL_FLOAT, nullptr));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
        GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPositionDepth, 0));

        // normal color buffer
        unsigned int gNormal;
        GLCall(glGenTextures(1, &gNormal));
        GLCall(glBindTexture(GL_TEXTURE_2D, gNormal));
        GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, Scr_Width, Scr_Height, 0, GL_RGB, GL_FLOAT, nullptr));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
        GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0));

        // color + specular color buffer
        unsigned int gAlbedoSpec;
        GLCall(glGenTextures(1, &gAlbedoSpec));
        GLCall(glBindTexture(GL_TEXTURE_2D, gAlbedoSpec));
        GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Scr_Width, Scr_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
        GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAlbedoSpec, 0));

        unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
        GLCall(glDrawBuffers(3, attachments));

        unsigned int rboDepth;
        GLCall(glGenRenderbuffers(1, &rboDepth));
        GLCall(glBindRenderbuffer(GL_RENDERBUFFER, rboDepth));
        GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, Scr_Width, Scr_Height));
        GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth));

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "Framebuffer not complete!" << std::endl;
        GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));

        // ---------------------------------------------------

        unsigned int ssaoFBO;
        GLCall(glGenFramebuffers(1, &ssaoFBO));
        GLCall(glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO));

        unsigned int ssaoColorBuffer;
        GLCall(glGenTextures(1, &ssaoColorBuffer));
        GLCall(glBindTexture(GL_TEXTURE_2D, ssaoColorBuffer));
        GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, Scr_Width, Scr_Height, 0, GL_RGB, GL_FLOAT, nullptr));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
        GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoColorBuffer, 0));

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "Framebuffer not complete!" << std::endl;
        GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));

        // ---------------------------------------------------

        unsigned int ssaoBlurFBO;
        GLCall(glGenFramebuffers(1, &ssaoBlurFBO));
        GLCall(glBindFramebuffer(GL_FRAMEBUFFER, ssaoBlurFBO));

        unsigned int ssaoColorBufferBlur;
        GLCall(glGenTextures(1, &ssaoColorBufferBlur));
        GLCall(glBindTexture(GL_TEXTURE_2D, ssaoColorBufferBlur));
        GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, Scr_Width, Scr_Height, 0, GL_RGB, GL_FLOAT, nullptr));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
        GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoColorBufferBlur, 0));

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "Framebuffer not complete!" << std::endl;
        GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));

        // ---------------------------------------------------

        // generate sample kernel
        std::uniform_real_distribution<float> randomFloats(0.0, 1.0);
        std::default_random_engine generator;
        std::vector<glm::vec3> ssaoKernel;
        for (unsigned int i = 0; i < 64; ++i) {
            glm::vec3 sample(randomFloats(generator) * 2.0 - 1.0,
                randomFloats(generator) * 2.0 - 1.0, randomFloats(generator));
            sample = glm::normalize(sample);
            sample *= randomFloats(generator);
            float scale = float(i) / 64.0f;

            // scale samples s.t. they're more aligned to center of kernel
            scale = OurLerp(0.1f, 1.0f, scale * scale);
            sample *= scale;
            ssaoKernel.push_back(sample);
        }

        // generate noise texture
        std::vector<glm::vec3> ssaoNoise;
        for (unsigned int i = 0; i < 16; i++)
        {
            glm::vec3 noise(randomFloats(generator) * 2.0 - 1.0, 
                randomFloats(generator) * 2.0 - 1.0, 0.0f);
            ssaoNoise.push_back(noise);
        }

        unsigned int noiseTexture; 
        GLCall(glGenTextures(1, &noiseTexture));
        GLCall(glBindTexture(GL_TEXTURE_2D, noiseTexture));
        GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, 4, 4, 0, GL_RGB, GL_FLOAT, &ssaoNoise[0]));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));

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

            glm::mat4 projection = glm::perspective(glm::radians(camera.GetFoV()),
                (float)Scr_Width / (float)Scr_Height, 0.1f, 100.0f);
            glm::mat4 view = camera.GetViewMatrix();
            glm::mat4 model;

            // 1. Geometry Pass: render scene's geometry/color data into gbuffer
            GLCall(glBindFramebuffer(GL_FRAMEBUFFER, gBuffer));
            renderer.Clear();
            
            gBufferShader.Bind();
            gBufferShader.SetUniformMat4f("projection", projection);
            gBufferShader.SetUniformMat4f("view", view);

            // Floor cube
            model = glm::translate(model, glm::vec3(0.0, 0.0f, 0.0f));
            model = glm::scale(model, glm::vec3(20.0f, 0.5f, 20.0f));
            gBufferShader.SetUniformMat4f("model", model);
            renderer.DrawArrays(cubeVAO, gBufferShader, 36);

            // Nanosuit model on the floord
            model = glm::mat4();
            model = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.0));
            model = glm::rotate(model, -0.5f * 3.14f, glm::vec3(1.0, 0.0, 0.0));
            model = glm::scale(model, glm::vec3(0.5f));
            gBufferShader.SetUniformMat4f("model", model);
            nanosuitModel.Draw(gBufferShader);

            GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));

            // 2. Create SSAO texture
            GLCall(glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO));
            renderer.Clear();

            ssaoShader.Bind();
            ssaoShader.SetUniform1i("gPositionDepth", 0);
            ssaoShader.SetUniform1i("gNormal", 1);
            ssaoShader.SetUniform1i("texNoise", 2);
            ssaoShader.SetUniformMat4f("projection", projection);

            for (unsigned int i = 0; i < 64; ++i) {
                std::string uniformName = "samples[" + std::to_string(i) + "]";
                ssaoShader.SetUniform3f(uniformName, ssaoKernel[i]);
            }

            GLCall(glActiveTexture(GL_TEXTURE0));
            GLCall(glBindTexture(GL_TEXTURE_2D, gPositionDepth));
            GLCall(glActiveTexture(GL_TEXTURE1));
            GLCall(glBindTexture(GL_TEXTURE_2D, gNormal));
            GLCall(glActiveTexture(GL_TEXTURE2));
            GLCall(glBindTexture(GL_TEXTURE_2D, noiseTexture));

            renderer.DrawArrays(quadVAO, ssaoShader, 6);

            // 3. Blur SSAO texture to remove noise
            GLCall(glBindFramebuffer(GL_FRAMEBUFFER, ssaoBlurFBO));
            renderer.Clear();

            ssaoBlurShader.Bind();
            ssaoBlurShader.SetUniform1i("ssaoInput", 0);

            GLCall(glActiveTexture(GL_TEXTURE0));
            GLCall(glBindTexture(GL_TEXTURE_2D, ssaoColorBuffer));

            renderer.DrawArrays(quadVAO, ssaoBlurShader, 6);

            // 4. Lighting Pass: traditional deferred Blinn-Phong lighting now with added screen-space ambient occlusion
            GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
            renderer.Clear();

            ssaoLightShader.Bind();
            ssaoLightShader.SetUniform1i("gPositionDepth", 0);
            ssaoLightShader.SetUniform1i("gNormal", 1);
            ssaoLightShader.SetUniform1i("gAlbedoSpec", 2);
            ssaoLightShader.SetUniform1i("ssao", 3);

            glm::vec3 lightPosView = glm::vec3(camera.GetViewMatrix() * glm::vec4(lightPos, 1.0));
            ssaoLightShader.SetUniform3f("light.Position", lightPosView);
            ssaoLightShader.SetUniform3f("light.Color", lightColor);
            ssaoLightShader.SetUniform1f("light.Linear", 0.09);
            ssaoLightShader.SetUniform1f("light.Quadratic", 0.032);

            GLCall(glActiveTexture(GL_TEXTURE0));
            GLCall(glBindTexture(GL_TEXTURE_2D, gPositionDepth));
            GLCall(glActiveTexture(GL_TEXTURE1));
            GLCall(glBindTexture(GL_TEXTURE_2D, gNormal));
            GLCall(glActiveTexture(GL_TEXTURE2));
            GLCall(glBindTexture(GL_TEXTURE_2D, gAlbedoSpec));
            GLCall(glActiveTexture(GL_TEXTURE3));
            GLCall(glBindTexture(GL_TEXTURE_2D, ssaoColorBufferBlur));

            renderer.DrawArrays(quadVAO, ssaoShader, 6);

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

float OurLerp(float a, float b, float f){
    return a + f * (b - a);
}