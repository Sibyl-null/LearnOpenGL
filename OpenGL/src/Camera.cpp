#include "Camera.h"

Camera::Camera(glm::vec3 pos)
	: _position(pos)
{
}

Camera::~Camera()
{
}

glm::mat4 Camera::GetViewMatrix() const
{
	return glm::lookAt(_position, _position + _lookAtDir, _upDir);
}

void Camera::ProcessKeyboard(GLFWwindow* window, float deltaTime)
{
    float cameraSpeed = 2.5f * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        _position += cameraSpeed * _lookAtDir;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        _position -= cameraSpeed * _lookAtDir;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        _position -= cameraSpeed * glm::normalize(glm::cross(_lookAtDir, _upDir));
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        _position += cameraSpeed * glm::normalize(glm::cross(_lookAtDir, _upDir));
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        _position += cameraSpeed * glm::vec3(0.0, 1.0, 0.0);
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        _position -= cameraSpeed * glm::vec3(0.0, 1.0, 0.0);
}

void Camera::ProcessMouseMovement(double xpos, double ypos) {
    if (_firstMouse) {
        _lastX = (float)xpos;
        _lastY = (float)ypos;
        _firstMouse = false;
    }

    float xOffset = xpos - _lastX;
    // 这里是相反的，ypos从下往上递减
    // 鼠标往上，pitch应该增大，yOffset应该为正
    float yOffset = _lastY - ypos;
    _lastX = (float)xpos;
    _lastY = (float)ypos;

    float sensitivity = 0.05f;    // 灵敏度
    xOffset *= sensitivity;
    yOffset *= sensitivity;

    _yaw += xOffset;
    _pitch += yOffset;

    // 90度时视角会发生逆转，把89度作为极限
    if (_pitch > 89.0f)
        _pitch = 89.0f;
    if (_pitch < -89.0f)
        _pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(_yaw)) * cos(glm::radians(_pitch));
    front.y = sin(glm::radians(_pitch));
    front.z = sin(glm::radians(_yaw)) * cos(glm::radians(_pitch));
    _lookAtDir = glm::normalize(front);
}

void Camera::ProcessMouseScroll(double yoffset) {
    if (_fov >= 1.0f && _fov <= 45.0f)
        _fov -= yoffset;

    if (_fov <= 1.0f)
        _fov = 1.0f;
    if (_fov >= 45.0f)
        _fov = 45.0f;
}