#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <exception>
#include <string>
#include <fstream>
#include <sstream>

struct ShaderProgramSource {
    std::string VertexSource;
    std::string FragmentSource;
};

GLFWwindow* OpenGLInit();
void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
void ProcessInput(GLFWwindow* window);
ShaderProgramSource ParseShader(const std::string& filePath);
unsigned int CompileShader(unsigned int shaderType, const std::string& shaderSources);
unsigned int CreateShaderProgram(const std::string& vertexShaderSource, const std::string& fragmentShaderSource);

const unsigned int Scr_Width = 800;
const unsigned int Scr_Height = 600;
const float Clear_RGBA[4] = { 0.2f, 0.3f, 0.3f, 1.0f };

int main(void)
{
    GLFWwindow* window = OpenGLInit();

    ShaderProgramSource source = ParseShader("res/shaders/Basic.shader");
    unsigned int shaderProgram = CreateShaderProgram(source.VertexSource, source.FragmentSource);

    //------------------------------------------------------------

    float vertices[] = {
        // 位置              // 颜色
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // 右下
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // 左下
         0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // 顶部
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // 位置属性
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // 颜色属性
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);   // 解绑VBO
    glBindVertexArray(0);   // 解绑VAO

    //------------------------------------------------------------

    while (!glfwWindowShouldClose(window)) {
        ProcessInput(window);

        glClearColor(Clear_RGBA[0], Clear_RGBA[1], Clear_RGBA[2], Clear_RGBA[3]);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteProgram(shaderProgram);
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

ShaderProgramSource ParseShader(const std::string& filePath) {
    std::ifstream stream(filePath);

    enum class shaderType {
        None = -1, Vertex = 0, Fragment = 1
    };
    shaderType type = shaderType::None;

    std::stringstream ss[2];
    std::string line;

    while (std::getline(stream, line)) {
        if (line.find("#shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos)
                type = shaderType::Vertex;
            else if (line.find("fragment") != std::string::npos)
                type = shaderType::Fragment;
        }
        else if (type != shaderType::None) {
            ss[(int)type] << line << '\n';
        }
    }

    return { ss[0].str(), ss[1].str() };
}

unsigned int CompileShader(unsigned int shaderType, const std::string& shaderSources) {
    unsigned int shader = glCreateShader(shaderType);
    const char* scr = shaderSources.c_str();
    glShaderSource(shader, 1, &scr, nullptr);
    glCompileShader(shader);

    int result;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(shader, length, &length, message);

        std::cout << "Failed to Compile Shader : "
            << (shaderType == GL_VERTEX_SHADER ? "vertex" : "fragment") << std::endl;
        std::cout << message << std::endl;

        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

unsigned int CreateShaderProgram(const std::string& vertexShaderSource, const std::string& fragmentShaderSource) {
    unsigned int vertexShader = CompileShader(GL_VERTEX_SHADER, vertexShaderSource);
    unsigned int fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

    unsigned int program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
}