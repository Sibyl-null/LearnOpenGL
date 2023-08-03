#pragma once
#include <string>
#include <unordered_map>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

struct ShaderProgramSource {
	std::string VertexSource;
	std::string FragmentSource;
	std::string GeometrySource;
};

class Shader{
private:
	std::string _filePath;
	unsigned int _renderId;
	std::unordered_map<std::string, int> _uniformLocationCache;
public:
	Shader(const std::string& filePath);
	~Shader();

	void Bind() const;
	void UnBind() const;

	unsigned int GetId() const { return _renderId; }

	void SetUniform1f(const std::string& name, float v);
	void SetUniform2f(const std::string& name, float v0, float v2);
	void SetUniform2f(const std::string& name, glm::vec2 v);
	void SetUniform3f(const std::string& name, float v0, float v1, float v2);
	void SetUniform3f(const std::string& name, glm::vec3 v);
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void SetUniform4f(const std::string& name, glm::vec4 v);
	void SetUniform1i(const std::string& name, int v);
	void SetUniformMat4f(const std::string& name, glm::mat4 trans, bool transpose = false);
private:
	int GetUniformLocation(const std::string& name);
	ShaderProgramSource ParseShader(const std::string& filePath);
	unsigned int CreateShaderProgram(const std::string& vertexShaderSource, 
		const std::string& fragmentShaderSource, const std::string& geometryShaderSource);
	unsigned int CompileShader(unsigned int shaderType, const std::string& shaderSources);
};