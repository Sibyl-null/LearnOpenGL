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

static Camera camera(glm::vec3(0.0f, 0.0f, 5.0f));

int main(void)
{
    GLFWwindow* window = OpenGLInit();

    // Light sources
    // positions
    std::vector<glm::vec3> lightPositions;
    lightPositions.push_back(glm::vec3(0.0f, 0.5f, 1.5f));
    lightPositions.push_back(glm::vec3(-4.0f, 0.5f, -3.0f));
    lightPositions.push_back(glm::vec3(3.0f, 0.5f, 1.0f));
    lightPositions.push_back(glm::vec3(-.8f, 2.4f, -1.0f));
    // colors
    std::vector<glm::vec3> lightColors;
    lightColors.push_back(glm::vec3(5.0f, 5.0f, 5.0f));
    lightColors.push_back(glm::vec3(10.0f, 0.0f, 0.0f));
    lightColors.push_back(glm::vec3(0.0f, 0.0f, 15.0f));
    lightColors.push_back(glm::vec3(0.0f, 5.0f, 0.0f));

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

        Texture woodTexture("res/textures/wood.png", TextureType::texture_diffuse);
        Texture containerTexture("res/textures/container2.png", TextureType::texture_diffuse);

        Shader basicBloomShader("res/shaders/Bloom/BasicBloom.shader");
        Shader lightBoxShader("res/shaders/Bloom/LightBoxBloom.shader");
        Shader blurShader("res/shaders/Bloom/Blur.shader");
        Shader mixBloomShader("res/shaders/Bloom/MixBloom.shader");

        // ---------------------------------------------------

        unsigned int hdrFBO;
        GLCall(glGenFramebuffers(1, &hdrFBO));
        GLCall(glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO));

        unsigned int colorBuffers[2];
        GLCall(glGenTextures(2, colorBuffers));
        for (unsigned int i = 0; i < 2; ++i) {
            GLCall(glBindTexture(GL_TEXTURE_2D, colorBuffers[i]));
            GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, Scr_Width, Scr_Height, 0, GL_RGBA, GL_FLOAT, nullptr));
            GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
            GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
            GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
            GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
            // 附加到帧缓冲区
            GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, colorBuffers[i], 0))
        }

        unsigned int rboDepth;
        GLCall(glGenRenderbuffers(1, &rboDepth));
        GLCall(glBindRenderbuffer(GL_RENDERBUFFER, rboDepth));
        GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, Scr_Width, Scr_Height));
        GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth));

        unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
        GLCall(glDrawBuffers(2, attachments));

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "Framebuffer not complete!" << std::endl;
        GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));

        // ---------------------------------------------------

        unsigned int pingpongFBO[2];
        unsigned int pingpongColorbuffers[2];
        GLCall(glGenFramebuffers(2, pingpongFBO));
        GLCall(glGenTextures(2, pingpongColorbuffers));
        
        for (unsigned int i = 0; i < 2; ++i) {
            GLCall(glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[i]));
            GLCall(glBindTexture(GL_TEXTURE_2D, pingpongColorbuffers[i]));
            GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, Scr_Width, Scr_Height, 0, GL_RGBA, GL_FLOAT, nullptr));
            GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
            GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
            GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
            GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

            GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingpongColorbuffers[i], 0));
            if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
                std::cout << "Framebuffer not complete!" << std::endl;
        }

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

            // 1. render scene into floating point framebuffer
            GLCall(glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO));
            renderer.Clear();

            basicBloomShader.Bind();
            basicBloomShader.SetUniformMat4f("projection", projection);
            basicBloomShader.SetUniformMat4f("view", view);
            for (unsigned int i = 0; i < 4; ++i) {
                std::string uniformName = "lights[" + std::to_string(i) + "].";
                basicBloomShader.SetUniform3f(uniformName + "Position", lightPositions[i]);
                basicBloomShader.SetUniform3f(uniformName + "Color", lightColors[i]);
            }
            basicBloomShader.SetUniform1i("diffuseTexture", 0);
            basicBloomShader.SetUniform3f("viewPos", camera.GetPosition());

            woodTexture.Bind(0);
            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0));
            model = glm::scale(model, glm::vec3(12.5f, 0.5f, 12.5f));
            basicBloomShader.SetUniformMat4f("model", model);
            renderer.DrawArrays(cubeVAO, basicBloomShader, 36);

            containerTexture.Bind(0);
            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(0.0f, 1.5f, 0.0));
            model = glm::scale(model, glm::vec3(0.5f));
            basicBloomShader.SetUniformMat4f("model", model);
            renderer.DrawArrays(cubeVAO, basicBloomShader, 36);

            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(2.0f, 0.0f, 1.0));
            model = glm::scale(model, glm::vec3(0.5f));
            basicBloomShader.SetUniformMat4f("model", model);
            renderer.DrawArrays(cubeVAO, basicBloomShader, 36);

            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(-1.0f, -1.0f, 2.0));
            model = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
            basicBloomShader.SetUniformMat4f("model", model);
            renderer.DrawArrays(cubeVAO, basicBloomShader, 36);

            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(0.0f, 2.7f, 4.0));
            model = glm::rotate(model, glm::radians(23.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
            model = glm::scale(model, glm::vec3(1.25));
            basicBloomShader.SetUniformMat4f("model", model);
            renderer.DrawArrays(cubeVAO, basicBloomShader, 36);

            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(-2.0f, 1.0f, -3.0));
            model = glm::rotate(model, glm::radians(124.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
            basicBloomShader.SetUniformMat4f("model", model);
            renderer.DrawArrays(cubeVAO, basicBloomShader, 36);

            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(-3.0f, 0.0f, 0.0));
            model = glm::scale(model, glm::vec3(0.5f));
            basicBloomShader.SetUniformMat4f("model", model);
            renderer.DrawArrays(cubeVAO, basicBloomShader, 36);


            lightBoxShader.Bind();
            lightBoxShader.SetUniformMat4f("projection", projection);
            lightBoxShader.SetUniformMat4f("view", view);
            for (unsigned int i = 0; i < lightPositions.size(); i++)
            {
                model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3(lightPositions[i]));
                model = glm::scale(model, glm::vec3(0.25f));
                lightBoxShader.SetUniformMat4f("model", model);
                lightBoxShader.SetUniform3f("lightColor", lightColors[i]);
                renderer.DrawArrays(cubeVAO, lightBoxShader, 36);
            }

            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            // 2. blur bright fragments with two-pass Gaussian Blur 
            bool horizontal = true, first_iteration = true;
            unsigned int amount = 10;
            blurShader.Bind();
            for (unsigned int i = 0; i < amount; i++) {
                glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[horizontal]);
                blurShader.SetUniform1i("horizontal", horizontal);
                blurShader.SetUniform1i("image", 0);
                GLCall(glBindTexture(GL_TEXTURE_2D, first_iteration ? colorBuffers[1] : pingpongColorbuffers[!horizontal]));
                renderer.DrawArrays(quadVAO, blurShader, 6);

                horizontal = !horizontal;
                if (first_iteration)
                    first_iteration = false;
            }

            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            // 3. now render floating point color buffer to 2D quad and tonemap HDR colors to default framebuffer's (clamped) color range
            renderer.Clear();

            mixBloomShader.Bind();
            mixBloomShader.SetUniform1i("scene", 0);
            mixBloomShader.SetUniform1i("bloomBlur", 1);
            mixBloomShader.SetUniform1f("exposure", 1.0f);
            mixBloomShader.SetUniform1i("bloom", true);

            GLCall(glActiveTexture(GL_TEXTURE0));
            GLCall(glBindTexture(GL_TEXTURE_2D, colorBuffers[0]));
            GLCall(glActiveTexture(GL_TEXTURE1));
            GLCall(glBindTexture(GL_TEXTURE_2D, pingpongColorbuffers[!horizontal]));

            renderer.DrawArrays(quadVAO, mixBloomShader, 6);

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