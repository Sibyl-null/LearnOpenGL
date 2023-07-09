#pragma once
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

class Camera{
private:
	glm::vec3 _position;
	glm::vec3 _lookAtDir = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 _upDir = glm::vec3(0.0f, 1.0f, 0.0f);

	bool _firstMouse = true;
	float _pitch = 0.0f;
	float _yaw = -90.0f;
	float _lastX = 400.0f;
	float _lastY = 300.0f;
	float _fov = 45.0f;
public:
	Camera(glm::vec3 pos);
	~Camera();

	glm::vec3 GetPosition() const { return _position; }
	glm::vec3 GetLookAtDir() const { return _lookAtDir; }
	float GetFoV() const { return _fov; }
	glm::mat4 GetViewMatrix() const;
	void ProcessKeyboard(GLFWwindow* window, float deltaTime);
	void ProcessMouseMovement(double xpos, double ypos);
	void ProcessMouseScroll(double yoffset);
};