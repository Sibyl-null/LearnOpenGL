#include "Shader.h"
#include "Renderer.h"

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

Shader::Shader(const std::string& filePath)
	: _filePath(filePath), _renderId(0)
{
    ShaderProgramSource source = ParseShader(filePath);
    _renderId = CreateShaderProgram(source.VertexSource, source.FragmentSource);
}

Shader::~Shader()
{
    GLCall(glDeleteProgram(_renderId));
}

void Shader::Bind() const
{
    GLCall(glUseProgram(_renderId));
}

void Shader::UnBind() const
{
    GLCall(glUseProgram(0));
}

void Shader::SetUniform1f(const std::string& name, float v)
{
    GLCall(glUniform1f(GetUniformLocation(name), v));
}

void Shader::SetUniform2f(const std::string& name, float v0, float v1)
{
    GLCall(glUniform2f(GetUniformLocation(name), v0, v1));
}

void Shader::SetUniform2f(const std::string& name, glm::vec2 v)
{
    GLCall(glUniform2f(GetUniformLocation(name), v.x, v.y));
}

void Shader::SetUniform3f(const std::string& name, float v0, float v1, float v2)
{
    GLCall(glUniform3f(GetUniformLocation(name), v0, v1, v2));
}

void Shader::SetUniform3f(const std::string& name, glm::vec3 v)
{
    GLCall(glUniform3f(GetUniformLocation(name), v.x, v.y, v.z));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

void Shader::SetUniform4f(const std::string& name, glm::vec4 v)
{
    GLCall(glUniform4f(GetUniformLocation(name), v.x, v.y, v.z, v.w));
}

void Shader::SetUniform1i(const std::string& name, int v)
{
    GLCall(glUniform1i(GetUniformLocation(name), v));
}

void Shader::SetUniformMat4f(const std::string& name, bool transpose, glm::mat4 trans)
{
    GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, transpose, glm::value_ptr(trans)));
}

int Shader::GetUniformLocation(const std::string& name)
{
    if (_uniformLocationCache.find(name) != _uniformLocationCache.end())
        return _uniformLocationCache[name];

    GLCall(int location = glGetUniformLocation(_renderId, name.c_str()));
    if (location == -1)
        std::cout << "Warning: uniform '" << name << "' doesn't exist!" << std::endl;

    _uniformLocationCache[name] = location;
    return location;
}

ShaderProgramSource Shader::ParseShader(const std::string& filePath) {
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

unsigned int Shader::CreateShaderProgram(const std::string& vertexShaderSource, const std::string& fragmentShaderSource) {
    unsigned int vertexShader = CompileShader(GL_VERTEX_SHADER, vertexShaderSource);
    unsigned int fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

    unsigned int program = glCreateProgram();
    GLCall(glAttachShader(program, vertexShader));
    GLCall(glAttachShader(program, fragmentShader));
    GLCall(glLinkProgram(program));
    GLCall(glValidateProgram(program));

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
}

unsigned int Shader::CompileShader(unsigned int shaderType, const std::string& shaderSources) {
    unsigned int shader = glCreateShader(shaderType);
    const char* scr = shaderSources.c_str();
    GLCall(glShaderSource(shader, 1, &scr, nullptr));
    GLCall(glCompileShader(shader));

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